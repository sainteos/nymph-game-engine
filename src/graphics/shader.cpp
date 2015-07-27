#include <OpenGL/gl3.h>
#include "shader.h"
#include "exceptions/invalid_vertex_shader_exception.h"
#include "exceptions/invalid_fragment_shader_exception.h"
#include "exceptions/invalid_shader_program_exception.h"

namespace Graphics {
  Shader::Shader(const unsigned int vertex_program, const unsigned int fragment_program) {
    if(!glIsShader(vertex_program)) {
      throw Exceptions::InvalidVertexShaderException(vertex_program);
    }
    if(!glIsShader(fragment_program)) {
      throw Exceptions::InvalidFragmentShaderException(fragment_program);
    }
    
    program_object = glCreateProgram();
    glAttachShader(program_object, vertex_program);
    glAttachShader(program_object, fragment_program);
    glLinkProgram(program_object);
    if(!glIsProgram(program_object)) {
      throw Exceptions::InvalidShaderProgramException(program_object);
    }
  }

  const unsigned int Shader::getHandle() const noexcept {
    return program_object;
  }

  void Shader::useProgram() const {
    if(!glIsProgram(program_object)) {
      throw Exceptions::InvalidShaderProgramException(program_object);
    }

    //bind uniforms
    //bind uniform buffers
    glUseProgram(program_object);
  }
}