#include <iostream>
#include <iomanip>

#include <cmath>

#include "gcp/program/Program.h"

#include "gcp/util/Angle.h"
#include "gcp/util/HourAngle.h"
#include "gcp/util/Declination.h"
#include "gcp/util/Mass.h"

using namespace std;
using namespace gcp::util;
using namespace gcp::program;

KeyTabEntry Program::keywords[] = {
  { "rad",        "0", "d", "Radians"},
  { END_OF_KEYWORDS}
};

class ScopedLock {
public:
    
    ScopedLock(Mutex& guard) {
        COUT("locking mutex");
        guard_ = &guard;
        guard_->lock();
    };
    
    ~ScopedLock() {
        COUT("unlocking mutex");
        guard_->unlock();
    };

private:
    Mutex* guard_;
};


void Program::initializeUsage() {};

Mutex globalMutex;

void testFn()
{
    ScopedLock lock(globalMutex);

    COUT("Body of code");

    ThrowError("Throwing an error");
}

int Program::main()
{
    try {
        testFn();
    } catch(...) {
        COUT("Caught an error");
    }
    return 0;
}

