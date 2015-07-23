#ifndef INVALID_SHADER_OBJECT_EXCEPTION_H
#define INVALID_SHADER_OBJECT_EXCEPTION_H
#include <exception>

namespace Exceptions {
  class InvalidShaderObjectException : public std::exception {
    private:
      unsigned int shader_object_binding;
    public:
      InvalidShaderObjectException(const unsigned int shader_object_binding) {
        this->shader_object_binding = shader_object_binding;
      }

      virtual const char* what() const throw() {
        return "Invalid shader object with binding: " + this->shader_object_binding;
      }
  };
}
#endif
