#include <easylogging++.h>
#include <fstream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include "shader_manager.h"
#include "exceptions/invalid_filename_exception.h"
#include "exceptions/invalid_shader_name_exception.h"
#include "exceptions/shader_compilation_exception.h"

namespace Graphics {
  char const* ShaderManager::VERTEX_EXTENSION = ".vert";
  char const* ShaderManager::FRAGMENT_EXTENSION = ".frag";
  char const* ShaderManager::GEOMETRY_EXTENSION = ".geom";
  char const* ShaderManager::SHADER_DIRECTORY = "./shaders/";

  ShaderManager::ShaderManager() {
    
  }

  ShaderManager::~ShaderManager() {

  }

  const bool ShaderManager::loadShader(const std::string& name, const bool geometry_shader) {
    std::string geom_name = "";
    if(geometry_shader)
      geom_name = name + GEOMETRY_EXTENSION;

    return loadShader(name, name+VERTEX_EXTENSION, name+FRAGMENT_EXTENSION, geom_name);
  }

  const bool ShaderManager::loadShader(const std::string& name, const std::string& vertex_filename, const std::string& fragment_filename, const std::string& geometry_filename) {
    std::ifstream vertex_file;
    if(vertex_filename != "") {
      vertex_file.open((SHADER_DIRECTORY + vertex_filename).c_str(), std::ios_base::binary);
    }
    std::ifstream fragment_file;
    if(fragment_filename != "") 
      fragment_file.open((SHADER_DIRECTORY + fragment_filename).c_str(), std::ios_base::binary);
    std::ifstream geometry_file;
    if(geometry_filename != "") {
      geometry_file.open((SHADER_DIRECTORY + geometry_filename).c_str(), std::ios_base::binary);
    }

    unsigned int vertex_shader = 0;
    unsigned int fragment_shader = 0;
    unsigned int geometry_shader = 0;

    if(vertex_filename != "" && !vertex_file.is_open()) {
      throw Exceptions::InvalidFilenameException(vertex_filename);
    }
    else if(vertex_filename != "" && vertex_file.is_open()) {
      LOG(INFO)<<vertex_filename<<" shader file loaded.";
      vertex_shader = glCreateShader(GL_VERTEX_SHADER);
      const char *vc_str;
      std::string vertex_string;
      vertex_file.seekg(0, std::ios::end);   
      vertex_string.reserve(vertex_file.tellg());
      vertex_file.seekg(0, std::ios::beg);

      vertex_string.assign((std::istreambuf_iterator<char>(vertex_file)),
                            std::istreambuf_iterator<char>());
      vertex_file.close();

      vc_str = vertex_string.c_str();

      glShaderSource(vertex_shader, 1, &vc_str, new int[1] {(int)vertex_string.size()});
      glCompileShader(vertex_shader);
      
      if(!checkCompilation(vertex_shader)) {
        logShaderInfoLog(vertex_shader);
        throw Exceptions::ShaderCompilationException(vertex_filename);
      }
    }
    if(fragment_filename != "" && !fragment_file.is_open()) {
      throw Exceptions::InvalidFilenameException(fragment_filename);
    }
    else if(fragment_filename != "" && fragment_file.is_open()) {
      LOG(INFO)<<fragment_filename<<" shader file loaded.";
      fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
      const char *fc_str;
      std::string fragment_string;
      fragment_file.seekg(0, std::ios::end);   
      fragment_string.reserve(fragment_file.tellg());
      fragment_file.seekg(0, std::ios::beg);

      fragment_string.assign((std::istreambuf_iterator<char>(fragment_file)),
                              std::istreambuf_iterator<char>());
      fragment_file.close();

      fc_str = fragment_string.c_str();

      glShaderSource(fragment_shader, 1, &fc_str, new int[1] {(int)fragment_string.size()});
      glCompileShader(fragment_shader);
      if(!checkCompilation(fragment_shader)) {
        logShaderInfoLog(fragment_shader);
        throw Exceptions::ShaderCompilationException(fragment_filename);
      }
    }
    if(geometry_filename != "" && !geometry_file.is_open()) {
      throw Exceptions::InvalidFilenameException(geometry_filename);
    }
    else if(geometry_filename != "" && geometry_file.is_open()) {
      LOG(INFO)<<geometry_filename<<" shader file loaded.";
      geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
      const char *gc_str;
      std::string geometry_string;
      geometry_file.seekg(0, geometry_file.end);   
      geometry_string.reserve(geometry_file.tellg());
      geometry_file.seekg(0, geometry_file.beg);

      geometry_string.assign((std::istreambuf_iterator<char>(geometry_file)),
                              std::istreambuf_iterator<char>());
      geometry_file.close();

      gc_str = geometry_string.c_str();

      glShaderSource(geometry_shader, 1, &gc_str, new int[1] {(int)geometry_string.size()});
      glCompileShader(geometry_shader);
      if(!checkCompilation(geometry_shader)) {
        logShaderInfoLog(geometry_shader);
        throw Exceptions::ShaderCompilationException(geometry_filename);
      }
    }

    try {
      shaders_to_names[name] = std::make_shared<Shader>(vertex_shader, fragment_shader, geometry_shader);
    }
    catch(std::exception& e) {
      LOG(ERROR)<<e.what();
      return false;
    }

    return true;
  }

  const std::shared_ptr<Shader> ShaderManager::operator[](const std::string& name) const {
    return getShader(name);
  }

  const std::shared_ptr<Shader> ShaderManager::getShader(const std::string& name) const {
    if(shaders_to_names.count(name) == 0) {
      throw Exceptions::InvalidShaderNameException(name);
    } 
    return shaders_to_names.at(name);
  }

  const bool ShaderManager::checkCompilation(const unsigned int& shader_object) {
    int status = 0;
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &status);
    return (bool)status;
  }

  void ShaderManager::logShaderInfoLog(const unsigned int& shader_object) {
    int max_length = 0;
    glGetShaderiv(shader_object, GL_INFO_LOG_LENGTH, &max_length);
    std::vector<char> info_log(max_length);
    glGetShaderInfoLog(shader_object, max_length, &max_length, &info_log[0]);
    std::string output(info_log.begin(), info_log.end());
    LOG(INFO)<<output;
  }

}