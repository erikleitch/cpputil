#define __FILEPATH__ "util/AbsTimer.cc"

#include <iostream>
#include <sstream>
#include <list>

#include <iomanip>
#include <stdlib.h>
#include <errno.h>

#include "gcp/util/AbsTimer.h"
#include "gcp/util/Exception.h"
#include "gcp/util/LogStream.h"

#define DEBUG

// Number of nanoseconds per second

#define NSEC_PER_SEC 1000000000

using namespace std;
using namespace gcp::util;

/**.......................................................................
 * Intialization of the static list of known timers
 */
list<AbsTimer::TimerId> AbsTimer::timerList_;

/**.......................................................................
 * Add a timer to the list of known timers. 
 */
void AbsTimer::addTimer(timer_t* timer, int sigNo)
{
  // Add the new timer if a timer for this signal doesn't already
  // exist

  if(!timerAlreadyExists(sigNo)) {
    TimerId newtimer(timer, sigNo);
    timerList_.insert(timerList_.begin(), newtimer);

    sigNo_ = newtimer.sigNo_;

  } else {
    ThrowError("timer already exists for this signal");
  }
}

/**.......................................................................
 * Remove a timer from the list of known timers
 */
void AbsTimer::remTimer(timer_t* timer)
{
  AbsTimer::timerList_.remove_if(TimerId_eq(timer));
}

/**.......................................................................
 * Check if a timer for this signal already exists
 */
bool AbsTimer::timerAlreadyExists(int sigNo)
{
  list<TimerId>::iterator ilist = find_if(timerList_.begin(), timerList_.end(),
					  TimerSig_eq(sigNo));
  return  (ilist != timerList_.end());
}

/**.......................................................................
 *  Constructor.  Create a timer for the requested signal, with no handler.
 */
AbsTimer::AbsTimer(int sigNo) 
{
  privateConstructor(sigNo, NULL);
}

AbsTimer::AbsTimer(int sigNo, void (*handler)(int)) 
{
  privateConstructor(sigNo, handler);
}

/**.......................................................................
 *  Private constructor.  Create a timer for the requested signal, installing
 *  a handler to deal with the signal.
 */
void AbsTimer::privateConstructor(int sigNo, void (*handler)(int)) 
{
  // Initialize members

  initSec_         = 0;
  initNanoSec_     = 0;
  intervalSec_     = 0;
  intervalNanoSec_ = 0;
  isRunning_       = false; // Default to disabled
  integral_        = true;  // Default to start on integral second
		            // boundaries
  reArm_           = false;
  periodic_        = false; // Default to once-only timers.
  timer_           = 0;     // Initialize the timer to 0
  
  // Before we do anything, make sure a timer doesn't already exist
  // for this signal

  if(timerAlreadyExists(sigNo)) 
    ThrowError("Timer already exists for this signal!");

  // See if we can create the timer

  if(sysconf(_SC_TIMER_MAX)==0)
    ThrowError("AbsTimer::AbsTimer: No timers available");

  // See if the requested signal is a valid real-time signal

  if(sigNo < SIGRTMIN) 
    ThrowError("Invalid signal: " << sigNo);

  // Now setup a signal handler for this timer, if one was passed.

  if(handler != 0) {

    struct sigaction sat;
  
    sigemptyset(&sat.sa_mask);

    sat.sa_flags   = SA_SIGINFO;
    sat.sa_flags  |= SA_RESTART; // If specified, interrupted system
				 // calls will be restarted
    sat.sa_handler = handler;
    
    if(sigaction(sigNo, &sat, NULL) != 0) 
      ThrowError("Failed in sigaction");
  }
  
  // Create a timer.  Set up the sigevent structure to associate this
  // timer with the requested signal.
  
  evp_.sigev_notify = SIGEV_SIGNAL;
  evp_.sigev_signo  = sigNo;
  
#if DIR_HAVE_RT

  if(timer_create(DEFAULT_CLOCK, &evp_, &timer_) < 0) 
    ThrowSysError("in timer_create()");

#endif

  // And add the new timer to the static list of known timers

  addTimer(&timer_, sigNo);
}

/**.......................................................................
 *  Destructor.  Remove a timer.
 */
AbsTimer::~AbsTimer()
{
  // Stop the timer

  stop();

  // Remove this timer from the list of allocated timers
  
  remTimer(&timer_);

  // And delete it

#if DIR_HAVE_RT
  timer_delete(timer_);
#endif

}

/**.......................................................................
 * Return the resolution of the default clock
 */
unsigned long AbsTimer::getResolution() 
{
  struct timespec resolution;
  
#if DIR_HAVE_RT

  if (clock_getres(DEFAULT_CLOCK, &resolution) < 0) 
    ThrowError("Cannot get Resolution for this clock");
  
  return  resolution.tv_nsec;
#else
  return 1;
#endif
}

/**.......................................................................
 * Set the amount of time we will delay after calling start() before
 * this timer goes off
 */
void AbsTimer::setInitialDelay(unsigned long sec, unsigned long nsec) 
{
  // Set the initial delay, correcting for possible nsec >
  // NSEC_PER_SEC

  fix(sec, nsec, initSec_, initNanoSec_);
}

/**.......................................................................
 * Set the interval between successive firings of this timer
 */
