#ifndef INVALID_UNIFORM_NAME_EXCEPTION_H
#define INVALID_UNIFORM_NAME_EXCEPTION_H
#include <string>
#include <exception>

namespace Exceptions {
  class InvalidUniformNameException : public std::exception {
    private:
      std::string name;
    public:
      InvalidUniformNameException(const std::string name) {
        this->name = name;
      }

      virtual const char* what() const throw() {
        return (std::string("Invalid uniform name: ") + this->name).c_str();
      }
  };
}
#endif
