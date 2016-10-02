#ifndef SHADER_H
#define SHADER_H
#include <map>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include "uniform.h"

namespace Graphics {
  class Shader {
    private:
      unsigned int program_object;
      std::map<std::string, int> name_to_location;
      std::map<std::string, GLenum> name_to_type;
    public:
      Shader() = delete;
      Shader(const unsigned int vertex_program, const unsigned int fragment_program, const unsigned int geometry_program = 0);

      const unsigned int getHandle() const noexcept;
      void useProgram() const;

      const std::vector<std::string> getUniformNames() const noexcept;
      
      template<class T>
      void setUniform(const std::string& name, const T& value);

      void setUniform(const Uniform& uniform);
  };
}

#endif
