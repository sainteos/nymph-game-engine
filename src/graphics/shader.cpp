#include <easylogging++.h>
#include <OpenGL/gl3.h>
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

  template<>
  void Shader::setUniform<float>(const std::string& name, const float& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform1f(name_to_location[name], data);
  }
  template<>
  void Shader::setUniform<glm::vec2>(const std::string& name, const glm::vec2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform2fv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform3fv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::vec4>(const std::string& name, const glm::vec4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform4fv(name_to_location[name], 1, glm::value_ptr(data));
  }

  template<>
  void Shader::setUniform<int>(const std::string& name, const int& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform1i(name_to_location[name], data);
  }
  template<>
  void Shader::setUniform<glm::ivec2>(const std::string& name, const glm::ivec2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform2iv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::ivec3>(const std::string& name, const glm::ivec3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform3iv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::ivec4>(const std::string& name, const glm::ivec4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform4iv(name_to_location[name], 1, glm::value_ptr(data));
  }

  template<>
  void Shader::setUniform<unsigned int>(const std::string& name, const unsigned int& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform1ui(name_to_location[name], data);
  }
  template<>
  void Shader::setUniform<glm::uvec2>(const std::string& name, const glm::uvec2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform2uiv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::uvec3>(const std::string& name, const glm::uvec3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform3uiv(name_to_location[name], 1, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::uvec4>(const std::string& name, const glm::uvec4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform4uiv(name_to_location[name], 1, glm::value_ptr(data));
  }

  template<>
  void Shader::setUniform<bool>(const std::string& name, const bool& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform1i(name_to_location[name], data);
  }
  template<>
  void Shader::setUniform<glm::bvec2>(const std::string& name, const glm::bvec2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform2iv(name_to_location[name], 1, glm::value_ptr(glm::ivec2(data)));
  }
  template<>
  void Shader::setUniform<glm::bvec3>(const std::string& name, const glm::bvec3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform3iv(name_to_location[name], 1, glm::value_ptr(glm::ivec3(data)));
  }
  template<>
  void Shader::setUniform<glm::bvec4>(const std::string& name, const glm::bvec4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniform4iv(name_to_location[name], 1, glm::value_ptr(glm::ivec4(data)));
  }

  template<>
  void Shader::setUniform<glm::mat2>(const std::string& name, const glm::mat2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix2fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat3>(const std::string& name, const glm::mat3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix3fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix4fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat2x3>(const std::string& name, const glm::mat2x3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix2x3fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat3x2>(const std::string& name, const glm::mat3x2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix3x2fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat2x4>(const std::string& name, const glm::mat2x4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix2x4fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat4x2>(const std::string& name, const glm::mat4x2& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix4x2fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat3x4>(const std::string& name, const glm::mat3x4& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix3x4fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
  template<>
  void Shader::setUniform<glm::mat4x3>(const std::string& name, const glm::mat4x3& data) {
    if(name_to_location.count(name) == 0) {
      throw Exceptions::InvalidUniformNameException(name);
    }
    glUniformMatrix4x3fv(name_to_location[name], 1, false, glm::value_ptr(data));
  }
}