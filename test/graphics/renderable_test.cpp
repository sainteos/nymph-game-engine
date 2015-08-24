#include <easylogging++.h>
#include <catch.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include <glm/vec3.hpp>
#include <vector>
#include <cstring>
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/shader.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable_factory.h"
#include "graphics/renderable_attribute_trait.h"
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/invalid_shader_object_exception.h"
#include "opengl_setup.h"

class TestAttributeTrait : public Graphics::RenderableAttributeTrait {
  private:
    std::map<Graphics::VertexData::DATA_TYPE, unsigned int> trait;
  public:
    TestAttributeTrait() {
      trait = std::map<Graphics::VertexData::DATA_TYPE, unsigned int> {
        {Graphics::VertexData::DATA_TYPE::GEOMETRY, 0},
        {Graphics::VertexData::DATA_TYPE::TEX_COORDS, 1},
        {Graphics::VertexData::DATA_TYPE::NORMAL_COORDS, 2},
        {Graphics::VertexData::DATA_TYPE::RESERVED1, 3},
        {Graphics::VertexData::DATA_TYPE::RESERVED2, 4},
        {Graphics::VertexData::DATA_TYPE::RESERVED3, 5},
        {Graphics::VertexData::DATA_TYPE::RESERVED4, 6},
        {Graphics::VertexData::DATA_TYPE::RESERVED5, 7},
        {Graphics::VertexData::DATA_TYPE::RESERVED6, 8},
        {Graphics::VertexData::DATA_TYPE::RESERVED7, 9},
        {Graphics::VertexData::DATA_TYPE::RESERVED8, 10},
        {Graphics::VertexData::DATA_TYPE::RESERVED9, 11},
        {Graphics::VertexData::DATA_TYPE::ONE_WIDE, 12},
        {Graphics::VertexData::DATA_TYPE::TWO_WIDE, 13},
        {Graphics::VertexData::DATA_TYPE::THREE_WIDE, 14},
        {Graphics::VertexData::DATA_TYPE::FOUR_WIDE, 15}
      };
    }

    virtual const std::map<Graphics::VertexData::DATA_TYPE, unsigned int> operator()() const noexcept override {
      return trait;
    }

};

std::shared_ptr<Graphics::Renderable> setup_renderable() {
  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  std::shared_ptr<Graphics::Renderable> renderable = std::make_shared<Graphics::Renderable>(good_binding, Graphics::VertexData(GL_TRIANGLES), new TestAttributeTrait);
  return renderable;
}

std::vector<glm::vec3> setup_random_vertex_data(const int size) {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1.0, 1.0);

  std::vector<glm::vec3> out_vec;
  for(int i = 0; i < size; i++) {
    out_vec.push_back(glm::vec3(dist(mt), dist(mt), 0.0));
  }

  return out_vec;
}

std::vector<unsigned int> setup_random_index_data(const int num_indices, const int max) {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<unsigned int> dist(0, max - 1);

  std::vector<unsigned int> index_vec;
  for(int i = 0; i < num_indices; i++) {
    index_vec.push_back(dist(mt));
  }

  return index_vec;
}

std::shared_ptr<Graphics::Shader> setup_shader() {
  Graphics::ShaderManager manager;
  manager.loadShader("test_shader");
  return manager["test_shader"];
}

std::shared_ptr<Graphics::Renderable> setup_renderable_with_data(bool multiple_vertex_data, bool use_index) {
  Graphics::RenderableFactory factory;

  Graphics::VertexData vertex_data(GL_TRIANGLES);
  std::vector<unsigned int> index_data;
  int num_indices = 6;
  int num_vertices = 6;
  if(use_index) {
    vertex_data.addIndices(setup_random_index_data(num_indices, num_vertices));
  }
  if(multiple_vertex_data) {
    vertex_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(num_vertices));
    for(int i=0; i<9; i++) {
      vertex_data.addVec<glm::vec3>((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i), setup_random_vertex_data(num_vertices));
    }
  } 
  else {
    vertex_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(num_vertices));
  }

  std::shared_ptr<Graphics::Renderable> renderable = factory.create<Graphics::Renderable>(vertex_data);
  renderable->setActive();
  return renderable;
}

