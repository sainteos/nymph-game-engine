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
//= SCRIPTABLE

namespace Graphics {
  class Shader {
    private:
      unsigned int program_object;
      std::map<std::string, int> name_to_location;
      std::map<std::string, GLenum> name_to_type;
      std::string name;
    public:
      Shader() = delete;
      Shader(const unsigned int vertex_program, const unsigned int fragment_program, const unsigned int geometry_program = 0);

      const unsigned int getHandle() const noexcept;
      void useProgram() const;

      //= BEGIN SCRIPTABLE
      const std::vector<std::string> getUniformNames() const noexcept;
      void setUniform(const Uniform& uniform);
      void setName(const std::string& name);
      const std::string getName() const noexcept;
      const std::string to_string() const noexcept;
      //= END SCRIPTABLE
      template<class T>
      void setUniform(const std::string& name, const T& value);
  };
}

#endif
