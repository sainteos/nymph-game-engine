#ifndef INVALID_VERTEX_ARRAY_EXCEPTION_H
#define INVALID_VERTEX_ARRAY_EXCEPTION_H
#include <exception>

namespace Exceptions {
  class InvalidVertexArrayException : public std::exception {
    private:
      unsigned int vertex_array_binding;
    public:
      InvalidVertexArrayException(const unsigned int vertex_array_binding) {
        this->vertex_array_binding = vertex_array_binding;
      }

      virtual const char* what() const throw() {
        return "Invalid vertex array with binding: " + this->vertex_array_binding;
      }
  };
}
#endif
