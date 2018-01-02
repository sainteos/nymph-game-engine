#ifndef SHADER_COMPILATION_EXCEPTION_H
#define SHADER_COMPILATION_EXCEPTION_H

#include <exception>
#include <string>

namespace Exceptions {
  class ShaderCompilationException : public std::exception {
    private:
      std::string shader_filename;
    public:
      ShaderCompilationException(const std::string& filename) {
        this->shader_filename = filename;
      }

      virtual const char* what() const throw() {
        return (std::string("Shader failed to compile: ") + this->shader_filename).c_str();
      }
  };
}

#endif
