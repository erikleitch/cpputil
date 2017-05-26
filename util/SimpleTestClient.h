// $Id: $

#ifndef GCP_UTIL_SIMPLETESTCLIENT_H
#define GCP_UTIL_SIMPLETESTCLIENT_H

/**
 * @file SimpleTestClient.h
 * 
 * Tagged: Fri May 26 12:16:48 PDT 2017
 * 
 * @version: $Revision: $, $Date: $
 * 
 * @author /bin/bash: username: command not found
 */
#include "gcp/util/Client.h"
#include "gcp/util/SimpleTestData.h"

namespace gcp {
    namespace util {

        class SimpleTestClient : public Client {
        public:

            /**
             * Constructor.
             */
            SimpleTestClient(bool spawn,
                             std::string serverHost,
                             unsigned serverPort);

            /**
             * Destructor.
             */
            virtual ~SimpleTestClient();

        private:

            SimpleTestData data_;

            std::vector<unsigned char> bytes_;
            void readServerData(NetHandler& handler);
            virtual void processServerData();
            
        }; // End class SimpleTestClient

    } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_SIMPLETESTCLIENT_H
