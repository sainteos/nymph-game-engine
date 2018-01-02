#ifndef INVALID_FILENAME_EXCEPTION_H
#define INVALID_FILENAME_EXCEPTION_H

#include <exception>
#include <string>

namespace Exceptions {
  class InvalidFilenameException : public std::exception {
    private:
      std::string filename;
    public:
      InvalidFilenameException(const std::string& filename) {
        this->filename = filename;
      }

      virtual const char* what() const throw() {
        return (std::string("Invalid filename given: ") + this->filename).c_str();
      }
  };
}

#endif
