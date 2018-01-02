#include <easylogging++.h>
#include <exception>
#include <utility>
#include <limits>
#include <glm/ext.hpp>
#include "vertex_data.h"

namespace Graphics {
  VertexData::VertexData(GLenum primitive_type) : index_count(0), vertex_count(0), primitive_type(primitive_type), highest_z(-1 * std::numeric_limits<float>::infinity()) {

  }


  VertexData::VertexData(const VertexData& vertex_data) {
    float_vector1s = vertex_data.float_vector1s;
    float_vector2s = vertex_data.float_vector2s;
    float_vector3s = vertex_data.float_vector3s;
    float_vector4s = vertex_data.float_vector4s;
    double_vector1s = vertex_data.double_vector1s;
    double_vector2s = vertex_data.double_vector2s;
    double_vector3s = vertex_data.double_vector3s;
    double_vector4s = vertex_data.double_vector4s;
    int_vector1s = vertex_data.int_vector1s;
    int_vector2s = vertex_data.int_vector2s;
    int_vector3s = vertex_data.int_vector3s;
    int_vector4s = vertex_data.int_vector4s;
    unsigned_int_vector1s = vertex_data.unsigned_int_vector1s;
    unsigned_int_vector2s = vertex_data.unsigned_int_vector2s;
    unsigned_int_vector3s = vertex_data.unsigned_int_vector3s;
    unsigned_int_vector4s = vertex_data.unsigned_int_vector4s;
    indices = vertex_data.indices;
    index_count = vertex_data.index_count;
    vertex_count = vertex_data.vertex_count;
    primitive_type = vertex_data.primitive_type;
    highest_z = vertex_data.highest_z;
  }

  VertexData VertexData::operator=(const VertexData& vertex_data) {
    float_vector1s = vertex_data.float_vector1s;
    float_vector2s = vertex_data.float_vector2s;
    float_vector3s = vertex_data.float_vector3s;
    float_vector4s = vertex_data.float_vector4s;
    double_vector1s = vertex_data.double_vector1s;
    double_vector2s = vertex_data.double_vector2s;
    double_vector3s = vertex_data.double_vector3s;
    double_vector4s = vertex_data.double_vector4s;
    int_vector1s = vertex_data.int_vector1s;
    int_vector2s = vertex_data.int_vector2s;
    int_vector3s = vertex_data.int_vector3s;
    int_vector4s = vertex_data.int_vector4s;
    unsigned_int_vector1s = vertex_data.unsigned_int_vector1s;
    unsigned_int_vector2s = vertex_data.unsigned_int_vector2s;
    unsigned_int_vector3s = vertex_data.unsigned_int_vector3s;
    unsigned_int_vector4s = vertex_data.unsigned_int_vector4s;
    indices = vertex_data.indices;
    index_count = vertex_data.index_count;
    vertex_count = vertex_data.vertex_count;
    primitive_type = vertex_data.primitive_type;
    highest_z = vertex_data.highest_z;
    return *this;
  }

  VertexData::~VertexData() {
    highest_z = -1 * std::numeric_limits<float>::infinity();
  }

