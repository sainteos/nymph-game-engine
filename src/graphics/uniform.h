#ifndef UNIFORM_H
#define UNIFORM_H
#include <glm/glm.hpp>
#include <string>

namespace Graphics {
  /**
   * @brief      Class for shader uniform.
   */
  class Uniform {
    public:
      /**
       * Enum describing the different types that can be held in a uniform.
       */
      enum class UniformTypes { FLOAT, VEC2, VEC3, VEC4, INT, IVEC2, IVEC3, IVEC4, UINT, UVEC2, UVEC3, UVEC4,
                          BOOL, BVEC2, BVEC3, BVEC4, MAT2, MAT3, MAT4, MAT23, MAT32, MAT24, MAT42, MAT34, MAT43 };
    private:
      UniformTypes uniform_type;
      std::string name;
      float float_data;
      glm::vec2 vec2_data;
      glm::vec3 vec3_data;
      glm::vec4 vec4_data;
      int int_data;
      glm::ivec2 ivec2_data;
      glm::ivec3 ivec3_data;
      glm::ivec4 ivec4_data;
      unsigned int uint_data;
      glm::uvec2 uvec2_data;
      glm::uvec3 uvec3_data;
      glm::uvec4 uvec4_data;
      bool bool_data;
      glm::bvec2 bvec2_data;
      glm::bvec3 bvec3_data;
      glm::bvec4 bvec4_data;
      glm::mat2 mat2_data;
      glm::mat3 mat3_data;
      glm::mat4 mat4_data;
      glm::mat2x3 mat23_data;
      glm::mat3x2 mat32_data;
      glm::mat2x4 mat24_data;
      glm::mat4x2 mat42_data;
      glm::mat3x4 mat34_data;
      glm::mat4x3 mat43_data;

      bool dirty;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      Uniform constructor.
       */
      Uniform();
      /**
       * @brief      Sets the data.
       *
       * @param[in]  name  The name
       * @param[in]  data  The data
       *
       * @tparam     T     DataType for uniform
       */
      template<typename T>
      void setData(const std::string& name, const T& data);
      /**
       * @brief      Gets the type.
       *
       * @return     The type.
       */
      UniformTypes getType() const noexcept;
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      std::string getName() const noexcept;
      /**
       * @brief      Determines if dirty.
       *
       * @return     True if dirty, False otherwise.
       */
      bool isDirty() const noexcept;
      /**
       * @brief      dirty = false
       */
      void clean() noexcept;

      /**
       * @brief      Gets the data.
       *
       * @tparam     T     Type of data to get
       *
       * @return     The data.
       */
      template<typename T>
      T getData() const noexcept;

      /**
       * @brief      < operator using name
       *
       * @param[in]  right  The right
       *
       * @return     true if right.name is alphabetically higher than this.name
       */
      bool operator<(const Uniform& right) const noexcept;
      /**
       * @brief      == operator
       *
       * @param[in]  right  The right
       *
       * @return     True if same uniform
       */
      bool operator==(const Uniform& right) const noexcept;
      /**
       * @brief      != operator
       *
       * @param[in]  right  The right
       *
       * @return     True if not the same uniform
       */
      bool operator!=(const Uniform& right) const noexcept;

      /**
       * @brief      Returns a string representation of the object.
       *
       * @return     String representation of the object.
       */
      std::string to_string() const noexcept;
      //= END SCRIPTABLE
  };
}

#endif
