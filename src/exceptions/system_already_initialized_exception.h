#ifndef SYSTEM_ALREADY_INITIALIZED_EXCEPTION_H
#define SYSTEM_ALREADY_INITIALIZED_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class SystemAlreadyInitializedException : public std::exception {
    private:
      std::string system_name;
    public:
      SystemAlreadyInitializedException(std::string system_name) {
        this->system_name = system_name;
      }

      virtual const char* what() const throw() {
        return this->system_name.c_str(); + " has already been initialized! Cannot reinitialize!";
      }
  };
}

#endif