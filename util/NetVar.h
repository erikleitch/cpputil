// $Id: NetVar.h,v 1.1.1.1 2005/10/21 23:48:24 eml Exp $

#ifndef GCP_UTIL_NETVAR_H
#define GCP_UTIL_NETVAR_H

/**
 * @file gcp/util/NetDat.h
 * 
 * Tagged: Wed Jul  6 13:31:47 PDT 2005
 * 
 * @version: $Revision: 1.1.1.1 $, $Date: 2005/10/21 23:48:24 $
 * 
 * @author Erik Leitch
 */
#include "gcp/util/DataType.h"

#include "gcp/util/NetDat.h"

namespace gcp {
  namespace util {

    class NetStruct;

    class NetVar : public NetDat {
    public:

      /**
       * Constructor.
       */
      NetVar(gcp::util::DataType::Type type, void* vPtr, unsigned nEl);

      /**
       * Destructor.
       */
      virtual ~NetVar();

      /**
       * Copy constructor
       */
      NetVar(const NetVar& netVar);
      NetVar(NetVar& netVar);

      /**
       * Deserialize data into this object
       */
      void deserialize(const std::vector<unsigned char>& bytes);

      unsigned size();

    private:

      // The type of this variable

      gcp::util::DataType::Type type_;

      // Pointer to where the start of the memory resides for this
      // variable

      void* vPtr_;

      // Number of elements in this variable

      unsigned nEl_;

      /**
       * Size of this variable
       */
      unsigned size_;

      /**
       * Serialize the data for this variable
       */
      void serialize();

      /**
       * Deserialize data into this object
       */
      void deserialize(const unsigned char* bytes);

      /**
       * Resize
       */
      void resize(unsigned size);

    }; // End class NetVar

  } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_NETVAR_H
