#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include <chaiscript/chaiscript.hpp>
#include "graphics/shader.h"
#include "graphics/uniform.h"
//= SCRIPTABLE

namespace Graphics {
  /**
   * @brief      Class for shader manager.
   */
  class ShaderManager {
    private:
      std::map<std::string, std::shared_ptr<Shader>> shaders_to_names;
      std::string shader_directory;
      bool checkCompilation(const unsigned int& shader_object);
      void logShaderInfoLog(const unsigned int& shader_object);
    public:
      /**
       * Vertex Shader Extension
       */
      static char const* VERTEX_EXTENSION;
      /**
       * Fragment Shader Extension
       */
      static char const* FRAGMENT_EXTENSION;
      /**
       * Geometry Shader Extension
       */
      static char const* GEOMETRY_EXTENSION;
      ShaderManager() = delete;

      /**
       * @brief      Shader Manager constructor.
       */
      ShaderManager(const std::string& shader_directory);
      /**
       * @brief      Destroys the object.
       */
      ~ShaderManager();
      //= BEGIN SCRIPTABLE

      /**
       * @brief      Loads a shader.
       *
       * @param[in]  name             The name
       * @param[in]  geometry_shader  The geometry shader
       *
       * @return     True if successful
       */
      bool loadShader(const std::string& name, const bool geometry_shader = false);
      /**
       * @brief      Loads a shader.
       *
       * @param[in]  name               The name
       * @param[in]  vertex_filename    The vertex filename
       * @param[in]  fragment_filename  The fragment filename
       * @param[in]  geometry_filename  The geometry filename
       *
       * @return     True if successful
       */
      bool loadShader(const std::string& name, const std::string& vertex_filename, const std::string& fragment_filename, const std::string& geometry_filename);

      /**
       * @brief      looks up shader by name
       *
       * @param[in]  name  The name
       *
       * @return     Shader with name
       */
      std::shared_ptr<Shader> operator[](const std::string& name) const;
      /**
       * @brief      Gets the shader.
       *
       * @param[in]  name  The name
       *
       * @return     The shader.
       */
      std::shared_ptr<Shader> getShader(const std::string& name) const;

      /**
       * @brief      Sets the uniform for all programs.
       *
       * @param[in]  u     The uniform to set
       */
      void setUniformForAllPrograms(const Uniform& u);
      //= END SCRIPTABLE

      /**
       * @brief      Sets the uniform for all programs.
       *
       * @param[in]  name  The name
       * @param[in]  data  The data
       *
       * @tparam     T     The data type for the uniform
       */
      template<class T>
      void setUniformForAllPrograms(const std::string& name, const T& data);

  };

  template<class T>
  void ShaderManager::setUniformForAllPrograms(const std::string& name, const T& data) {
    for(auto& i : shaders_to_names) {
      i.second->setUniform(name, data);
    }
  }
}

#endif
