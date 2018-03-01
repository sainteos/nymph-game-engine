#ifndef SYSTEM_NOT_INITIALIZED_EXCEPTION_H
#define SYSTEM_NOT_INITIALIZED_EXCEPTION_H

#include <string>
#include <exception>

namespace Exceptions {
  class SystemNotInitializedException : public std::exception {
    private:
      std::string system_name;
    public:
      SystemNotInitializedException(std::string system_name) {
        this->system_name = system_name;
      }

      virtual const char* what() const throw() {
        return (this->system_name + std::string(" hasn't been initialized yet!")).c_str();
      }
  };
}

#endif
