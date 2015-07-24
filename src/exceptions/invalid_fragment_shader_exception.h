#ifndef INVALID_FRAGMENT_SHADER_EXCEPTION_H
#define INVALID_FRAGMENT_SHADER_EXCEPTION_H
#include <exception>

namespace Exceptions {
  class InvalidFragmentShaderException : public std::exception {
    private:
      unsigned int fragment_shader_binding;
    public:
      InvalidFragmentShaderException(const unsigned int fragment_shader_binding) {
        this->fragment_shader_binding = fragment_shader_binding;
      }

      virtual const char* what() const throw() {
        return "Invalid fragment shader object with binding: " + this->fragment_shader_binding;
      }
  };
}
#endif
