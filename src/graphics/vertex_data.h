#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
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
        RESERVED9 = 11,
        ONE_WIDE = 12,
        TWO_WIDE = 13,
        THREE_WIDE = 14,
        FOUR_WIDE = 15
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
      unsigned int index_count;
      unsigned int vertex_count;
      GLenum primitive_type;

      const unsigned char ANY = 0;

      const std::map<GLenum, unsigned char> divisibility {
        {GL_TRIANGLES, 3},
        {GL_TRIANGLE_STRIP, ANY},
        {GL_TRIANGLE_FAN, ANY},
        {GL_LINES, 2},
        {GL_LINE_LOOP, ANY},
        {GL_LINE_STRIP, ANY},
        {GL_QUADS, 4}
      };

      const std::map<GLenum, unsigned char> minimum {
        {GL_TRIANGLES, 3},
        {GL_TRIANGLE_STRIP, 3},
        {GL_TRIANGLE_FAN, 3},
        {GL_LINES, 2},
        {GL_LINE_STRIP, 2},
        {GL_LINE_LOOP, 3},
        {GL_QUADS, 4}
      };

      void checkDivisibility(const unsigned int size);
      void checkMinimum(const unsigned int size);
      void clearDataType(const DATA_TYPE& type);

      template<class T>
      const bool mapCompare(std::map<DATA_TYPE, std::vector<T>> lhs, std::map<DATA_TYPE, std::vector<T>> rhs);
    public:
      static const std::map<DATA_TYPE, unsigned int> DataWidth;
      
      VertexData(const GLenum primitive_type = GL_TRIANGLES);

      VertexData(const VertexData& vertex_data);
      VertexData operator=(const VertexData& vertex_data);
      
      ~VertexData();

      const bool operator==(const VertexData& other);
      const bool operator!=(const VertexData& other);

      //if indices are added first, then they are used for min and divisibility validation
      //otherwise, vertices added are used for min and divisibility validation
      void addIndices(const std::vector<unsigned int>& indices);

      template<typename T>
      void addVec(DATA_TYPE data_type, const std::vector<T>& vec);

      const unsigned int getIndexCount() const noexcept;
      const unsigned int getVertexCount() const noexcept;
      
      template<typename T>
      std::map<DATA_TYPE, std::vector<T>> getCollapsedVectors() const;

      std::vector<unsigned int> getIndices() const noexcept;

      unsigned int numberVertexBufferObjects() const noexcept;
      const unsigned int generateVertexArrayObject() const;
  };
}

#endif
