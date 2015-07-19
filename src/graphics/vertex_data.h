#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <map>
namespace Graphics {
  class VertexData {
    public:
      enum DATA_TYPE : unsigned char { 
        GEOMETRY = 0,
        TEX_COORDS = 1,
        NORMAL_COORDS = 2,
        RESERVED1 = 3,
        RESERVED2 = 4,
        RESERVED3 = 5,
        RESERVED4 = 6,
        RESERVED5 = 7,
        RESERVED6 = 8,
        RESERVED7 = 9,
        RESERVED8 = 10,
        RESERVED9 = 11
      };

    private:
      std::map<DATA_TYPE, std::vector<float>> float_vector1s;
      std::map<DATA_TYPE, std::vector<glm::vec2>> float_vector2s;   
      std::map<DATA_TYPE, std::vector<glm::vec3>> float_vector3s;
      std::map<DATA_TYPE, std::vector<glm::vec4>> float_vector4s;
      std::map<DATA_TYPE, std::vector<double>> double_vector1s;
      std::map<DATA_TYPE, std::vector<glm::dvec2>> double_vector2s;   
      std::map<DATA_TYPE, std::vector<glm::dvec3>> double_vector3s;
      std::map<DATA_TYPE, std::vector<glm::dvec4>> double_vector4s;
      std::map<DATA_TYPE, std::vector<int>> int_vector1s;
      std::map<DATA_TYPE, std::vector<glm::ivec2>> int_vector2s;   
      std::map<DATA_TYPE, std::vector<glm::ivec3>> int_vector3s;
      std::map<DATA_TYPE, std::vector<glm::ivec4>> int_vector4s;
      std::map<DATA_TYPE, std::vector<unsigned int>> unsigned_int_vector1s;
      std::map<DATA_TYPE, std::vector<glm::uvec2>> unsigned_int_vector2s;   
      std::map<DATA_TYPE, std::vector<glm::uvec3>> unsigned_int_vector3s;
      std::map<DATA_TYPE, std::vector<glm::uvec4>> unsigned_int_vector4s;
      std::vector<unsigned int> indices;
    public:
      static const std::map<DATA_TYPE, unsigned char> DataWidth;

      VertexData();

      void addIndices(const std::vector<unsigned int>& indices);
      void addFloatVec1(DATA_TYPE data_type, const std::vector<float>& vec);
      void addFloatVec2(DATA_TYPE data_type, const std::vector<glm::vec2>& vec);
      void addFloatVec3(DATA_TYPE data_type, const std::vector<glm::vec3>& vec);
      void addFloatVec4(DATA_TYPE data_type, const std::vector<glm::vec4>& vec);
      void addDoubleVec1(DATA_TYPE data_type, const std::vector<double>& vec);
      void addDoubleVec2(DATA_TYPE data_type, const std::vector<glm::dvec2>& vec);
      void addDoubleVec3(DATA_TYPE data_type, const std::vector<glm::dvec3>& vec);
      void addDoubleVec4(DATA_TYPE data_type, const std::vector<glm::dvec4>& vec);
      void addIntVec1(DATA_TYPE data_type, const std::vector<int>& vec);
      void addIntVec2(DATA_TYPE data_type, const std::vector<glm::ivec2>& vec);
      void addIntVec3(DATA_TYPE data_type, const std::vector<glm::ivec3>& vec);
      void addIntVec4(DATA_TYPE data_type, const std::vector<glm::ivec4>& vec);
      void addUnsignedIntVec1(DATA_TYPE data_type, const std::vector<unsigned int>& vec);
      void addUnsignedIntVec2(DATA_TYPE data_type, const std::vector<glm::uvec2>& vec);
      void addUnsignedIntVec3(DATA_TYPE data_type, const std::vector<glm::uvec3>& vec);
      void addUnsignedIntVec4(DATA_TYPE data_type, const std::vector<glm::uvec4>& vec);

      std::map<DATA_TYPE, std::vector<float>> getCollapsedFloatVectors() const;
      std::map<DATA_TYPE, std::vector<double>> getCollapsedDoubleVectors() const;
      std::map<DATA_TYPE, std::vector<int>> getCollapsedIntVectors() const;
      std::map<DATA_TYPE, std::vector<unsigned int>> getCollapsedUnsignedIntVectors() const;
      std::vector<unsigned int> getIndices() const noexcept;
  };
}

#endif
