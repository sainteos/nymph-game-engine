#ifndef INVALID_GEOMETRY_SHADER_EXCEPTION_H
#define INVALID_GEOMETRY_SHADER_EXCEPTION_H
#include <exception>

namespace Exceptions {
  class InvalidGeometryShaderException : public std::exception {
    private:
      unsigned int geometry_shader_binding;
    public:
      InvalidGeometryShaderException(const unsigned int geometry_shader_binding) {
        this->geometry_shader_binding = geometry_shader_binding;
      }

      virtual const char* what() const throw() {
        return "Invalid geometry shader object with binding: " + this->geometry_shader_binding;
      }
  };
}
#endif
