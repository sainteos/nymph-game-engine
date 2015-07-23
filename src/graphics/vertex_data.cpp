#include <easylogging++.h>
#include <exception>
#include "vertex_data.h"

namespace Graphics {
  VertexData::VertexData(GLenum primitive_type) : index_count(0), vertex_count(0), primitive_type(primitive_type) {

  }
  VertexData::~VertexData() {
  }
  //initialize the datawidth
  const std::map<Graphics::VertexData::DATA_TYPE, unsigned char> VertexData::DataWidth = std::map<Graphics::VertexData::DATA_TYPE, unsigned char>  {
        {Graphics::VertexData::DATA_TYPE::GEOMETRY, 3},
        {Graphics::VertexData::DATA_TYPE::TEX_COORDS, 2},
        {Graphics::VertexData::DATA_TYPE::NORMAL_COORDS, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED1, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED2, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED3, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED4, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED5, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED6, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED7, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED8, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED9, 3},
        {Graphics::VertexData::DATA_TYPE::ONE_WIDE, 1},
        {Graphics::VertexData::DATA_TYPE::TWO_WIDE, 2},
        {Graphics::VertexData::DATA_TYPE::THREE_WIDE, 3},
        {Graphics::VertexData::DATA_TYPE::FOUR_WIDE, 4}
  };

  void VertexData::checkDivisibility(const unsigned int size) {
    if(divisibility.at(primitive_type) != ANY && size%divisibility.at(primitive_type) != 0)
      throw std::length_error("Passed data doesn't comform to vertex count divisibility spec!");
  }

  void VertexData::checkMinimum(const unsigned int size) {
    if(size < minimum.at(primitive_type)) 
      throw std::length_error("Passed data doesn't comform to vertex count minimum spec!");
  }

  void VertexData::clearDataType(const DATA_TYPE& type) {
    if(float_vector1s.count(type) > 0)
      float_vector1s.erase(type);
    if(float_vector2s.count(type) > 0)
      float_vector2s.erase(type);
    if(float_vector3s.count(type) > 0)
      float_vector3s.erase(type);
    if(float_vector4s.count(type) > 0)
      float_vector4s.erase(type);
    if(double_vector1s.count(type) > 0)
      double_vector1s.erase(type);
    if(double_vector2s.count(type) > 0)
      double_vector2s.erase(type);
    if(double_vector3s.count(type) > 0)
      double_vector3s.erase(type);
    if(double_vector4s.count(type) > 0)
      double_vector4s.erase(type);
    if(int_vector1s.count(type) > 0)
      int_vector1s.erase(type);
    if(int_vector2s.count(type) > 0)
      int_vector2s.erase(type);
    if(int_vector3s.count(type) > 0)
      int_vector3s.erase(type);
    if(int_vector4s.count(type) > 0)
      int_vector4s.erase(type);
    if(unsigned_int_vector1s.count(type) > 0)
      unsigned_int_vector1s.erase(type);
    if(unsigned_int_vector2s.count(type) > 0)
      unsigned_int_vector2s.erase(type);
    if(unsigned_int_vector3s.count(type) > 0)
      unsigned_int_vector3s.erase(type);
    if(unsigned_int_vector4s.count(type) > 0)
      unsigned_int_vector4s.erase(type);
  }

