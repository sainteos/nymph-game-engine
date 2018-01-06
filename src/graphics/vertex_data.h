#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include <map>
namespace Graphics {
  /**
   * @brief      Class for vertex data.
   */
  class VertexData {
    public:
      /**
       * @brief      VertexData possible types
       */
      enum DATA_TYPE : unsigned int {
        GEOMETRY = 0,
        TEX_COORDS = 1,
        TEXTURE_UNIT = 2,
        NORMAL_COORDS = 3,
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
      float highest_z;

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

      void setHighestZIfHigher(float z) noexcept;

      template<class T>
      bool mapCompare(std::map<DATA_TYPE, std::vector<T>> lhs, std::map<DATA_TYPE, std::vector<T>> rhs);

    public:
      /**
       * Holds the different data widths possible for DATA_TYPE
       */
      static const std::map<DATA_TYPE, unsigned int> DataWidth;

      /**
       * @brief      VertexData constructor
       *
       * @param[in]  primitive_type  The primitive type
       */
      VertexData(const GLenum primitive_type = GL_TRIANGLES);

      /**
       * @brief      VertexData copy constructor
       *
       * @param[in]  vertex_data  The vertex data
       */
      VertexData(const VertexData& vertex_data);
      /**
       * @brief      = operator
       *
       * @param[in]  vertex_data  The vertex data
       *
       * @return     this = other
       */
      VertexData operator=(const VertexData& vertex_data);

      /**
       * @brief      Destroys the object.
       */
      ~VertexData();

      /**
       * @brief      == operator
       *
       * @param[in]  other  The other
       *
       * @return     True if same vertexdata
       */
      bool operator==(const VertexData& other);
      /**
       * @brief      != operator
       *
       * @param[in]  other  The other
       *
       * @return     True if not same vertexdata
       */
      bool operator!=(const VertexData& other);

      /**
       * @brief      Adds indices.
       *
       * @detail     If indices are added first, then they are used for min and divisibility validation
       * otherwise, vertices added are used for min and divisibility validation
       *
       * @param[in]  indices  The indices
       */
      void addIndices(const std::vector<unsigned int>& indices);

      /**
       * @brief      Adds a vector of data for a certain data type
       *
       * @param[in]  data_type  The data type
       * @param[in]  vec        The vector
       *
       * @tparam     T          Physical Type of data to add
       */
      template<typename T>
      void addVec(DATA_TYPE data_type, const std::vector<T>& vec);

      /**
       * @brief      Gets the index count.
       *
       * @return     The index count.
       */
      unsigned int getIndexCount() const noexcept;
      /**
       * @brief      Gets the vertex count.
       *
       * @return     The vertex count.
       */
      unsigned int getVertexCount() const noexcept;

      /**
       * @brief      Gets a map of collapsed type vectors to a data types
       *
       * @return     Map of data types to vectors of type
       */
      template<typename T>
      std::map<DATA_TYPE, std::vector<T>> getCollapsedVectors() const;

      /**
       * @brief      Gets the indices.
       *
       * @return     The indices.
       */
      std::vector<unsigned int> getIndices() const noexcept;

      /**
       * @brief      Returns the highest Z value found in this vertex data.
       *
       * @return     float representing highest z value
       */
      float highestZ() const noexcept;

      /**
       * @brief      Returns number of vertex buffer objects needed for this vertex data
       *
       * @return     Number of vbos
       */
      unsigned int numberVertexBufferObjects() const noexcept;
      /**
       * @brief      Generates an opengl vertex array object
       *
       * @return     A handle to an opengl vao
       */
      unsigned int generateVertexArrayObject() const;

      /**
       * @brief      Returns a string representation of the object.
       *
       * @return     String representation of the object.
       */
      std::string to_string() const noexcept;
  };
}

#endif
