#ifndef INVALID_VERTEX_ARRAY_EXCEPTION_H
#define INVALID_VERTEX_ARRAY_EXCEPTION_H
#include <exception>
#include <sstream>

namespace Exceptions {
  class InvalidVertexArrayException : public std::exception {
    private:
      unsigned int vertex_array_binding;
    public:
      InvalidVertexArrayException(const unsigned int vertex_array_binding) {
        this->vertex_array_binding = vertex_array_binding;
      }

      virtual const char* what() const throw() {
        std::stringstream s;
        s << "Invalid vertex array with binding: " << this->vertex_array_binding;
        return s.str().c_str();
      }
  };
}
#endif
