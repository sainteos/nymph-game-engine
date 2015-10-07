#include "graphics/uniform.h"
#include <easylogging++.h>

namespace Graphics {
  Uniform::Uniform() : name("") {
  }

  template<>
  void Uniform::setData<float>(const std::string& name, const float& data) {
    uniform_type = UniformTypes::FLOAT;
    float_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::vec2>(const std::string& name, const glm::vec2& data) {
    uniform_type = UniformTypes::VEC2;
    vec2_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::vec3>(const std::string& name, const glm::vec3& data) {
    uniform_type = UniformTypes::VEC3;
    vec3_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::vec4>(const std::string& name, const glm::vec4& data) {
    uniform_type = UniformTypes::VEC4;
    vec4_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<int>(const std::string& name, const int& data) {
    uniform_type = UniformTypes::INT;
    int_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::ivec2>(const std::string& name, const glm::ivec2& data) {
    uniform_type = UniformTypes::IVEC2;
    ivec2_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::ivec3>(const std::string& name, const glm::ivec3& data) {
    uniform_type = UniformTypes::VEC3;
    ivec3_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::ivec4>(const std::string& name, const glm::ivec4& data) {
    uniform_type = UniformTypes::IVEC4;
    ivec4_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<unsigned int>(const std::string& name, const unsigned int& data) {
    uniform_type = UniformTypes::UINT;
    uint_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::uvec2>(const std::string& name, const glm::uvec2& data) {
    uniform_type = UniformTypes::UVEC2;
    uvec2_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::uvec3>(const std::string& name, const glm::uvec3& data) {
    uniform_type = UniformTypes::UVEC3;
    uvec3_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::uvec4>(const std::string& name, const glm::uvec4& data) {
    uniform_type = UniformTypes::UVEC4;
    uvec4_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<bool>(const std::string& name, const bool& data) {
    uniform_type = UniformTypes::BOOL;
    bool_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::bvec2>(const std::string& name, const glm::bvec2& data) {
    uniform_type = UniformTypes::VEC2;
    bvec2_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::bvec3>(const std::string& name, const glm::bvec3& data) {
    uniform_type = UniformTypes::VEC3;
    bvec3_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::bvec4>(const std::string& name, const glm::bvec4& data) {
    uniform_type = UniformTypes::BVEC4;
    bvec4_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat2>(const std::string& name, const glm::mat2& data) {
    uniform_type = UniformTypes::MAT2;
    mat2_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat3>(const std::string& name, const glm::mat3& data) {
    uniform_type = UniformTypes::MAT3;
    mat3_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat4>(const std::string& name, const glm::mat4& data) {
    uniform_type = UniformTypes::MAT4;
    mat4_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat2x3>(const std::string& name, const glm::mat2x3& data) {
    uniform_type = UniformTypes::MAT23;
    mat23_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat3x2>(const std::string& name, const glm::mat3x2& data) {
    uniform_type = UniformTypes::MAT32;
    mat32_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat2x4>(const std::string& name, const glm::mat2x4& data) {
    uniform_type = UniformTypes::MAT24;
    mat24_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat4x2>(const std::string& name, const glm::mat4x2& data) {
    uniform_type = UniformTypes::MAT42;
    mat42_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat3x4>(const std::string& name, const glm::mat3x4& data) {
    uniform_type = UniformTypes::MAT34;
    mat34_data = data;
    this->name = name;
  }

  template<>
  void Uniform::setData<glm::mat4x3>(const std::string& name, const glm::mat4x3& data) {
    uniform_type = UniformTypes::MAT43;
    mat43_data = data;
    this->name = name;
  }

  const Uniform::UniformTypes Uniform::getType() const noexcept {
    return uniform_type;
  }

  const std::string Uniform::getName() const noexcept {
    return name;
  }

  template<>
  const float Uniform::getData<float>() const noexcept {
    return float_data;
  }

  template<>
  const glm::vec2 Uniform::getData<glm::vec2>() const noexcept {
    return vec2_data;
  }

  template<>
  const glm::vec3 Uniform::getData<glm::vec3>() const noexcept {
    return vec3_data;
  }

  template<>
  const glm::vec4 Uniform::getData<glm::vec4>() const noexcept {
    return vec4_data;
  }

  template<>
  const int Uniform::getData<int>() const noexcept {
    return int_data;
  }

  template<>
  const glm::ivec2 Uniform::getData<glm::ivec2>() const noexcept {
    return ivec2_data;
  }

  template<>
  const glm::ivec3 Uniform::getData<glm::ivec3>() const noexcept {
    return ivec3_data;
  }

  template<>
  const glm::ivec4 Uniform::getData<glm::ivec4>() const noexcept {
    return ivec4_data;
  }

  template<>
  const unsigned int Uniform::getData<unsigned int>() const noexcept {
    return uint_data;
  }

  template<>
  const glm::uvec2 Uniform::getData<glm::uvec2>() const noexcept {
    return uvec2_data;
  }

  template<>
  const glm::uvec3 Uniform::getData<glm::uvec3>() const noexcept {
    return uvec3_data;
  }

  template<>
  const glm::uvec4 Uniform::getData<glm::uvec4>() const noexcept {
    return uvec4_data;
  }

  template<>
  const bool Uniform::getData<bool>() const noexcept {
    return bool_data;
  }

  template<>
  const glm::bvec2 Uniform::getData<glm::bvec2>() const noexcept {
    return bvec2_data;
  }

  template<>
  const glm::bvec3 Uniform::getData<glm::bvec3>() const noexcept {
    return bvec3_data;
  }

  template<>
  const glm::bvec4 Uniform::getData<glm::bvec4>() const noexcept {
    return bvec4_data;
  }

  template<>
  const glm::mat2 Uniform::getData<glm::mat2>() const noexcept {
    return mat2_data;
  }

  template<>
  const glm::mat3 Uniform::getData<glm::mat3>() const noexcept {
    return mat3_data;
  }

  template<>
  const glm::mat4 Uniform::getData<glm::mat4>() const noexcept {
    return mat4_data;
  }

  template<>
  const glm::mat2x3 Uniform::getData<glm::mat2x3>() const noexcept {
    return mat23_data;
  }

  template<>
  const glm::mat3x2 Uniform::getData<glm::mat3x2>() const noexcept {
    return mat32_data;
  }

  template<>
  const glm::mat2x4 Uniform::getData<glm::mat2x4>() const noexcept {
    return mat24_data;
  }

  template<>
  const glm::mat4x2 Uniform::getData<glm::mat4x2>() const noexcept {
    return mat42_data;
  }

  template<>
  const glm::mat3x4 Uniform::getData<glm::mat3x4>() const noexcept {
    return mat34_data;
  }

  template<>
  const glm::mat4x3 Uniform::getData<glm::mat4x3>() const noexcept {
    return mat43_data;
  }

  const bool Uniform::operator<(const Uniform& right) const noexcept {
    return name < right.name;
  }
}