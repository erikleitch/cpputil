#include <iostream>
#include <iomanip>

#include <cmath>
#include <unistd.h>

#include <sys/select.h>

#include "gcp/program/Program.h"

#include "gcp/util/FdSet.h"
#include "gcp/util/SerialClient.h"
#include "gcp/util/Vector.h"

using namespace std;
using namespace gcp::util;
using namespace gcp::program;

KeyTabEntry Program::keywords[] = {
  { "baud",        "9600", "i", "Baud rate"},
  { END_OF_KEYWORDS}
};

enum Byte {
    BTH_B0   = 1,
    BTH_T3T4 = 2,
    BTH_T1T2 = 3,
    BTH_H1H2 = 4,
    BTH_D1D2 = 5,
    BTH_P1P2 = 6,
    BTH_F1F2 = 7,
    BTH_UNK  = 8,
    BTH_S3S4 = 9,
    BTH_S1S2 = 10,
    BTH_C1C2 = 11
};

void Program::initializeUsage() {};
void processBTHMessage(std::string bytes);
void processMessage(std::string bytes);
int getIntFromHexInt(int d3d4, int d1d2);
int getIntFromHexShort(int d1d2);

int Program::main()
{
    SerialClient client("/dev/cu.usbserial", Program::getIntegerParameter("baud"));
    COUT("Connect returned " << client.connect());
    
    FdSet fdSet;
    fdSet.registerReadFd(client.getFd());

    Vector<unsigned char> bytes;
    std::ostringstream os;

    unsigned nStart = 0;
    bool first = true;
    
    int nready=0;
    do {
        nready = select(fdSet.size(), fdSet.readFdSet(), NULL, NULL, NULL);

        client.readBytes(bytes);

        for(unsigned i=0; i < bytes.size(); i++) {

            CTOUT("Bytes = " << hex << "0x" << (int)bytes[i]);
            
            if(bytes[i] == 0xff) {
                ++nStart;
                
                if(nStart > 2) {
                    processMessage(os.str());
                    os.str("");
                    nStart = 1;
                }
                
            } else {
                os << bytes[i];
            }
        }

    } while(true);
    
    return 0;
}

int getIntFromHexInt(int d3d4, int d1d2)
{
    int t4 = (d3d4 & 0x0F);
    int t3 = (d3d4 & 0xF0) >> 4;

    int t2 = (d1d2 & 0x0F);
    int t1 = (d1d2 & 0xF0) >> 4;

    return t4 + 10*(t3 + 10*(t2 + 10*t1));
}

int getIntFromHexShort(int d1d2)
{
    int t2 = (d1d2 & 0x0F);
    int t1 = (d1d2 & 0xF0) >> 4;

    return t2 + 10*t1;
}

void processBTHMessage(std::string bytes)
{
    COUT("Temp             = " << (float)(getIntFromHexInt(bytes[BTH_T3T4], bytes[BTH_T1T2]))/10 << " C");
    COUT("Humidity         = " << getIntFromHexShort(bytes[BTH_H1H2]) << " %");
    COUT("DewPoint         = " << getIntFromHexShort(bytes[BTH_D1D2]) << " C");
    COUT("Pressure         = " << getIntFromHexShort(bytes[BTH_P1P2]) << " mb");
    COUT("Sea-level Offset = " << getIntFromHexInt(bytes[BTH_S3S4], bytes[BTH_S1S2]) << " mb");
}

void processMessage(std::string bytes)
{
    int code = (int)bytes[0];
    COUT("Found code " << hex << "0x" << code);
    switch (code) {
    case 0x6:
        processBTHMessage(bytes);
        break;
    case 0xe:
//        processClockMinuteMessage(bytes);
        break;
    default:
        COUT("Unrecognized message code: " << hex << "0x" << code);
        break;
    }
}

