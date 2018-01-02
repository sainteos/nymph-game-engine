#ifndef CONFIG_NOT_LOADED_EXCEPTION_H
#define CONFIG_NOT_LOADED_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class ConfigNotLoadedException : public std::exception {
    public:
      ConfigNotLoadedException() {
      }

      virtual const char* what() const throw() {
        return "No config has been loaded!";
      }
  };
}

#endif
