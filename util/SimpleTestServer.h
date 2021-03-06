// $Id: $

#ifndef GCP_UTIL_SIMPLETESTSERVER_H
#define GCP_UTIL_SIMPLETESTSERVER_H

/**
 * @file SimpleTestServer.h
 * 
 * Tagged: Fri May 26 12:13:05 PDT 2017
 * 
 * @version: $Revision: $, $Date: $
 * 
 * @author /bin/bash: username: command not found
 */
#include "gcp/util/Server.h"
#include "gcp/util/SimpleTestData.h"

namespace gcp {
    namespace util {

        class SimpleTestServer : public Server {
        public:

            /**
             * Constructor.
             */
            SimpleTestServer(bool spawn,
                             unsigned listenPort);

            /**
             * Destructor.
             */
            virtual ~SimpleTestServer();

            void acceptClientAction();

        private:

            SimpleTestData data_;

            std::vector<unsigned char> bytes_;
            void readClientData(NetHandler& handler);
            virtual void processClientData();

        }; // End class SimpleTestServer

    } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_SIMPLETESTSERVER_H
