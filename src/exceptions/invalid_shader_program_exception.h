#ifndef INVALID_SHADER_PROGRAM_EXCEPTION_H
#define INVALID_SHADER_PROGRAM_EXCEPTION_H
#include <exception>
#include <sstream>

namespace Exceptions {
  class InvalidShaderProgramException : public std::exception {
    private:
      unsigned int shader_program;
    public:
      InvalidShaderProgramException(const unsigned int shader_program) {
        this->shader_program = shader_program;
      }

      virtual const char* what() const throw() {
        std::stringstream s;
        s << "Invalid shader program with binding: " << this->shader_program;
        return s.str().c_str();
      }
  };
}
#endif
