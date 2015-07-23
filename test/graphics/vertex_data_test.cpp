#include <easylogging++.h>
#include <catch.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <OpenGL/gl3.h>
#include <map>
#include "graphics/vertex_data.h"

using namespace Graphics;

const unsigned char ANY = 0;

std::map<GLenum, unsigned char> divisibility {
  {GL_TRIANGLES, 3},
  {GL_TRIANGLE_STRIP, ANY},
  {GL_TRIANGLE_FAN, ANY},
  {GL_LINES, 2},
  {GL_LINE_LOOP, ANY},
  {GL_LINE_STRIP, ANY},
  {GL_QUADS, 4}
};

std::map<GLenum, unsigned char> minimum {
  {GL_TRIANGLES, 3},
  {GL_TRIANGLE_STRIP, 3},
  {GL_TRIANGLE_FAN, 3},
  {GL_LINES, 2},
  {GL_LINE_STRIP, 2},
  {GL_LINE_LOOP, 3},
  {GL_QUADS, 4}
};

std::map<GLenum, std::string> enum_to_string {
  {GL_TRIANGLES, "GL_TRIANGLES"},
  {GL_TRIANGLE_STRIP, "GL_TRIANGLE_STRIP"},
  {GL_TRIANGLE_FAN, "GL_TRIANGLE_FAN"},
  {GL_LINES, "GL_LINES"},
  {GL_LINE_STRIP, "GL_LINE_STRIP"},
  {GL_LINE_LOOP, "GL_LINE_LOOP"},
  {GL_QUADS, "GL_QUADS"}
};

SCENARIO("Indices added to Vertex Data with less than minimum are rejected") {
  GIVEN("Vectors with 1 less than the minimum for each primitive type") {
    for(auto& min : minimum){
      std::vector<unsigned int> vec(min.second - 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addIndices(vec), std::length_error);
        }
      }
    }
  }
}

SCENARIO("Indices added to Vertex Data not divisible by the correct amount are rejected") {
  GIVEN("Vectors divisible by the correct amount for each primitive type") {
    for(auto& divide : divisibility){
      std::vector<unsigned int> vec(minimum[divide.first] + divide.second + 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addIndices(vec), std::length_error);
        }
      }
    }
  }
}

SCENARIO("Valid indices added to Vertex Data reflect in getter and count") {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<unsigned int> dist(1, 10);

  GIVEN("Valid indices for each primitive type") {
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::uniform_int_distribution<unsigned int> index(0, min.second * multiplier);
      std::vector<unsigned int> vec(min.second * multiplier, index(mt));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addIndices(vec);
        THEN("the vertex data object should have the correct indices in it") {
          REQUIRE(data.getIndices() == vec);
        }
        THEN("the vertex data object should have the correct index count") {
          REQUIRE(data.getIndexCount() == vec.size());
        }
      }
    }
  }
}

