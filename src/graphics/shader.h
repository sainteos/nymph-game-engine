#ifndef SHADER_H
#define SHADER_H
#include <map>
#include <string>
#include <OpenGL/gl3.h>

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

      template<class T>
      void setUniform(const std::string& name, const T& value);
  };
}

#endif
