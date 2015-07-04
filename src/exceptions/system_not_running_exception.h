#ifndef SYSTEM_NOT_RUNNING_EXCEPTION_H
#define SYSTEM_NOT_RUNNING_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class SystemNotRunningException : public std::exception {
    private:
      std::string system_name;
    public:
      SystemNotRunningException(std::string system_name) {
        this->system_name = system_name;
      }

      virtual const char* what() const throw() {
        return this->system_name.c_str(); + " is not running!";
      }
  };
}

#endif