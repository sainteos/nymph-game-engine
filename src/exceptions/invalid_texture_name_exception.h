#ifndef INVALID_TEXTURE_NAME_EXCEPTION_H
#define INVALID_TEXTURE_NAME_EXCEPTION_H

namespace Exceptions {
  class InvalidTextureNameException : public std::exception {
    private:
      std::string name;
    public:
      InvalidTextureNameException(const std::string& name) {
        this->name = name;
      }

      virtual const char* what() const throw() {
        return (std::string("Invalid texture name given: ") + this->name).c_str();
      }
  };
}

#endif
