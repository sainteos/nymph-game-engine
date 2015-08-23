#ifndef INVALID_FILE_FORMAT_EXCEPTION_H
#define INVALID_FILE_FORMAT_EXCEPTION_H

namespace Exceptions {
  class InvalidFileFormatException : public std::exception {
    private:
      std::string filename;
    public:
      InvalidFileFormatException(const std::string& filename) {
        this->filename = filename;
      }

      virtual const char* what() const throw() {
        return (std::string("Invalid file format given at: ") + this->filename).c_str();
      }
  };
}

#endif
