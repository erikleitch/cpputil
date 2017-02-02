#include "gcp/util/Exception.h"
#include "gcp/util/ErrHandler.h"
#include "gcp/util/LogStream.h"

using namespace std;
using namespace gcp::util;

/**.......................................................................
 * Constructor.
 */
ErrHandler::ErrHandler() 
{
  // Initialize the throw function to NULL

  throwFn_  = 0;
  reportFn_ = 0;
  logFn_    = 0;
}

// Create the static instance

//ErrHandler::ErrHandler();

// Initialize static variables

ERR_HANDLER_FN(*ErrHandler::throwFn_);
ERR_HANDLER_FN(*ErrHandler::reportFn_);
ERR_HANDLER_FN(*ErrHandler::logFn_);

/**.......................................................................
 * Destructor.
 */
ErrHandler::~ErrHandler() {}

/**.......................................................................
 * Install a user-defined throw function
 */
void ErrHandler::installThrowFn(ERR_HANDLER_FN(*throwFn))
{
  throwFn_ = throwFn;
}

/**.......................................................................
 * Install a user-defined report function
 */
void ErrHandler::installReportFn(ERR_HANDLER_FN(*reportFn))
{
  reportFn_ = reportFn;
}

/**.......................................................................
 * Install a user-defined log function
 */
void ErrHandler::installLogFn(ERR_HANDLER_FN(*logFn))
{
  logFn_ = logFn;
}

/**.......................................................................
 * Throw an error
 */
ERR_HANDLER_FN(ErrHandler::throwError)
{
  if(throwFn_ == 0)
    defaultThrowFn(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, printHelp);
  else
    throwFn_(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, printHelp);
}

/**.......................................................................
 * Report a message
 */
ERR_HANDLER_FN(ErrHandler::report)
{
  if(reportFn_ == 0)
    defaultReportFn(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, false);
  else
    reportFn_(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, false);
}

/**.......................................................................
 * Log a message
 */
ERR_HANDLER_FN(ErrHandler::log)
{
  if(logFn_ == 0)
    defaultLogFn(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, false);
  else
    logFn_(os, fileName, lineNumber, functionName, isErr, isSimple, isSysErr, false);
}

/**.......................................................................
 * Default throw function
 */
ERR_HANDLER_FN(ErrHandler::defaultThrowFn)
{
  gcp::util::LogStream errStr;

  // Init the message according to type

  errStr.initLogStreamMessage(fileName, lineNumber, functionName, true, !isSimple);

  // If this is a system error, append the system message to it

  if(isSysErr)
    errStr.appendSysLogStreamError(os.str(), fileName, lineNumber, functionName, true, true);
  else
    errStr << os.str();

  // Log the error

  errStr.log();

  // Finally, throw it

  throw gcp::util::Exception(errStr, fileName, lineNumber, true, printHelp);
}

/**.......................................................................
 * Default report function
 */
ERR_HANDLER_FN(ErrHandler::defaultReportFn)
{
  gcp::util::LogStream errStr;

  // Init the message according to type

  errStr.initLogStreamMessage(fileName, lineNumber, functionName, isErr, !isSimple);

  // If this is a system error, append the system message to it

  if(isSysErr)
    errStr.appendSysLogStreamError(os.str(), fileName, lineNumber, functionName, isErr, true);
  else
    errStr << os.str();

  // Report the error

  errStr.report();
}

/**.......................................................................
 * Default log function
 */
ERR_HANDLER_FN(ErrHandler::defaultLogFn)
{
  gcp::util::LogStream errStr;

  // Init the message according to type

  errStr.initLogStreamMessage(fileName, lineNumber, functionName, isErr, !isSimple);

  // If this is a system error, append the system message to it

  if(isSysErr)
    errStr.appendSysLogStreamError(os.str(), fileName, lineNumber, functionName, isErr, true);
  else
    errStr << os.str();

  // Report the error

  errStr.log();
}
