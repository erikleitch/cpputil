#include <iostream>
#include <sstream>
#include <cmath>

#include "gcp/util/Debug.h"
#include "gcp/util/Exception.h"
#include "gcp/util/LogStream.h"
#include "gcp/util/SimpleTestServer.h"

#include "gcp/program/Program.h"

using namespace std;
using namespace gcp::util;
using namespace gcp::program;

KeyTabEntry Program::keywords[] = {
    { "port",    "4358",      "i", "Server port to listen on"},
    {END_OF_KEYWORDS}
};

void Program::initializeUsage() {}

int Program::main()
{
    SimpleTestServer server(false,
                            Program::getIntegerParameter("port"));
    
    try {
        server.run();
    } catch (Exception& err) {
        COUT("Caught an exception: " << err.what());
        return 1;
    } catch (...) {
        COUT("Caught an exception");
        return 1;
    }

    return 0;
}
