#ifndef SHADER_H
#define SHADER_H
#include <map>
#include <string>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif
#include "uniform.h"
//= SCRIPTABLE

namespace Graphics {
  /**
   * @brief      Class for shader.
   */
  class Shader {
    private:
      unsigned int program_object;
      std::map<std::string, int> name_to_location;
      std::map<std::string, GLenum> name_to_type;
      std::string name;
    public:
      Shader() = delete;
      /**
       * @brief      Shader constructor
       *
       * @param[in]  vertex_program    The vertex program
       * @param[in]  fragment_program  The fragment program
       * @param[in]  geometry_program  The geometry program
       */
      Shader(const unsigned int vertex_program, const unsigned int fragment_program, const unsigned int geometry_program = 0);

      /**
       * @brief      Gets the shader's opengl handle.
       *
       * @return     The handle.
       */
      unsigned int getHandle() const noexcept;
      /**
       * @brief      Tell open gl to use this shader
       */
      void useProgram() const;

      //= BEGIN SCRIPTABLE

      /**
       * @brief      Gets the uniform names.
       *
       * @return     The uniform names.
       */
      std::vector<std::string> getUniformNames() const noexcept;
      /**
       * @brief      Sets the uniform.
       *
       * @param[in]  uniform  The uniform
       */
      void setUniform(const Uniform& uniform);
      /**
       * @brief      Sets the name.
       *
       * @param[in]  name  The name
       */
      void setName(const std::string& name);
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      std::string getName() const noexcept;
      /**
       * @brief      Returns a string representation of the object.
       *
       * @return     String representation of the object.
       */
      std::string to_string() const noexcept;
      //= END SCRIPTABLE

      /**
       * @brief      Sets the uniform.
       *
       * @param[in]  name   The name
       * @param[in]  value  The value
       *
       * @tparam     T      Type the uniform should hold
       */
      template<class T>
      void setUniform(const std::string& name, const T& value);
  };
}

#endif