  //initialize the datawidth
  const std::map<Graphics::VertexData::DATA_TYPE, unsigned int> VertexData::DataWidth = std::map<Graphics::VertexData::DATA_TYPE, unsigned int>  {
        {Graphics::VertexData::DATA_TYPE::GEOMETRY, 3},
        {Graphics::VertexData::DATA_TYPE::TEX_COORDS, 2},
        {Graphics::VertexData::DATA_TYPE::TEXTURE_UNIT, 1},
        {Graphics::VertexData::DATA_TYPE::NORMAL_COORDS, 3},
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

  bool VertexData::operator==(const VertexData& other) {
    return float_vector1s.size() == other.float_vector1s.size() &&
           float_vector2s.size() == other.float_vector2s.size() &&
           float_vector3s.size() == other.float_vector3s.size() &&
           float_vector4s.size() == other.float_vector4s.size() &&
           double_vector1s.size() == other.double_vector1s.size() &&
           double_vector2s.size() == other.double_vector2s.size() &&
           double_vector3s.size() == other.double_vector3s.size() &&
           double_vector4s.size() == other.double_vector4s.size() &&
           int_vector1s.size() == other.int_vector1s.size() &&
           int_vector2s.size() == other.int_vector2s.size() &&
           int_vector3s.size() == other.int_vector3s.size() &&
           int_vector4s.size() == other.int_vector4s.size() &&
           unsigned_int_vector1s.size() == other.unsigned_int_vector1s.size() &&
           unsigned_int_vector2s.size() == other.unsigned_int_vector2s.size() &&
           unsigned_int_vector3s.size() == other.unsigned_int_vector3s.size() &&
           unsigned_int_vector4s.size() == other.unsigned_int_vector4s.size() &&
           indices.size() == other.indices.size() &&
           index_count == other.index_count &&
           vertex_count == other.vertex_count &&
           primitive_type == other.primitive_type &&
           mapCompare(float_vector1s, other.float_vector1s) &&
           mapCompare(float_vector2s, other.float_vector2s) &&
           mapCompare(float_vector3s, other.float_vector3s) &&
           mapCompare(float_vector4s, other.float_vector4s) &&
           mapCompare(double_vector1s, other.double_vector1s) &&
           mapCompare(double_vector2s, other.double_vector2s) &&
           mapCompare(double_vector3s, other.double_vector3s) &&
           mapCompare(double_vector4s, other.double_vector4s) &&
           mapCompare(int_vector1s, other.int_vector1s) &&
           mapCompare(int_vector2s, other.int_vector2s) &&
           mapCompare(int_vector3s, other.int_vector3s) &&
           mapCompare(int_vector4s, other.int_vector4s) &&
           mapCompare(unsigned_int_vector1s, other.unsigned_int_vector1s) &&
           mapCompare(unsigned_int_vector2s, other.unsigned_int_vector2s) &&
           mapCompare(unsigned_int_vector3s, other.unsigned_int_vector3s) &&
           mapCompare(unsigned_int_vector4s, other.unsigned_int_vector4s) &&
           indices == other.indices;
  }

  bool VertexData::operator!=(const VertexData& other) {
    return !(*this == other);
  }

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

  template<class T>
  bool VertexData::mapCompare(const std::map<DATA_TYPE, std::vector<T>> lhs, const std::map<DATA_TYPE, std::vector<T>> rhs) {
    return lhs.size() == rhs.size() &&
           std::equal(lhs.begin(), lhs.end(), rhs.begin());
  }

  void VertexData::setHighestZIfHigher(float z) noexcept {
    if(z > highest_z)
      highest_z = z;
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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

    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<glm::vec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::vec4>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<double>(VertexData::DATA_TYPE data_type, const std::vector<double>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<glm::dvec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::dvec4>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<int>(VertexData::DATA_TYPE data_type, const std::vector<int>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<glm::ivec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::ivec4>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<unsigned int>(VertexData::DATA_TYPE data_type, const std::vector<unsigned int>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }
  template<>
  void VertexData::addVec<glm::uvec4>(VertexData::DATA_TYPE data_type, const std::vector<glm::uvec4>& vec){
    if(indices.size() == 0) {
      checkMinimum(vec.size());
      checkDivisibility(vec.size());
    }
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
    if(data_type == VertexData::DATA_TYPE::GEOMETRY) {
      for(auto v : vec) {
        setHighestZIfHigher(v.z);
      }
    }
  }

  unsigned int VertexData::getVertexCount() const noexcept {
    return vertex_count;
  }

  unsigned int VertexData::getIndexCount() const noexcept {
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

  float VertexData::highestZ() const noexcept {
    return highest_z;
  }

  unsigned int VertexData::numberVertexBufferObjects() const noexcept {
    return float_vector1s.size() + float_vector2s.size() + float_vector3s.size() + float_vector4s.size() +
           double_vector1s.size() + double_vector2s.size() + double_vector3s.size() + double_vector4s.size() +
           int_vector1s.size() + int_vector2s.size() + int_vector3s.size() + int_vector4s.size() +
           unsigned_int_vector1s.size() + unsigned_int_vector2s.size() + unsigned_int_vector3s.size() + unsigned_int_vector4s.size();
  }

  unsigned int VertexData::generateVertexArrayObject() const {
    unsigned int vertex_array_object = 0;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

    auto float_data = getCollapsedVectors<float>();
    auto double_data = getCollapsedVectors<double>();
    auto int_data = getCollapsedVectors<int>();
    auto unsigned_int_data = getCollapsedVectors<unsigned int>();

    unsigned int num_of_vertex_buffers = numberVertexBufferObjects();
    unsigned int* vertex_buffer_objects = new unsigned int[num_of_vertex_buffers];
    unsigned int index_buffer_object = 0;

    std::vector<std::pair<VertexData::DATA_TYPE, GLenum>> data_types;
    glGenBuffers(num_of_vertex_buffers, vertex_buffer_objects);

    unsigned int current_buffer = 0;

    //Do this if we actually have indices
    if(getIndices().size() > 0) {
      glGenBuffers(1, &index_buffer_object);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndices().size() * sizeof(unsigned int), &(getIndices())[0], GL_STATIC_DRAW);
    }

    for(auto i : float_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(float), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_FLOAT));
    }

    for(auto i : double_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(double), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_DOUBLE));
    }

    for(auto i : int_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(int), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_INT));
    }

    for(auto i : unsigned_int_data) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(unsigned int), &(i.second)[0], GL_STATIC_DRAW);
      data_types.push_back(std::pair<VertexData::DATA_TYPE, GLenum>(i.first, GL_UNSIGNED_INT));
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vertex_array_object);

    for(int i = 0; i < num_of_vertex_buffers; i++) {
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[i]);
      if(data_types[i].second == GL_FLOAT || data_types[i].second == GL_DOUBLE)
        glVertexAttribPointer(data_types[i].first, VertexData::DataWidth.at(data_types[i].first), data_types[i].second, GL_FALSE, 0, 0);
      //VERY IMPORTANT, WITHOUT glVertexAttribIPointer, integers will be destroyed!!
      if(data_types[i].second == GL_INT || data_types[i].second == GL_UNSIGNED_INT)
        glVertexAttribIPointer(data_types[i].first, VertexData::DataWidth.at(data_types[i].first), data_types[i].second, 0, 0);
      glEnableVertexAttribArray(data_types[i].first);
    }

    if(getIndices().size() > 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    }
    glBindVertexArray(0);

    return vertex_array_object;
  }

  std::string VertexData::to_string() const noexcept {
    std::stringstream str;
    str << "VertexData:: Indices: "<<index_count<<" Vertices: "<<vertex_count<<" Highest Z: "<<highest_z<<"\n";

    if(float_vector1s.size() > 0) {
      for(auto pair : float_vector1s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << f << ", ";
        }
      }
      str <<"\n";
    }

    if(float_vector2s.size() > 0) {
      for(auto pair : float_vector2s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(float_vector3s.size() > 0) {
      for(auto pair : float_vector3s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(float_vector4s.size() > 0) {
      for(auto pair : float_vector4s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }


    if(double_vector1s.size() > 0) {
      for(auto pair : double_vector1s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << f << ", ";
        }
      }
      str <<"\n";
    }

    if(double_vector2s.size() > 0) {
      for(auto pair : double_vector2s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(double_vector3s.size() > 0) {
      for(auto pair : double_vector3s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(double_vector4s.size() > 0) {
      for(auto pair : double_vector4s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(int_vector1s.size() > 0) {
      for(auto pair : int_vector1s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << f << ", ";
        }
      }
      str <<"\n";
    }

    if(int_vector2s.size() > 0) {
      for(auto pair : int_vector2s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(int_vector3s.size() > 0) {
      for(auto pair : int_vector3s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(int_vector4s.size() > 0) {
      for(auto pair : int_vector4s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(unsigned_int_vector1s.size() > 0) {
      for(auto pair : unsigned_int_vector1s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << f << ", ";
        }
      }
      str <<"\n";
    }

    if(unsigned_int_vector2s.size() > 0) {
      for(auto pair : unsigned_int_vector2s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(unsigned_int_vector3s.size() > 0) {
      for(auto pair : unsigned_int_vector3s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }

    if(unsigned_int_vector4s.size() > 0) {
      for(auto pair : unsigned_int_vector4s) {
        str <<"Vec Type: "<<pair.first<<": ";
        for(auto f : pair.second) {
          str << glm::to_string(f) << ", ";
        }
      }
      str <<"\n";
    }
    return str.str();
  }
}