SCENARIO("Vertices added to Vertex Data with less than minimum are rejected") {
  GIVEN("Vectors with 1 less than the minimum for each primitive type and each vertex data type") {
    //float
    for(auto& min : minimum){
      std::vector<float> vec(min.second - 1, 0.0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<float>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec2
    for(auto& min : minimum){
      std::vector<glm::vec2> vec(min.second - 1, glm::vec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec3
    for(auto& min : minimum){
      std::vector<glm::vec3> vec(min.second - 1, glm::vec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec4
    for(auto& min : minimum){
      std::vector<glm::vec4> vec(min.second - 1, glm::vec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //double
    for(auto& min : minimum){
      std::vector<double> vec(min.second - 1, 0.0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<double>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec2
    for(auto& min : minimum){
      std::vector<glm::dvec2> vec(min.second - 1, glm::dvec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec3
    for(auto& min : minimum){
      std::vector<glm::dvec3> vec(min.second - 1, glm::dvec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec4
    for(auto& min : minimum){
      std::vector<glm::dvec4> vec(min.second - 1, glm::dvec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //int
    for(auto& min : minimum){
      std::vector<int> vec(min.second - 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<int>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec2
    for(auto& min : minimum){
      std::vector<glm::ivec2> vec(min.second - 1, glm::ivec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec3
    for(auto& min : minimum){
      std::vector<glm::ivec3> vec(min.second - 1, glm::ivec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec4
    for(auto& min : minimum){
      std::vector<glm::ivec4> vec(min.second - 1, glm::ivec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //unsigned int
    for(auto& min : minimum){
      std::vector<unsigned int> vec(min.second - 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<unsigned int>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::uvec2
    for(auto& min : minimum){
      std::vector<glm::uvec2> vec(min.second - 1, glm::uvec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::uvec3
    for(auto& min : minimum){
      std::vector<glm::uvec3> vec(min.second - 1, glm::uvec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec4
    for(auto& min : minimum){
      std::vector<glm::uvec4> vec(min.second - 1, glm::uvec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
  }
}

SCENARIO("Valid Vertices added to Vertex Data are reflected in count and getter") {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<unsigned int> dist(1, 10);
  std::uniform_int_distribution<unsigned int> rand_uint(0, 100);
  std::uniform_int_distribution<int> rand_int(-100, 100);
  std::uniform_real_distribution<float> rand_float(0.0, 10.0);
  std::uniform_real_distribution<double> rand_double(0.0, 10.0);

  GIVEN("Vectors with correct amounts for each primitive type and each vertex data type") {
    //float
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<float> vec(min.second * multiplier, rand_float(mt));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<float>(VertexData::DATA_TYPE::ONE_WIDE, vec);
        THEN("it will have the correct data") {
          REQUIRE(data.getCollapsedVectors<float>().at(VertexData::DATA_TYPE::ONE_WIDE) == vec);
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::vec2
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::vec2> vec(min.second * multiplier, glm::vec2(rand_float(mt), rand_float(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::vec2>(VertexData::DATA_TYPE::TWO_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<float>();
          for(int i = 0; i < data.getVertexCount() * 2; i+=2) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i] == vec[i/2].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i+1] == vec[i/2].y);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::vec3
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::vec3> vec(min.second * multiplier, glm::vec3(rand_float(mt), rand_float(mt), rand_float(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::vec3>(VertexData::DATA_TYPE::THREE_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<float>();
          for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i] == vec[i/3].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+1] == vec[i/3].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+2] == vec[i/3].z);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::vec4
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::vec4> vec(min.second * multiplier, glm::vec4(rand_float(mt), rand_float(mt), rand_float(mt), rand_float(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::vec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<float>();
          for(int i = 0; i < data.getVertexCount() * 4; i+=4) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i] == vec[i/4].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+1] == vec[i/4].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+2] == vec[i/4].z);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+3] == vec[i/4].w);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //double
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<double> vec(min.second * multiplier, rand_double(mt));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<double>(VertexData::DATA_TYPE::ONE_WIDE, vec);
        THEN("it will have the correct data") {
          REQUIRE(data.getCollapsedVectors<double>().at(VertexData::DATA_TYPE::ONE_WIDE) == vec);
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::dvec2
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::dvec2> vec(min.second * multiplier, glm::dvec2(rand_double(mt), rand_double(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::dvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<double>();
          for(int i = 0; i < data.getVertexCount() * 2; i+=2) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i] == vec[i/2].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i+1] == vec[i/2].y);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::dvec3
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::dvec3> vec(min.second * multiplier, glm::dvec3(rand_double(mt), rand_double(mt), rand_double(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::dvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<double>();
          for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i] == vec[i/3].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+1] == vec[i/3].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+2] == vec[i/3].z);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::dvec4
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::dvec4> vec(min.second * multiplier, glm::dvec4(rand_double(mt), rand_double(mt), rand_double(mt), rand_double(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::dvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<double>();
          for(int i = 0; i < data.getVertexCount() * 4; i+=4) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i] == vec[i/4].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+1] == vec[i/4].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+2] == vec[i/4].z);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+3] == vec[i/4].w);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //int
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<int> vec(min.second * multiplier, rand_int(mt));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<int>(VertexData::DATA_TYPE::ONE_WIDE, vec);
        THEN("it will have the correct data") {
          REQUIRE(data.getCollapsedVectors<int>().at(VertexData::DATA_TYPE::ONE_WIDE) == vec);
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::ivec2
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::ivec2> vec(min.second * multiplier, glm::ivec2(rand_int(mt), rand_int(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::ivec2>(VertexData::DATA_TYPE::TWO_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<int>();
          for(int i = 0; i < data.getVertexCount() * 2; i+=2) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i] == vec[i/2].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i+1] == vec[i/2].y);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::ivec3
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::ivec3> vec(min.second * multiplier, glm::ivec3(rand_int(mt), rand_int(mt), rand_int(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::ivec3>(VertexData::DATA_TYPE::THREE_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<int>();
          for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i] == vec[i/3].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+1] == vec[i/3].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+2] == vec[i/3].z);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::ivec4
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::ivec4> vec(min.second * multiplier, glm::ivec4(rand_int(mt), rand_int(mt), rand_int(mt), rand_int(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::ivec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<int>();
          for(int i = 0; i < data.getVertexCount() * 4; i+=4) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i] == vec[i/4].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+1] == vec[i/4].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+2] == vec[i/4].z);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+3] == vec[i/4].w);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //unsigned int
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<unsigned int> vec(min.second * multiplier, rand_uint(mt));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<unsigned int>(VertexData::DATA_TYPE::ONE_WIDE, vec);
        THEN("it will have the correct data") {
          REQUIRE(data.getCollapsedVectors<unsigned int>().at(VertexData::DATA_TYPE::ONE_WIDE) == vec);
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::uvec2
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::uvec2> vec(min.second * multiplier, glm::uvec2(rand_uint(mt), rand_uint(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::uvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<unsigned int>();
          for(int i = 0; i < data.getVertexCount() * 2; i+=2) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i] == vec[i/2].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::TWO_WIDE][i+1] == vec[i/2].y);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::uvec3
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::uvec3> vec(min.second * multiplier, glm::uvec3(rand_uint(mt), rand_uint(mt), rand_uint(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::uvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<unsigned int>();
          for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i] == vec[i/3].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+1] == vec[i/3].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::THREE_WIDE][i+2] == vec[i/3].z);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
    //glm::uvec4
    for(auto& min : minimum){
      auto multiplier = dist(mt);
      std::vector<glm::uvec4> vec(min.second * multiplier, glm::uvec4(rand_uint(mt), rand_uint(mt), rand_uint(mt), rand_uint(mt)));
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(min.first);
        data.addVec<glm::uvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec);
        THEN("it will have the correct data") {
          auto collapsed = data.getCollapsedVectors<unsigned int>();
          for(int i = 0; i < data.getVertexCount() * 4; i+=4) {
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i] == vec[i/4].x);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+1] == vec[i/4].y);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+2] == vec[i/4].z);
            REQUIRE(collapsed[VertexData::DATA_TYPE::FOUR_WIDE][i+3] == vec[i/4].w);
          }
        }
        THEN("it will have the correct vertex count") {
          REQUIRE(data.getVertexCount() == vec.size());
        }
      }
    }
  }
}

SCENARIO("Correct Vertices added to Vertex Data not divisible by the right amount are rejected") {
  GIVEN("Vectors with 1 greater than the required divisibility for each primitive type and each vertex data type") {
    //float
    for(auto& divide : divisibility){
      std::vector<float> vec(minimum[divide.first] + divide.second + 1, 0.0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<float>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec2
    for(auto& divide : divisibility){
      std::vector<glm::vec2> vec(minimum[divide.first] + divide.second + 1, glm::vec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec3
    for(auto& divide : divisibility){
      std::vector<glm::vec3> vec(minimum[divide.first] + divide.second + 1, glm::vec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::vec4
    for(auto& divide : divisibility){
      std::vector<glm::vec4> vec(minimum[divide.first] + divide.second + 1, glm::vec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::vec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //double
    for(auto& divide : divisibility){
      std::vector<double> vec(minimum[divide.first] + divide.second + 1, 0.0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<double>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec2
    for(auto& divide : divisibility){
      std::vector<glm::dvec2> vec(minimum[divide.first] + divide.second + 1, glm::dvec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec3
    for(auto& divide : divisibility){
      std::vector<glm::dvec3> vec(minimum[divide.first] + divide.second + 1, glm::dvec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::dvec4
    for(auto& divide : divisibility){
      std::vector<glm::dvec4> vec(minimum[divide.first] + divide.second + 1, glm::dvec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::dvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //int
    for(auto& divide : divisibility){
      std::vector<int> vec(minimum[divide.first] + divide.second + 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<int>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec2
    for(auto& divide : divisibility){
      std::vector<glm::ivec2> vec(minimum[divide.first] + divide.second + 1, glm::ivec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec3
    for(auto& divide : divisibility){
      std::vector<glm::ivec3> vec(minimum[divide.first] + divide.second + 1, glm::ivec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec4
    for(auto& divide : divisibility){
      std::vector<glm::ivec4> vec(minimum[divide.first] + divide.second + 1, glm::ivec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::ivec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
    //unsigned int
    for(auto& divide : divisibility){
      std::vector<unsigned int> vec(minimum[divide.first] + divide.second + 1, 0);
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<unsigned int>(VertexData::DATA_TYPE::ONE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::uvec2
    for(auto& divide : divisibility){
      std::vector<glm::uvec2> vec(minimum[divide.first] + divide.second + 1, glm::uvec2());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec2>(VertexData::DATA_TYPE::TWO_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::uvec3
    for(auto& divide : divisibility){
      std::vector<glm::uvec3> vec(minimum[divide.first] + divide.second + 1, glm::uvec3());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec3>(VertexData::DATA_TYPE::THREE_WIDE, vec), std::length_error);
        }
      }
    }
    //glm::ivec4
    for(auto& divide : divisibility){
      std::vector<glm::uvec4> vec(minimum[divide.first] + divide.second + 1, glm::uvec4());
      WHEN("Added to a VertexData configured for the current iter's primitive type") {
        VertexData data(divide.first);
        THEN("it will throw a std::length_error") {
          REQUIRE_THROWS_AS(data.addVec<glm::uvec4>(VertexData::DATA_TYPE::FOUR_WIDE, vec), std::length_error);
        }
      }
    }
  }
}

SCENARIO("Various types of vectors added as vertex data, reflect the correct vertex count") {
  GIVEN("A vertex data object configured for GL_TRIANGLES") {
    VertexData data(GL_TRIANGLES);
    WHEN("two kinds of vectors compatible with GL_TRIANGLES are added") {

      std::vector<glm::vec3> vec1(12, glm::vec3());
      std::vector<glm::uvec2> vec2(12, glm::uvec2());
      data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, vec1);
      data.addVec<glm::uvec2>(VertexData::DATA_TYPE::TEX_COORDS, vec2);
      THEN("Vertex Count should reflect the same count for both of them") {
        REQUIRE(data.getVertexCount() == vec1.size());
        REQUIRE(data.getVertexCount() == vec2.size());
      }

      THEN("the data can be collapsed in the correct way") {
        auto floats = data.getCollapsedVectors<float>();
        auto uints = data.getCollapsedVectors<unsigned int>();

        for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
          REQUIRE(floats[VertexData::DATA_TYPE::GEOMETRY][i] == vec1[i/3].x);
          REQUIRE(floats[VertexData::DATA_TYPE::GEOMETRY][i+1] == vec1[i/3].y);
          REQUIRE(floats[VertexData::DATA_TYPE::GEOMETRY][i+2] == vec1[i/3].z);
        }

        for(int i = 0; i < data.getVertexCount() * 2; i+= 2) {
          REQUIRE(uints[VertexData::DATA_TYPE::TEX_COORDS][i] == vec2[i/2].x);
          REQUIRE(uints[VertexData::DATA_TYPE::TEX_COORDS][i] == vec2[i/2].y);
        }
      }
    }
    WHEN("a vector that is not compatible with GL_TRIANGLES is added") {
      std::vector<int> vec3(11, 5);
      THEN("it will throw a std::length_error") {
        REQUIRE_THROWS_AS(data.addVec<int>(VertexData::DATA_TYPE::ONE_WIDE, vec3), std::length_error);
      }
    }
  }
}

SCENARIO("A VertexData object can only have one set of data for a data type") {
  GIVEN("A VertexData object with data already in it") {
    VertexData data(GL_TRIANGLES);
    std::vector<glm::vec3> vec1(12, glm::vec3());
    data.addVec<glm::vec3>(VertexData::DATA_TYPE::THREE_WIDE, vec1);
    WHEN("another set of data for the same data type is added to it") {
      std::vector<glm::ivec3> vec2(12, glm::ivec3());
      data.addVec<glm::ivec3>(VertexData::DATA_TYPE::THREE_WIDE, vec2);
      THEN("the collapsed data of the old type should be nonexistent") {
        REQUIRE(data.getCollapsedVectors<float>().count(VertexData::DATA_TYPE::THREE_WIDE) == 0);
      }
      THEN("the collapsed data of the new type should be reflected correctly") {
        auto ints = data.getCollapsedVectors<int>();
        for(int i = 0; i < data.getVertexCount() * 3; i+=3) {
          REQUIRE(ints[VertexData::DATA_TYPE::THREE_WIDE][i] == vec2[i/3].x);
          REQUIRE(ints[VertexData::DATA_TYPE::THREE_WIDE][i+1] == vec2[i/3].y);
          REQUIRE(ints[VertexData::DATA_TYPE::THREE_WIDE][i+2] == vec2[i/3].z);
        }
      }
      THEN("the vertex count should be of the new added data") {
        REQUIRE(data.getVertexCount() == vec2.size());
      }
    }
  }
}