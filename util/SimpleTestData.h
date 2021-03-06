// $Id: $

#ifndef GCP_UTIL_SIMPLETESTDATA_H
#define GCP_UTIL_SIMPLETESTDATA_H

/**
 * @file SimpleTestData.h
 * 
 * Tagged: Fri May 26 12:09:59 PDT 2017
 * 
 * @version: $Revision: $, $Date: $
 * 
 * @author /bin/bash: username: command not found
 */
#include "gcp/util/NetStruct.h"

namespace gcp {
    namespace util {

        class SimpleTestData : public NetStruct {
        public:

            /**
             * Constructor.
             */
            SimpleTestData();

            /**
             * Destructor.
             */
            virtual ~SimpleTestData();

            friend std::ostream&
                operator<<(std::ostream& os, SimpleTestData& data);
            
            unsigned testInt_;
            
        }; // End class SimpleTestData

        std::ostream& operator<<(std::ostream& os, SimpleTestData& data);
        
    } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_SIMPLETESTDATA_H
