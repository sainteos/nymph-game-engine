#include "vertex_data.h"

namespace Graphics {
  VertexData::VertexData() {

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
  };

  void VertexData::addIndices(const std::vector<unsigned int>& indices) {
    this->indices = indices;
  }
  void VertexData::addFloatVec1(DATA_TYPE data_type, const std::vector<float>& vec){
    float_vector1s[data_type] = vec;
  }
  void VertexData::addFloatVec2(DATA_TYPE data_type, const std::vector<glm::vec2>& vec){
    float_vector2s[data_type] = vec;
  }
  void VertexData::addFloatVec3(DATA_TYPE data_type, const std::vector<glm::vec3>& vec){
    float_vector3s[data_type] = vec;
  }
  void VertexData::addFloatVec4(DATA_TYPE data_type, const std::vector<glm::vec4>& vec){
    float_vector4s[data_type] = vec;
  }
  void VertexData::addDoubleVec1(DATA_TYPE data_type, const std::vector<double>& vec){
    double_vector1s[data_type] = vec;
  }
  void VertexData::addDoubleVec2(DATA_TYPE data_type, const std::vector<glm::dvec2>& vec){
    double_vector2s[data_type] = vec;
  }
  void VertexData::addDoubleVec3(DATA_TYPE data_type, const std::vector<glm::dvec3>& vec){
    double_vector3s[data_type] = vec;
  }
  void VertexData::addDoubleVec4(DATA_TYPE data_type, const std::vector<glm::dvec4>& vec){
    double_vector4s[data_type] = vec;
  }
  void VertexData::addIntVec1(DATA_TYPE data_type, const std::vector<int>& vec){
    int_vector1s[data_type] = vec;
  }
  void VertexData::addIntVec2(DATA_TYPE data_type, const std::vector<glm::ivec2>& vec){
    int_vector2s[data_type] = vec;
  }
  void VertexData::addIntVec3(DATA_TYPE data_type, const std::vector<glm::ivec3>& vec){
    int_vector3s[data_type] = vec;
  }
  void VertexData::addIntVec4(DATA_TYPE data_type, const std::vector<glm::ivec4>& vec){
    int_vector4s[data_type] = vec;
  }
  void VertexData::addUnsignedIntVec1(DATA_TYPE data_type, const std::vector<unsigned int>& vec){
    unsigned_int_vector1s[data_type] = vec;
  }
  void VertexData::addUnsignedIntVec2(DATA_TYPE data_type, const std::vector<glm::uvec2>& vec){
    unsigned_int_vector2s[data_type] = vec;
  }
  void VertexData::addUnsignedIntVec3(DATA_TYPE data_type, const std::vector<glm::uvec3>& vec){
    unsigned_int_vector3s[data_type] = vec;
  }
  void VertexData::addUnsignedIntVec4(DATA_TYPE data_type, const std::vector<glm::uvec4>& vec){
    unsigned_int_vector4s[data_type] = vec;
  }

  std::vector<unsigned int> VertexData::getIndices() const noexcept{
    return indices;
  }

  std::map<VertexData::DATA_TYPE, std::vector<float>> VertexData::getCollapsedFloatVectors() const {
    std::map<VertexData::DATA_TYPE, std::vector<float>> out_map;

    for(auto i : float_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto i : float_vector2s) {
      out_map[i.first] = std::vector<float>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto i : float_vector3s) {
      out_map[i.first] = std::vector<float>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto i : float_vector4s) {
      out_map[i.first] = std::vector<float>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  std::map<VertexData::DATA_TYPE, std::vector<double>> VertexData::getCollapsedDoubleVectors() const {
    std::map<VertexData::DATA_TYPE, std::vector<double>> out_map;

    for(auto i : double_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto i : double_vector2s) {
      out_map[i.first] = std::vector<double>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto i : double_vector3s) {
      out_map[i.first] = std::vector<double>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto i : double_vector4s) {
      out_map[i.first] = std::vector<double>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  std::map<VertexData::DATA_TYPE, std::vector<int>> VertexData::getCollapsedIntVectors() const {
    std::map<VertexData::DATA_TYPE, std::vector<int>> out_map;

    for(auto i : int_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto i : int_vector2s) {
      out_map[i.first] = std::vector<int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto i : int_vector3s) {
      out_map[i.first] = std::vector<int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto i : int_vector4s) {
      out_map[i.first] = std::vector<int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  }

  std::map<VertexData::DATA_TYPE, std::vector<unsigned int>> VertexData::getCollapsedUnsignedIntVectors() const {
    std::map<VertexData::DATA_TYPE, std::vector<unsigned int>> out_map;

    for(auto i : unsigned_int_vector1s) {
      out_map[i.first] = i.second;
    }
    for(auto i : unsigned_int_vector2s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
      }
    }
    for(auto i : unsigned_int_vector3s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
      }
    }
    for(auto i : unsigned_int_vector4s) {
      out_map[i.first] = std::vector<unsigned int>();
      for(auto j : i.second) {
        out_map[i.first].push_back(j.x);
        out_map[i.first].push_back(j.y);
        out_map[i.first].push_back(j.z);
        out_map[i.first].push_back(j.w);
      }
    }
    return out_map;
  } 
}