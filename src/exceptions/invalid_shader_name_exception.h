#ifndef INVALID_SHADER_NAME_EXCEPTION_H
#define INVALID_SHADER_NAME_EXCEPTION_H

namespace Exceptions {
  class InvalidShaderNameException : public std::exception {
    private:
      std::string name;
    public:
      InvalidShaderNameException(const std::string& name) {
        this->name = name;
      }

      virtual const char* what() const throw() {
        return (std::string("Invalid shader name given: ") + this->name).c_str();
      }
  };
}

#endif
