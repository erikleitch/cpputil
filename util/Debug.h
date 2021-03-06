#ifndef GCP_UTIL_DEBUG_H
#define GCP_UTIL_DEBUG_H

/**
 * @file Debug.h
 * 
 * Started: Wed Jan 14 11:00:24 PST 2004
 * 
 * @author Erik Leitch
 */
#include <iostream>
#include "gcp/util/IoLock.h"
#include "gcp/util/Mutex.h"
#include "gcp/util/TimeVal.h"
#include "gcp/util/Directives.h"

#if DIR_DEBUG
#define DBPRINT(doprint, level, statement) \
if(doprint && gcp::util::Debug::debugging(level)) {\
    gcp::util::TimeVal timeVal; \
    timeVal.setToCurrentTime(); \
    gcp::util::IoLock::lockCout(); \
    std::cout << timeVal \
              <<  ": In " \
              << __PRETTY_FUNCTION__ << ": "\
              << statement << std::endl; \
    gcp::util::IoLock::unlockCout(); \
}
#else
#define DBPRINT(doprint, level, statement) {}
#endif

#define DBPRINT_SIMPLE(doprint, level, statement) {}

// Some handy predefines

#define DEBUG_CAN       gcp::util::Debug::DEBUG1
#define DEBUG_CALTERT   gcp::util::Debug::DEBUG2
#define DEBUG_SIGNAL    gcp::util::Debug::DEBUG3
#define DEBUG_DELAY     gcp::util::Debug::DEBUG4

#define DEBUG_PMAC      gcp::util::Debug::DEBUG6
#define DEBUG_TRACKER   gcp::util::Debug::DEBUG7
#define DEBUG_IFMOD     gcp::util::Debug::DEBUG8

namespace gcp {
  namespace util {
    
    class Debug {
    public:

      enum Level {
	DEBUGNONE = 0x0,
	DEBUG1    = 0x1,
	DEBUG2    = 0x2,
	DEBUG3    = 0x4,
	DEBUG4    = 0x8,
	DEBUG5    = 0x10,
	DEBUG6    = 0x20,
	DEBUG7    = 0x40,
	DEBUG8    = 0x80,
	DEBUG9    = 0x100,
	DEBUG10   = 0x200,
	DEBUG11   = 0x400,
	DEBUG12   = 0x800,
	DEBUG13   = 0x1000,
	DEBUG14   = 0x2000,
	DEBUG15   = 0x4000,
	DEBUG16   = 0x8000,
	DEBUG17   = 0x10000,
	DEBUG18   = 0x20000,
	DEBUG19   = 0x40000,
	DEBUG20   = 0x80000,
	DEBUG21   = 0x100000,
	DEBUG22   = 0x200000,
	DEBUG23   = 0x400000,
	DEBUG24   = 0x800000,
	DEBUG25   = 0x1000000,
	DEBUG26   = 0x2000000,
	DEBUG27   = 0x4000000,
	DEBUG28   = 0x8000000,
	DEBUG29   = 0x10000000,
	DEBUG30   = 0x20000000,
	DEBUG31   = 0x40000000,
	DEBUG32   = 0x80000000,
	DEBUGANY  = 0xFFFFFFFF
      };

      /**
       * Public method to turn debugging on/off
       */
      static void setLevel(Level level);

      /**
       * Public method to turn debugging on/off
       */
      static void setLevel(unsigned int);

      /**
       * Add a level to the current debug level
       */
      static void addLevel(Level level);

      /**
       * Remove a level from the current debug level
       */
      static void remLevel(Level level);

      /**
       * Public method to query debugging state.
       */
      static bool debugging(Level level = Debug::DEBUGANY);
      
      static void lock();
      static void unlock();

    private:

      /**
       * A mutex to protect cout
       */
      static Mutex mutex_;

      /**
       * Debug level.
       */
      static Level level_;

      /**
       * Private constructor prevents instantiation
       */
      Debug();

    }; // End class Debug
    
  }; // End namespace util
}; // End namespace gcp

#endif // End #ifndef 