  void VertexData::addIndices(const std::vector<unsigned int>& indices) {
    checkMinimum(indices.size());
    checkDivisibility(indices.size());
    if(this->index_count <= 0) {
      this->index_count = indices.size();
      this->indices = indices;
    }
    else if(this->index_count == indices.size()) {
      this->indices = indices; 
    }
    else {
      throw std::length_error("Indices size mismatch when adding indices!");
    }
  }
  template<>
  void VertexData::addVec<float>(VertexData::DATA_TYPE data_type, const std::vector<float>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      float_vector1s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      float_vector1s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding floatvec1!");
    }
  }
  template<>
  void VertexData::addVec<glm::vec2>(VertexData::DATA_TYPE data_type, const std::vector<glm::vec2>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      float_vector2s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      float_vector2s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding floatvec2!");
    }
  }
  template<>
  void VertexData::addVec<glm::vec3>(VertexData::DATA_TYPE data_type, const std::vector<glm::vec3>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      float_vector3s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      float_vector3s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding floatvec3!");
    }
  }
  template<>
  void VertexData::addVec<glm::vec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::vec4>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      float_vector4s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      float_vector4s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding floatvec4!");
    }
  }
  template<>
  void VertexData::addVec<double>(VertexData::DATA_TYPE data_type, const std::vector<double>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      double_vector1s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      double_vector1s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding doublevec1!");
    }
  }
  template<>
  void VertexData::addVec<glm::dvec2>(VertexData::DATA_TYPE data_type, const std::vector<glm::dvec2>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      double_vector2s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      double_vector2s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding doublevec2!");
    }
  }
  template<>
  void VertexData::addVec<glm::dvec3>(VertexData::DATA_TYPE data_type, const std::vector<glm::dvec3>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      double_vector3s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      double_vector3s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding doublevec3!");
    }
  }
  template<>
  void VertexData::addVec<glm::dvec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::dvec4>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      double_vector4s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      double_vector4s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding doublevec4!");
    }
  }
  template<>
  void VertexData::addVec<int>(VertexData::DATA_TYPE data_type, const std::vector<int>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      int_vector1s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      int_vector1s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding intvec1!");
    }
  }
  template<>
  void VertexData::addVec<glm::ivec2>(VertexData::DATA_TYPE data_type, const std::vector<glm::ivec2>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      int_vector2s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      int_vector2s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding intvec2!");
    }
  }
  template<>
  void VertexData::addVec<glm::ivec3>(VertexData::DATA_TYPE data_type, const std::vector<glm::ivec3>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      int_vector3s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      int_vector3s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding intvec3!");
    }
  }
  template<>
  void VertexData::addVec<glm::ivec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::ivec4>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      int_vector4s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      int_vector4s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding intvec4!");
    }
  }
  template<>
  void VertexData::addVec<unsigned int>(VertexData::DATA_TYPE data_type, const std::vector<unsigned int>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      unsigned_int_vector1s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      unsigned_int_vector1s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding unsignedintvec1!");
    }
  }
  template<>
  void VertexData::addVec<glm::uvec2>(VertexData::DATA_TYPE data_type, const std::vector<glm::uvec2>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      unsigned_int_vector2s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      unsigned_int_vector2s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding unsignedintvec2!");
    }
  }
  template<>
  void VertexData::addVec<glm::uvec3>(VertexData::DATA_TYPE data_type, const std::vector<glm::uvec3>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      unsigned_int_vector3s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      unsigned_int_vector3s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding unsignedintvec3!");
    }
  }
  template<>
  void VertexData::addVec<glm::uvec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::uvec4>& vec){
    checkMinimum(vec.size());
    checkDivisibility(vec.size());
    clearDataType(data_type);
    if(this->vertex_count <= 0) {
      unsigned_int_vector4s[data_type] = vec;
      vertex_count = vec.size();
    }
    else if(this->vertex_count == vec.size()) {
      unsigned_int_vector4s[data_type] = vec;
    }
    else {
      throw std::length_error("Vertices size mismatch when adding unsignedintvec4!");
    }
  }
 
  const unsigned int VertexData::getVertexCount() const noexcept {
    return vertex_count;
  }

  const unsigned int VertexData::getIndexCount() const noexcept {
    return index_count;
  }

  std::vector<unsigned int> VertexData::getIndices() const noexcept {
    return indices;
  }

  template<>
  std::map<VertexData::DATA_TYPE, std::vector<float>> VertexData::getCollapsedVectors<float>() const {
    std::map<VertexData::DATA_TYPE, std::vector<float>> out_map;

    for(auto& i : float_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto& i : float_vector2s) {
      out_map[i.first] = std::vector<float>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto& i : float_vector3s) {
      out_map[i.first] = std::vector<float>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto& i : float_vector4s) {
      out_map[i.first] = std::vector<float>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  template<>
  std::map<VertexData::DATA_TYPE, std::vector<double>> VertexData::getCollapsedVectors<double>() const {
    std::map<VertexData::DATA_TYPE, std::vector<double>> out_map;

    for(auto& i : double_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto& i : double_vector2s) {
      out_map[i.first] = std::vector<double>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto& i : double_vector3s) {
      out_map[i.first] = std::vector<double>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto& i : double_vector4s) {
      out_map[i.first] = std::vector<double>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  template<>
  std::map<VertexData::DATA_TYPE, std::vector<int>> VertexData::getCollapsedVectors<int>() const{
    std::map<VertexData::DATA_TYPE, std::vector<int>> out_map;

    for(auto& i : int_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto& i : int_vector2s) {
      out_map[i.first] = std::vector<int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto& i : int_vector3s) {
      out_map[i.first] = std::vector<int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto& i : int_vector4s) {
      out_map[i.first] = std::vector<int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  template<>
  std::map<VertexData::DATA_TYPE, std::vector<unsigned int>> VertexData::getCollapsedVectors<unsigned int>() const {
    std::map<VertexData::DATA_TYPE, std::vector<unsigned int>> out_map;

    for(auto& i : unsigned_int_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto& i : unsigned_int_vector2s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto& i : unsigned_int_vector3s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto& i : unsigned_int_vector4s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto& j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  } 
}