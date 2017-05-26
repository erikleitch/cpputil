#include <iostream>
#include <sstream>
#include <cmath>

#include "gcp/util/Debug.h"
#include "gcp/util/Exception.h"
#include "gcp/util/LogStream.h"
#include "gcp/util/SimpleTestClient.h"

#include "gcp/program/Program.h"

using namespace std;
using namespace gcp::util;
using namespace gcp::program;

KeyTabEntry Program::keywords[] = {
    { "port",    "4358",      "i", "Server port to connect to"},
    { "host",    "localhost", "s", "Server host to connect to"},
    {END_OF_KEYWORDS}
};

void Program::initializeUsage() {}

int Program::main()
{
    SimpleTestClient client(true,
                            Program::getStringParameter("host"),
                            Program::getIntegerParameter("port"));
    
    try {
        COUT("Running clinet");
        client.spawn();
        client.connect();
        client.blockForever();
    } catch (Exception& err) {
        COUT("Caught an exception: " << err.what());
        return 1;
    } catch (...) {
        COUT("Caught an exception");
        return 1;
    }

    return 0;
}    