std::shared_ptr<Graphics::Renderable> setup_initialized_renderable(bool multiple_vertex_data, bool use_index) {
  std::shared_ptr<Graphics::Renderable> renderable = setup_renderable_with_data(multiple_vertex_data, use_index);
  renderable->setShader(setup_shader());
  return renderable;
}

SCENARIO("A renderable can't be constructed with an invalid vertex array object", "[renderable]") {
  setup_opengl();
  GIVEN("an invalid vertex array object binding") {
    unsigned int shit_binding = 5;
    WHEN("a renderable is constructed with it") {
      THEN("an invalid vertex array exception is thrown") {
        //gotta lambda that motherfucker
        auto construct = [&]() {Graphics::Renderable renderable(shit_binding, Graphics::VertexData(GL_TRIANGLES), new TestAttributeTrait);};
        REQUIRE_THROWS_AS(construct(), Exceptions::InvalidVertexArrayException);
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be constructed with a valid vertex array object", "[renderable]") {
  setup_opengl();
  GIVEN("a valid vertex array object binding") {
    unsigned int good_binding;
    glGenVertexArrays(1, &good_binding); 
    glBindVertexArray(good_binding);
    glBindVertexArray(0);
    WHEN("a renderable is constructed with it") {
      Graphics::Renderable renderable(good_binding, GL_TRIANGLES, new TestAttributeTrait);
      THEN("the renderable is successfully created, and has the correct vertex array object handle") {
        REQUIRE(renderable.getVertexArrayBinding() == good_binding);
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be given a valid shader object handle") {
  setup_opengl();
  GIVEN("A constructed renderable") {
    std::shared_ptr<Graphics::Renderable> renderable = setup_renderable();
    WHEN("setShader is called with a completely valid shader object") {
      auto shader_object = setup_shader();
      renderable->setShader(shader_object);
      THEN("the renderable's shader should be a valid program object") {
        REQUIRE(glIsProgram(renderable->getShader()->getHandle()));
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with one set of vertex data and no index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with one set of vertex data and no index data") {
    std::shared_ptr<Graphics::Renderable> renderable = setup_initialized_renderable(false, false);
    WHEN("onRender is called") {
      renderable->onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable->getVertexArrayBinding());
      }
      THEN("the current shader program binding is the same as the renderable's") {
        REQUIRE([]() {int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); return id;}() == renderable->getShader()->getHandle());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with multiple sets of vertex data and no index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with multiple sets of vertex data and no index data") {
    std::shared_ptr<Graphics::Renderable> renderable = setup_initialized_renderable(true, false);
    WHEN("onRender is called") {
      renderable->onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable->getVertexArrayBinding());
      }
      THEN("the current shader program binding is the same as the renderable's") {
        REQUIRE([]() {int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); return id;}() == renderable->getShader()->getHandle());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with one set of vertex data and index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with one set of vertex data and index data") {
    std::shared_ptr<Graphics::Renderable> renderable = setup_initialized_renderable(false, true);
    WHEN("onRender is called") {
      renderable->onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable->getVertexArrayBinding());
      }
      THEN("the current shader program binding is the same as the renderable's") {
        REQUIRE([]() {int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); return id;}() == renderable->getShader()->getHandle());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with multiple sets of vertex data and index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with multiple sets of vertex data and index data") {
    std::shared_ptr<Graphics::Renderable> renderable = setup_initialized_renderable(true, true);
    WHEN("onRender is called") {
      renderable->onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable->getVertexArrayBinding());
      }
      THEN("the current shader program binding is the same as the renderable's") {
        REQUIRE([]() {int id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); return id;}() == renderable->getShader()->getHandle());
      }
    }
  }
  destroy_opengl();
}
