#include "gcp/util/SimpleTestClient.h"

using namespace std;

using namespace gcp::util;

/**.......................................................................
 * Constructor.
 */
SimpleTestClient::SimpleTestClient(bool spawn, std::string host, unsigned port) :
    Client(spawn, host, port)
{
    bytes_.resize(data_.size());
    setReadBufSize(data_.size() + 8);
    setSendBufSize(data_.size() + 8);
}

/**.......................................................................
 * Destructor.
 */
SimpleTestClient::~SimpleTestClient() {}

//------------------------------------------------------------
// read data from the server
//------------------------------------------------------------

void SimpleTestClient::readServerData(NetHandler& handler)
{
    int size;

    handler.getReadStr()->startGet(&size);

    if(size != bytes_.size())
        ThrowError("Received incompatible message size");

    handler.getReadStr()->getChar(size, &bytes_[0]);
    handler.getReadStr()->endGet();

    data_.deserialize(bytes_);

    processServerData();
}

//------------------------------------------------------------
// Do something with it
//------------------------------------------------------------

void SimpleTestClient::processServerData()
{
    COUT("Got data from server: " << data_);
}
