#include "gcp/util/SimpleTestData.h"

using namespace std;

using namespace gcp::util;

/**.......................................................................
 * Constructor.
 */
SimpleTestData::SimpleTestData()
{
    NETSTRUCT_UINT(testInt_);
}

/**.......................................................................
 * Destructor.
 */
SimpleTestData::~SimpleTestData()
{
}

std::ostream&
gcp::util::operator<<(std::ostream& os, SimpleTestData& data)
{
    os << data.testInt_ << endl;
    return os;
}


