#include <iostream>
#include <iomanip>

#include <cmath>
#include <unistd.h>

#include <sys/select.h>

#include "gcp/program/Program.h"

#include "gcp/util/FdSet.h"
#include "gcp/util/SerialClient.h"

using namespace std;
using namespace gcp::util;
using namespace gcp::program;

KeyTabEntry Program::keywords[] = {
  { "baud",        "9600", "i", "Baud rate"},
  { END_OF_KEYWORDS}
};

void Program::initializeUsage() {};

int Program::main()
{
    SerialClient client("/dev/tty.usbserial4", Program::getIntegerParameter("baud"));
    COUT("Connect returned " << client.connect());
    
    std::string message = "Hello";
    client.writeString(message);

    sleep(5);

    message = "Goodbye";
    client.writeString(message);

    sleep(1);
    return 0;
}
