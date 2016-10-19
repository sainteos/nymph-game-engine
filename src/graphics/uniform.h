#ifndef UNIFORM_H
#define UNIFORM_H
#include <glm/glm.hpp>
#include <string>

namespace Graphics {
  class Uniform {
    public:
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
      Uniform();
      template<typename T>
      void setData(const std::string& name, const T& data);
      const UniformTypes getType() const noexcept;
      const std::string getName() const noexcept;
      const bool isDirty() const noexcept;
      void clean() noexcept;

      template<typename T>
      const T getData() const noexcept;

      const bool operator<(const Uniform& right) const noexcept;
      const bool operator==(const Uniform& right) const noexcept;
      const bool operator!=(const Uniform& right) const noexcept;

      const std::string to_string() const noexcept;
      //= END SCRIPTABLE
  };
}

#endif
