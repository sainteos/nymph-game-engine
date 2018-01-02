#include "graphics/uniform.h"
#include <easylogging++.h>

namespace Graphics {
  Uniform::Uniform() : name(""), dirty(false) {
  }

  template<>
  void Uniform::setData<float>(const std::string& name, const float& data) {
    if(name == this->name && data == float_data)
      return;
    uniform_type = UniformTypes::FLOAT;
    float_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::vec2>(const std::string& name, const glm::vec2& data) {
    if(name == this->name && data == vec2_data)
      return;
    uniform_type = UniformTypes::VEC2;
    vec2_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::vec3>(const std::string& name, const glm::vec3& data) {
    if(name == this->name && data == vec3_data)
      return;
    uniform_type = UniformTypes::VEC3;
    vec3_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::vec4>(const std::string& name, const glm::vec4& data) {
    if(name == this->name && data == vec4_data)
      return;
    uniform_type = UniformTypes::VEC4;
    vec4_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<int>(const std::string& name, const int& data) {
    if(name == this->name && data == int_data)
      return;
    uniform_type = UniformTypes::INT;
    int_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::ivec2>(const std::string& name, const glm::ivec2& data) {
    if(name == this->name && data == ivec2_data)
      return;
    uniform_type = UniformTypes::IVEC2;
    ivec2_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::ivec3>(const std::string& name, const glm::ivec3& data) {
    if(name == this->name && data == ivec3_data)
      return;
    uniform_type = UniformTypes::IVEC3;
    ivec3_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::ivec4>(const std::string& name, const glm::ivec4& data) {
    if(name == this->name && data == ivec4_data)
      return;
    uniform_type = UniformTypes::IVEC4;
    ivec4_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<unsigned int>(const std::string& name, const unsigned int& data) {
    if(name == this->name && data == uint_data)
      return;
    uniform_type = UniformTypes::UINT;
    uint_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::uvec2>(const std::string& name, const glm::uvec2& data) {
    if(name == this->name && data == uvec2_data)
      return;
    uniform_type = UniformTypes::UVEC2;
    uvec2_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::uvec3>(const std::string& name, const glm::uvec3& data) {
    if(name == this->name && data == uvec3_data)
      return;
    uniform_type = UniformTypes::UVEC3;
    uvec3_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::uvec4>(const std::string& name, const glm::uvec4& data) {
    if(name == this->name && data == uvec4_data)
      return;
    uniform_type = UniformTypes::UVEC4;
    uvec4_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<bool>(const std::string& name, const bool& data) {
    if(name == this->name && data == bool_data)
      return;
    uniform_type = UniformTypes::BOOL;
    bool_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::bvec2>(const std::string& name, const glm::bvec2& data) {
    if(name == this->name && data == bvec2_data)
      return;
    uniform_type = UniformTypes::VEC2;
    bvec2_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::bvec3>(const std::string& name, const glm::bvec3& data) {
    if(name == this->name && data == bvec3_data)
      return;
    uniform_type = UniformTypes::VEC3;
    bvec3_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::bvec4>(const std::string& name, const glm::bvec4& data) {
    if(name == this->name && data == bvec4_data)
      return;
    uniform_type = UniformTypes::BVEC4;
    bvec4_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat2>(const std::string& name, const glm::mat2& data) {
    if(name == this->name && data == mat2_data)
      return;
    uniform_type = UniformTypes::MAT2;
    mat2_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat3>(const std::string& name, const glm::mat3& data) {
    if(name == this->name && data == mat3_data)
      return;
    uniform_type = UniformTypes::MAT3;
    mat3_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat4>(const std::string& name, const glm::mat4& data) {
    if(name == this->name && data == mat4_data)
      return;
    uniform_type = UniformTypes::MAT4;
    mat4_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat2x3>(const std::string& name, const glm::mat2x3& data) {
    if(name == this->name && data == mat23_data)
      return;
    uniform_type = UniformTypes::MAT23;
    mat23_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat3x2>(const std::string& name, const glm::mat3x2& data) {
    if(name == this->name && data == mat32_data)
      return;
    uniform_type = UniformTypes::MAT32;
    mat32_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat2x4>(const std::string& name, const glm::mat2x4& data) {
    if(name == this->name && data == mat24_data)
      return;
    uniform_type = UniformTypes::MAT24;
    mat24_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat4x2>(const std::string& name, const glm::mat4x2& data) {
    if(name == this->name && data == mat42_data)
      return;
    uniform_type = UniformTypes::MAT42;
    mat42_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat3x4>(const std::string& name, const glm::mat3x4& data) {
    if(name == this->name && data == mat34_data)
      return;
    uniform_type = UniformTypes::MAT34;
    mat34_data = data;
    this->name = name;
    dirty = true;
  }

  template<>
  void Uniform::setData<glm::mat4x3>(const std::string& name, const glm::mat4x3& data) {
    if(name == this->name && data == mat43_data)
      return;
    uniform_type = UniformTypes::MAT43;
    mat43_data = data;
    this->name = name;
    dirty = true;
  }

  Uniform::UniformTypes Uniform::getType() const noexcept {
    return uniform_type;
  }

  std::string Uniform::getName() const noexcept {
    return name;
  }

  bool Uniform::isDirty() const noexcept {
    return dirty;
  }

  void Uniform::clean() noexcept {
    dirty = false;
  }

  template<>
  float Uniform::getData<float>() const noexcept {
    return float_data;
  }

  template<>
  glm::vec2 Uniform::getData<glm::vec2>() const noexcept {
    return vec2_data;
  }

  template<>
  glm::vec3 Uniform::getData<glm::vec3>() const noexcept {
    return vec3_data;
  }

  template<>
  glm::vec4 Uniform::getData<glm::vec4>() const noexcept {
    return vec4_data;
  }

  template<>
  int Uniform::getData<int>() const noexcept {
    return int_data;
  }

  template<>
  glm::ivec2 Uniform::getData<glm::ivec2>() const noexcept {
    return ivec2_data;
  }

  template<>
  glm::ivec3 Uniform::getData<glm::ivec3>() const noexcept {
    return ivec3_data;
  }

  template<>
  glm::ivec4 Uniform::getData<glm::ivec4>() const noexcept {
    return ivec4_data;
  }

  template<>
  unsigned int Uniform::getData<unsigned int>() const noexcept {
    return uint_data;
  }

  template<>
  glm::uvec2 Uniform::getData<glm::uvec2>() const noexcept {
    return uvec2_data;
  }

  template<>
  glm::uvec3 Uniform::getData<glm::uvec3>() const noexcept {
    return uvec3_data;
  }

  template<>
  glm::uvec4 Uniform::getData<glm::uvec4>() const noexcept {
    return uvec4_data;
  }

  template<>
  bool Uniform::getData<bool>() const noexcept {
    return bool_data;
  }

  template<>
  glm::bvec2 Uniform::getData<glm::bvec2>() const noexcept {
    return bvec2_data;
  }

  template<>
  glm::bvec3 Uniform::getData<glm::bvec3>() const noexcept {
    return bvec3_data;
  }

  template<>
  glm::bvec4 Uniform::getData<glm::bvec4>() const noexcept {
    return bvec4_data;
  }

  template<>
  glm::mat2 Uniform::getData<glm::mat2>() const noexcept {
    return mat2_data;
  }

  template<>
  glm::mat3 Uniform::getData<glm::mat3>() const noexcept {
    return mat3_data;
  }

  template<>
  glm::mat4 Uniform::getData<glm::mat4>() const noexcept {
    return mat4_data;
  }

  template<>
  glm::mat2x3 Uniform::getData<glm::mat2x3>() const noexcept {
    return mat23_data;
  }

  template<>
  glm::mat3x2 Uniform::getData<glm::mat3x2>() const noexcept {
    return mat32_data;
  }

  template<>
  glm::mat2x4 Uniform::getData<glm::mat2x4>() const noexcept {
    return mat24_data;
  }

  template<>
  glm::mat4x2 Uniform::getData<glm::mat4x2>() const noexcept {
    return mat42_data;
  }

  template<>
  glm::mat3x4 Uniform::getData<glm::mat3x4>() const noexcept {
    return mat34_data;
  }

  template<>
  glm::mat4x3 Uniform::getData<glm::mat4x3>() const noexcept {
    return mat43_data;
  }

  bool Uniform::operator<(const Uniform& right) const noexcept {
    return name < right.name;
  }

  bool Uniform::operator!=(const Uniform& right) const noexcept {
    return !(*this == right);
  }

  bool Uniform::operator==(const Uniform& right) const noexcept {
    bool ret = false;
    ret &= this->name == right.name;
    ret &= this->uniform_type == right.uniform_type;
    switch(this->uniform_type) {
      case UniformTypes::FLOAT:
        ret &= this->float_data == right.float_data;
        break;
      case UniformTypes::VEC2:
        ret &= this->vec2_data == right.vec2_data;
        break;
      case UniformTypes::VEC3:
        ret &= this->vec3_data == right.vec3_data;
        break;
      case UniformTypes::VEC4:
        ret &= this->vec4_data == right.vec4_data;
        break;
      case UniformTypes::INT:
        ret &= this->int_data == right.int_data;
        break;
      case UniformTypes::IVEC2:
        ret &= this->ivec2_data == right.ivec2_data;
        break;
      case UniformTypes::IVEC3:
        ret &= this->ivec3_data == right.ivec3_data;
        break;
      case UniformTypes::IVEC4:
        ret &= this->ivec4_data == right.ivec4_data;
        break;
      case UniformTypes::UINT:
        ret &= this->uint_data == right.uint_data;
        break;
      case UniformTypes::UVEC2:
        ret &= this->uvec2_data == right.uvec2_data;
        break;
      case UniformTypes::UVEC3:
        ret &= this->uvec3_data == right.uvec3_data;
        break;
      case UniformTypes::UVEC4:
        ret &= this->uvec4_data == right.uvec4_data;
        break;
      case UniformTypes::BOOL:
        ret &= this->bool_data == right.bool_data;
        break;
      case UniformTypes::BVEC2:
        ret &= this->bvec2_data == right.bvec2_data;
        break;
      case UniformTypes::BVEC3:
        ret &= this->bvec3_data == right.bvec3_data;
        break;
      case UniformTypes::BVEC4:
        ret &= this->bvec4_data == right.bvec4_data;
        break;
      case UniformTypes::MAT2:
        ret &= this->mat2_data == right.mat2_data;
        break;
      case UniformTypes::MAT3:
        ret &= this->mat3_data == right.mat3_data;
        break;
      case UniformTypes::MAT4:
        ret &= this->mat4_data == right.mat4_data;
        break;
      case UniformTypes::MAT23:
        ret &= this->mat23_data == right.mat23_data;
        break;
      case UniformTypes::MAT32:
        ret &= this->mat32_data == right.mat32_data;
        break;
      case UniformTypes::MAT24:
        ret &= this->mat24_data == right.mat24_data;
        break;
      case UniformTypes::MAT42:
        ret &= this->mat42_data == right.mat42_data;
        break;
      case UniformTypes::MAT34:
        ret &= this->mat34_data == right.mat34_data;
        break;
      case UniformTypes::MAT43:
        ret &= this->mat43_data == right.mat43_data;
        break;
    }

    return ret;
  }

  std::string Uniform::to_string() const noexcept {
    std::stringstream str;
    str <<"Uniform name: "<<getName()<<" dirty: "<<isDirty();
    return str.str();
  }
}
