#include <easylogging++.h>
#include <catch.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include <glm/vec3.hpp>
#include <vector>
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "exceptions/invalid_vertex_array_exception.h"

void setup_opengl() {
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  auto window = glfwCreateWindow(400, 400, "Renderable test window", nullptr, nullptr);
  glfwMakeContextCurrent(window);
}

Graphics::Renderable setup_renderable() {
  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  Graphics::Renderable renderable(good_binding);
  return renderable;
}

std::vector<glm::vec3> setup_random_vertex_data(const int indices) {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(-1.0, 1.0);

  std::vector<glm::vec3> out_vec;
  for(int i = 0; i < indices; i++) {
    out_vec.push_back(glm::vec3(dist(mt), dist(mt), 0.0));
  }

  return out_vec;
}

std::vector<unsigned int> setup_random_index_data(const int indices, const int max) {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<unsigned int> dist(0, max);

  std::vector<unsigned int> index_vec;
  for(int i = 0; i < indices; i++) {
    index_vec.push_back(dist(mt));
  }

  return index_vec;
}

Graphics::Renderable setup_initialized_renderable(bool multiple_vertex_data, bool use_index) {
  Graphics::Renderable renderable = setup_renderable();
  Graphics::VertexData vertex_data;
  std::vector<unsigned int> index_data;
  int num_indices = 10;
  int max = 15;
  if(multiple_vertex_data) {
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(num_indices));
    for(int i=0; i<9; i++) {
      vertex_data.addFloatVec3((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i), setup_random_vertex_data(num_indices));
    }
  } 
  else {
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(num_indices));
  }
  if(use_index) {
    vertex_data.addIndices(setup_random_index_data(num_indices, max));
  }
  renderable.initialize(vertex_data);
  renderable.setActive();
  return renderable;
}

void destroy_opengl() {
  glfwTerminate();
}

SCENARIO("A renderable can't be constructed with an invalid vertex array object", "[renderable]") {
  setup_opengl();
  GIVEN("an invalid vertex array object binding") {
    unsigned int shit_binding = 5;
    WHEN("a renderable is constructed with it") {
      THEN("an invalid vertex array exception is thrown") {
        //gotta lambda that motherfucker
        auto construct = [&]() {Graphics::Renderable renderable(shit_binding);};
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
      Graphics::Renderable renderable(good_binding);
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

SCENARIO("A renderable can be initialized with one set of vertex data and no index data", "[renderable]") {
  setup_opengl();
  GIVEN("a renderable and well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(6));
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("the renderable should have a valid vertex buffer binding") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be initialized with multiple sets of vertex data and no index data", "[renderable]") {
  setup_opengl();
  GIVEN("a renderable and 2 sets of well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(6));
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::RESERVED1, setup_random_vertex_data(6));
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED1)));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }

  GIVEN("a renderable and 9 sets of well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    for(int i = 0; i < 9; i++)
      vertex_data.addFloatVec3((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i), setup_random_vertex_data(6));
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        for(int i = 0; i < 9; i++)
          REQUIRE(glIsBuffer(renderable.getVertexBufferBinding((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i))));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be initialized with index data and one set of vertex data", "[renderable]") {
  setup_opengl();
  GIVEN("a renderable and well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    std::vector<unsigned int> index_data = setup_random_index_data(10, 6);
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(6));
    vertex_data.addIndices(index_data);
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have a valid vertex buffer binding") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
      }
      THEN("it should have a valid index buffer binding") {
        REQUIRE(glIsBuffer(renderable.getIndexBufferBinding()));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable can be initialized with index data and multiple sets of vertex data", "[renderable]") {
  setup_opengl();
  GIVEN("a renderable and 2 sets of well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    std::vector<unsigned int> index_data = setup_random_index_data(10, 6);
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(6));
    vertex_data.addFloatVec3(Graphics::VertexData::DATA_TYPE::RESERVED1, setup_random_vertex_data(6));
    vertex_data.addIndices(index_data);
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED1))); 
      }
      THEN("it should have a valid index buffer binding") {
        REQUIRE(glIsBuffer(renderable.getIndexBufferBinding()));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }

  GIVEN("a renderable and 9 sets of well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable();
    Graphics::VertexData vertex_data;
    std::vector<unsigned int> index_data = setup_random_index_data(10, 6);
    for(int i = 0; i < 9; i++)
      vertex_data.addFloatVec3((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i), setup_random_vertex_data(6));
    vertex_data.addIndices(index_data);
    WHEN("initialize is called") {
      renderable.initialize(vertex_data);
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        for(int i = 0; i < 9; i++)
          REQUIRE(glIsBuffer(renderable.getVertexBufferBinding((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i))));
      }
      THEN("it should have a valid index buffer binding") {
        REQUIRE(glIsBuffer(renderable.getIndexBufferBinding()));
      }
      THEN("there should be no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with one set of vertex data and no index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with one set of vertex data and no index data") {
    Graphics::Renderable renderable = setup_initialized_renderable(false, false);
    WHEN("onRender is called") {
      renderable.onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable.getVertexArrayBinding());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with multiple sets of vertex data and no index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with multiple sets of vertex data and no index data") {
    Graphics::Renderable renderable = setup_initialized_renderable(true, false);
    WHEN("onRender is called") {
      renderable.onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable.getVertexArrayBinding());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with one set of vertex data and index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with one set of vertex data and index data") {
    Graphics::Renderable renderable = setup_initialized_renderable(false, true);
    WHEN("onRender is called") {
      renderable.onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable.getVertexArrayBinding());
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable with multiple sets of vertex data and index data can be rendered", "[renderable]") {
  setup_opengl();
  GIVEN("an initialized renderable with multiple sets of vertex data and index data") {
    Graphics::Renderable renderable = setup_initialized_renderable(true, true);
    WHEN("onRender is called") {
      renderable.onRender();
      THEN("there is no opengl error") {
        REQUIRE(glGetError() == GL_NO_ERROR);
      }
      THEN("the current verrex array object binding is the same as the renderable's") {
        int vertex_array_binding;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array_binding);
        REQUIRE(vertex_array_binding == renderable.getVertexArrayBinding());
      }
    }
  }
  destroy_opengl();
}