void AbsTimer::setIntervalDelay(unsigned long sec, unsigned long nsec) 
{
  // Set the interval, correcting for possible nsec > NSEC_PER_SEC

  fix(sec, nsec, intervalSec_, intervalNanoSec_);

  // If a non-zero interval was specified, this is a periodic timer.

  if(intervalSec_ > 0 || intervalNanoSec_ > 0)
    periodic_ = true;
}

/**.......................................................................
 * Start the timer
 */
void AbsTimer::start(bool reArm) 
{
  reArm_ = reArm;

  // Get the current absolute time
  
  struct timespec absolute;

#if DIR_HAVE_RT
  clock_gettime(DEFAULT_CLOCK, &absolute);
#endif

  // Set up this timer to expire relative to the current absolute time

  unsigned long sec, nsec;

  if(!integral_) {

    // If we are not starting wrt to integral second boundaries, just
    // add the times to get the initial start time.
    
    nsec = absolute.tv_nsec + initNanoSec_;
    sec  = (absolute.tv_sec + initSec_) + (nsec / NSEC_PER_SEC);

  }   else {
    
    // Else start relative to the next closest second boundary
    // (absolute.tv_sec + 1).

    nsec = initNanoSec_;
    sec  = (absolute.tv_sec + 1 + initSec_);
  }

  // Now correct for the possiblity of getting nsec > NSEC_PER_SEC.

  fix(sec, nsec, sec, nsec);

  // If this is a normal periodic timer, just set the interval and be
  // done with it.

  if(!integral_)
    setFutureTime(sec, nsec, intervalSec_, intervalNanoSec_);

  // If it is desired that this timer fire relative to integral second
  // boundaries, we should re-set the future time each time that the
  // timer expires, or we can accumulate significant timing errors.

  else 
    setFutureTime(sec, nsec, (reArm_ ? 0 : intervalSec_), 
		  (reArm_ ? 0 : intervalNanoSec_));

  // Set the target time for expiry of this timer

  targetSec_     = sec;
  targetNanoSec_ = nsec;

  isRunning_ = true;
}

/**.......................................................................
 * Set a timer to go off at the specified absolute time
 */
void AbsTimer::start(unsigned long sec, unsigned long nsec) 
{
  // Now correct for the possiblity of getting nsec > NSEC_PER_SEC.

  fix(sec, nsec, sec, nsec);
  
  setFutureTime(sec, nsec, 0, 0);

  isRunning_ = true;
}

/**.......................................................................
 * Stop the timer
 */
void AbsTimer::stop() 
{
  setFutureTime(0, 0, 0, 0);
  isRunning_ = false;
}

/**.......................................................................
 * Private function to set up timing conditions for a timer
 */
void AbsTimer::setFutureTime(unsigned long initSec, 
			     unsigned long initNanoSec, 
			     unsigned long intervalSec, 
			     unsigned long intervalNanoSec)
{
#if DIR_HAVE_RT

  struct itimerspec new_setting;

  new_setting.it_value.tv_sec     = initSec;
  new_setting.it_value.tv_nsec    = initNanoSec;
  new_setting.it_interval.tv_sec  = intervalSec;
  new_setting.it_interval.tv_nsec = intervalNanoSec;

  if(timer_settime(timer_, TIMER_ABSTIME, &new_setting, NULL) < 0) 
    ReportSysError("timer_settime");

#endif
}

/**.......................................................................
 * A query function to see if this timer has been started
 */
bool AbsTimer::isRunning()
{
  return isRunning_;
}

/**.......................................................................
 * A debugging function, to print out the signals of all know timers
 */
void AbsTimer::checkTimer()
{
  for(list<AbsTimer::TimerId>::iterator ilist=AbsTimer::timerList_.begin();  
      ilist != AbsTimer::timerList_.end();ilist++) {
    cout << ilist->sigNo_ << endl;
  }
}

/**.......................................................................
 * Set whether this clock should run on integral second
 * boundaries relative to the current time.
 */
void AbsTimer::setIntegral(bool integral)
{
  integral_ = integral;
}

/**.......................................................................
 * Re-arm a periodic integral timer.
 */
void AbsTimer::reArm()
{
  // This is a no-op unless the timer is a periodic timer we want to
  // fire relative to integral second boundaries.

  if(integral_ && periodic_ && reArm_) {

    // Calculate the absolute time at which we want this timer to
    // expire.  For integral periodic timers, this will be the last
    // target time (stored in targetSec_ & targetNanoSec_
    // variables), plus the time interval.

    unsigned long sec  = targetSec_     + intervalSec_;
    unsigned long nsec = targetNanoSec_ + intervalNanoSec_;

    fix(sec, nsec, sec, nsec);

    setFutureTime(sec, nsec, 0, 0);

    targetSec_     = sec;
    targetNanoSec_ = nsec;
  }
}

/**.......................................................................
 * Shift time intervals around so that nsec arg is always <
 * NSEC_PER_SEC
 */
void AbsTimer::fix(unsigned long inSec,   unsigned long inNsec,
		   unsigned long& outSec, unsigned long& outNsec)
{
  while(inNsec >= NSEC_PER_SEC) {
    inNsec -= NSEC_PER_SEC;
    inSec  += 1;
  }

  outSec  = inSec;
  outNsec = inNsec;
}
