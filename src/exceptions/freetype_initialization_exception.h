#ifndef FREE_TYPE_INITIALIZATION_EXCEPTION_H
#define FREE_TYPE_INITIALIZATION_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class FreeTypeInitializationException : public std::exception {
    public:
      FreeTypeInitializationException() {
      }

      virtual const char* what() const throw() {
        return "Freetype could not be initialized!";
      }
  };
}

#endif
