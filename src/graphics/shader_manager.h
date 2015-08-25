#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H
#include <map>
#include <memory>
#include "graphics/shader.h"

namespace Graphics {
  class ShaderManager {
    private:
      std::map<std::string, std::shared_ptr<Shader>> shaders_to_names;
      const bool checkCompilation(const unsigned int& shader_object);
      void logShaderInfoLog(const unsigned int& shader_object);
    public:
      static char const* VERTEX_EXTENSION;
      static char const* FRAGMENT_EXTENSION;
      static char const* GEOMETRY_EXTENSION;
      static char const* SHADER_DIRECTORY;

      ShaderManager();
      ~ShaderManager();

      const bool loadShader(const std::string& name, const bool geometry_shader = false);
      const bool loadShader(const std::string& name, const std::string& vertex_filename, const std::string& fragment_filename, const std::string& geometry_filename);

      const std::shared_ptr<Shader> operator[](const std::string& name) const;
      const std::shared_ptr<Shader> getShader(const std::string& name) const;

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
