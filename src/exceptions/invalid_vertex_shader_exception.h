#ifndef INVALID_VERTEX_SHADER_EXCEPTION_H
#define INVALID_VERTEX_SHADER_EXCEPTION_H
#include <exception>

namespace Exceptions {
  class InvalidVertexShaderException : public std::exception {
    private:
      unsigned int vertex_shader_binding;
    public:
      InvalidVertexShaderException(const unsigned int vertex_shader_binding) {
        this->vertex_shader_binding = vertex_shader_binding;
      }

      virtual const char* what() const throw() {
        return "Invalid vertex shader object with binding: " + this->vertex_shader_binding;
      }
  };
}
#endif
