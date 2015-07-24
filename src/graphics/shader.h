#ifndef SHADER_H
#define SHADER_H

namespace Graphics {
  class Shader {
    private:
      unsigned int program_object;
    public:
      Shader() = delete;
      Shader(const unsigned int vertex_program, const unsigned int fragment_program);

      const unsigned int getHandle() const noexcept;
  };
}

#endif
