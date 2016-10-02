#include <easylogging++.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "exceptions/invalid_vertex_shader_exception.h"
#include "exceptions/invalid_fragment_shader_exception.h"
#include "exceptions/invalid_geometry_shader_exception.h"
#include "exceptions/invalid_shader_program_exception.h"
#include "exceptions/invalid_uniform_name_exception.h"

namespace Graphics {
  Shader::Shader(const unsigned int vertex_program, const unsigned int fragment_program, const unsigned int geometry_program) {
    if(!glIsShader(vertex_program)) {
      throw Exceptions::InvalidVertexShaderException(vertex_program);
    }
    if(!glIsShader(fragment_program)) {
      throw Exceptions::InvalidFragmentShaderException(fragment_program);
    }
    if(geometry_program != 0 && !glIsShader(geometry_program)) {
      throw Exceptions::InvalidGeometryShaderException(geometry_program);
    }
    
    program_object = glCreateProgram();
    glAttachShader(program_object, vertex_program);
    glAttachShader(program_object, fragment_program);
    if(geometry_program != 0)
      glAttachShader(program_object, geometry_program);
    
    glLinkProgram(program_object);
    int is_linked = 0;
    glGetProgramiv(program_object, GL_LINK_STATUS, &is_linked);
    if(!is_linked)
    {
      int max_length = 0;
      glGetProgramiv(program_object, GL_INFO_LOG_LENGTH, &max_length);
      std::vector<char> info_log(max_length);
      glGetProgramInfoLog(program_object, max_length, &max_length, &info_log[0]);
      std::string output(info_log.begin(), info_log.end());
      LOG(ERROR)<<output;
      glDeleteProgram(program_object);

      throw Exceptions::InvalidShaderProgramException(program_object);
    }
    if(!glIsProgram(program_object)) {
      throw Exceptions::InvalidShaderProgramException(program_object);
    }

    int count = 0;
    glGetProgramiv(program_object, GL_ACTIVE_UNIFORMS, &count);
    for(int i = 0; i < count; i++) {
      int name_length;
      int arr_size;
      GLenum type;
      GLchar name[64];
      glGetActiveUniform(program_object, i, 64, &name_length, &arr_size, &type, name);
      int location = glGetUniformLocation(program_object, name);
      name_to_location[std::string(name)] = location;
      name_to_type[std::string(name)] = type;
      LOG(INFO)<<"Uniform found: "<<name<<" Location: "<<location;
    }
  }

  const unsigned int Shader::getHandle() const noexcept {
    return program_object;
  }

  void Shader::useProgram() const {
    if(!glIsProgram(program_object)) {
      throw Exceptions::InvalidShaderProgramException(program_object);
    }
    glUseProgram(program_object);
  }

  const std::vector<std::string> Shader::getUniformNames() const noexcept {
    std::vector<std::string> names;
    for(auto i : name_to_location) 
      names.push_back(i.first);
    return names;
  }

