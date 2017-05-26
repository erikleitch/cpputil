// $Id: emacs_macros,v 1.2 2011/08/15 20:25:21 abeard Exp $

#ifndef GCP_UTIL_FILESTREAM_H
#define GCP_UTIL_FILESTREAM_H

/**
 * @file FileStream.h
 * 
 * Tagged: Wed Feb 29 13:21:01 PST 2012
 * 
 * @version: $Revision: 1.2 $, $Date: 2011/08/15 20:25:21 $
 * 
 * @author username: Command not found.
 */
#include "gcp/util/Exception.h"

#include <string>
#include <unistd.h>

namespace gcp {
  namespace util {

    // A base class for handling file streams

    class FileStream {
    public:

      // Constructor.

      FileStream() {
	fileSizeIsSet_ = false;
      };

      // Destructor.

      virtual ~FileStream() {
	close();
      }

      // Open this file for reading

      virtual void openForRead(std::string fileName)=0;

      // Close this file

      virtual void close() {
      }

      // Read nBytes out of the file, from the current location

      virtual ssize_t read(void* buf, size_t nBytes)=0;

      // Seek to the requested location in the file

      virtual off_t seek(long offset, int whence)=0;

      // Check that we have a valid file handle

      virtual void checkStream()=0;

      // Return the uncompressed size of this file in bytes

      long uncompressedFileSizeInBytes()
      {
	if(fileSizeIsSet_) {
	  return fileSizeInBytes_;
	}

	ThrowError("File size isn't set");
	return 0;
      }
 
      void setUncompressedFileSizeInBytes(long fileSizeInBytes)
      {
	fileSizeInBytes_ = fileSizeInBytes;
	fileSizeIsSet_   = true;
      }

    protected:

      std::string fileName_;
      long fileSizeInBytes_;
      bool fileSizeIsSet_;

    }; // End class FileStream

  } // End namespace util
} // End namespace gcp



#endif // End #ifndef GCP_UTIL_FILESTREAM_H
