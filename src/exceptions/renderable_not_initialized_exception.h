#ifndef RENDERABLE_NOT_INITIALIZED_EXCEPTION_H
#define RENDERABLE_NOT_INITIALIZED_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class RenderableNotInitializedException : public std::exception {
    public:
      RenderableNotInitializedException() {
      }

      virtual const char* what() const throw() {
        return "Renderable has not been initialized yet!";
      }
  };
}

#endif