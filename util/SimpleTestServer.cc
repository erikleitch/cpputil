#include "gcp/util/SimpleTestServer.h"

using namespace std;

using namespace gcp::util;

/**.......................................................................
 * Constructor.
 */
SimpleTestServer::SimpleTestServer(bool spawn, unsigned listenPort) :
    Server(spawn, listenPort)
{
    setReadBufSize(data_.size() + 8);
    setSendBufSize(data_.size() + 8);
}

/**.......................................................................
 * Destructor.
 */
SimpleTestServer::~SimpleTestServer() {}

void SimpleTestServer::readClientData(NetHandler& handler)
{
    int size;

    handler.getReadStr()->startGet(&size);

    if(size != bytes_.size())
        ThrowError("Received incompatible message size");

    handler.getReadStr()->getChar(size, &bytes_[0]);
    handler.getReadStr()->endGet();

    data_.deserialize(bytes_);

    processClientData();
}

//------------------------------------------------------------
// Do something with it
//------------------------------------------------------------

void SimpleTestServer::processClientData()
{
    COUT("Got data from client: " << data_);
}

void SimpleTestServer::acceptClientAction()
{
    data_.testInt_ = 1234;
    sendClientData(data_);
}
