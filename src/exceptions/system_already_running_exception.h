#ifndef SYSTEM_ALREADY_RUNNING_EXCEPTION_H
#define SYSTEM_ALREADY_RUNNING_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class SystemAlreadyRunningException : public std::exception {
    private:
      std::string system_name;
    public:
      SystemAlreadyRunningException(std::string system_name) {
        this->system_name = system_name;
      }

      virtual const char* what() const throw() {
        return this->system_name.c_str(); + " is already running!";
      }
  };
}

#endif