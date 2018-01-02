#ifndef TEXTURE_NOT_LOADED_EXCEPTION_H
#define TEXTURE_NOT_LOADED_EXCEPTION_H

#include <exception>

namespace Exceptions {
  class TextureNotLoadedException : public std::exception {
    public:
      TextureNotLoadedException() {
      }

      virtual const char* what() const throw() {
        return "Texture that was bound is not loaded!";
      }
  };
}

#endif