  template<>
  void Shader::setUniform<float>(const std::string& name, const float& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform1f(name_to_location[name], data);
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::vec2>(const std::string& name, const glm::vec2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform2fv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform3fv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::vec4>(const std::string& name, const glm::vec4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform4fv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }

  template<>
  void Shader::setUniform<int>(const std::string& name, const int& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform1i(name_to_location[name], data);
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::ivec2>(const std::string& name, const glm::ivec2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform2iv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::ivec3>(const std::string& name, const glm::ivec3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform3iv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::ivec4>(const std::string& name, const glm::ivec4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform4iv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }

  template<>
  void Shader::setUniform<unsigned int>(const std::string& name, const unsigned int& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform1ui(name_to_location[name], data);
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::uvec2>(const std::string& name, const glm::uvec2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform2uiv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::uvec3>(const std::string& name, const glm::uvec3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform3uiv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::uvec4>(const std::string& name, const glm::uvec4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform4uiv(name_to_location[name], 1, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }

  template<>
  void Shader::setUniform<bool>(const std::string& name, const bool& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform1i(name_to_location[name], data);
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::bvec2>(const std::string& name, const glm::bvec2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform2iv(name_to_location[name], 1, glm::value_ptr(glm::ivec2(data)));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::bvec3>(const std::string& name, const glm::bvec3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform3iv(name_to_location[name], 1, glm::value_ptr(glm::ivec3(data)));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::bvec4>(const std::string& name, const glm::bvec4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniform4iv(name_to_location[name], 1, glm::value_ptr(glm::ivec4(data)));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }

  template<>
  void Shader::setUniform<glm::mat2>(const std::string& name, const glm::mat2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix2fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat3>(const std::string& name, const glm::mat3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix3fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix4fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat2x3>(const std::string& name, const glm::mat2x3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix2x3fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat3x2>(const std::string& name, const glm::mat3x2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix3x2fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat2x4>(const std::string& name, const glm::mat2x4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix2x4fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat4x2>(const std::string& name, const glm::mat4x2& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix4x2fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat3x4>(const std::string& name, const glm::mat3x4& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix3x4fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  template<>
  void Shader::setUniform<glm::mat4x3>(const std::string& name, const glm::mat4x3& data) {
    int prev_bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prev_bound_program);
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    if(prev_bound_program != program_object)
      useProgram();
    glUniformMatrix4x3fv(name_to_location[name], 1, false, glm::value_ptr(data));
    if(prev_bound_program != program_object)
      glUseProgram(prev_bound_program);
  }
  
  void Shader::setUniform(const Uniform& uniform) {
    switch(uniform.getType()) {
      case Graphics::Uniform::UniformTypes::FLOAT:
        setUniform(uniform.getName(), uniform.getData<float>());
        break;
      case Graphics::Uniform::UniformTypes::VEC2:
        setUniform(uniform.getName(), uniform.getData<glm::vec2>());
        break;
      case Graphics::Uniform::UniformTypes::VEC3:
        setUniform(uniform.getName(), uniform.getData<glm::vec2>());
        break;
      case Graphics::Uniform::UniformTypes::VEC4:
        setUniform(uniform.getName(), uniform.getData<glm::vec4>());
        break;
      case Graphics::Uniform::UniformTypes::INT:
        setUniform(uniform.getName(), uniform.getData<int>());
        break;
      case Graphics::Uniform::UniformTypes::IVEC2:
        setUniform(uniform.getName(), uniform.getData<glm::ivec2>());
        break;
      case Graphics::Uniform::UniformTypes::IVEC3:
        setUniform(uniform.getName(), uniform.getData<glm::ivec3>());
        break;
      case Graphics::Uniform::UniformTypes::IVEC4:
        setUniform(uniform.getName(), uniform.getData<glm::ivec4>());
        break;
      case Graphics::Uniform::UniformTypes::UINT:
        setUniform(uniform.getName(), uniform.getData<unsigned int>());
        break;
      case Graphics::Uniform::UniformTypes::UVEC2:
        setUniform(uniform.getName(), uniform.getData<glm::uvec2>());
        break;
      case Graphics::Uniform::UniformTypes::UVEC3:
        setUniform(uniform.getName(), uniform.getData<glm::uvec3>());
        break;
      case Graphics::Uniform::UniformTypes::UVEC4:
        setUniform(uniform.getName(), uniform.getData<glm::uvec4>());
        break;
      case Graphics::Uniform::UniformTypes::BOOL:
        setUniform(uniform.getName(), uniform.getData<bool>());
        break;
      case Graphics::Uniform::UniformTypes::BVEC2:
        setUniform(uniform.getName(), uniform.getData<glm::bvec2>());
        break;
      case Graphics::Uniform::UniformTypes::BVEC3:
        setUniform(uniform.getName(), uniform.getData<glm::bvec3>());
        break;
      case Graphics::Uniform::UniformTypes::BVEC4:
        setUniform(uniform.getName(), uniform.getData<glm::bvec4>());
        break;
      case Graphics::Uniform::UniformTypes::MAT2:
        setUniform(uniform.getName(), uniform.getData<glm::mat2>());
        break;
      case Graphics::Uniform::UniformTypes::MAT3:
        setUniform(uniform.getName(), uniform.getData<glm::mat3>());
        break;
      case Graphics::Uniform::UniformTypes::MAT4:
        setUniform(uniform.getName(), uniform.getData<glm::mat4>());
        break;
      case Graphics::Uniform::UniformTypes::MAT23:
        setUniform(uniform.getName(), uniform.getData<glm::mat2x3>());
        break;
      case Graphics::Uniform::UniformTypes::MAT32:
        setUniform(uniform.getName(), uniform.getData<glm::mat3x2>());
        break;
      case Graphics::Uniform::UniformTypes::MAT24:
        setUniform(uniform.getName(), uniform.getData<glm::mat2x4>());
        break;
      case Graphics::Uniform::UniformTypes::MAT42:
        setUniform(uniform.getName(), uniform.getData<glm::mat4x2>());
        break;
      case Graphics::Uniform::UniformTypes::MAT34:
        setUniform(uniform.getName(), uniform.getData<glm::mat3x4>());
        break;
      case Graphics::Uniform::UniformTypes::MAT43:
        setUniform(uniform.getName(), uniform.getData<glm::mat4x3>());
        break;
      default:
        break;
    }
  }
}