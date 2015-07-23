#include <easylogging++.h>
#include <catch.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include <glm/vec3.hpp>
#include <vector>
#include <cstring>
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/renderable_attribute_trait.h"
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/invalid_shader_object_exception.h"

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

void setup_opengl() {
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  auto window = glfwCreateWindow(400, 400, "Renderable test window", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  GLint attribs; 
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribs);
}

Graphics::Renderable setup_renderable() {
  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  Graphics::Renderable renderable(good_binding, Graphics::VertexData(GL_TRIANGLES), new TestAttributeTrait());
  return renderable;
}

std::vector<glm::vec3> setup_random_vertex_data(const int indices) {
  //rand this mofo
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<float> dist(-1.0, 1.0);

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

const unsigned int setup_shader() {
  auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  auto program_object = glCreateProgram();
  
  std::string vertex_code = std::string(R"(
    #version 330

    layout(location = 0)in vec3 vert;

    void main()
    {
        gl_Position = vec4(vert, 1.0);
    })");

   std::string fragment_code = std::string(R"(
    #version 330

    out vec4 fragColor;

    void main()
    {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    })");
    const char *vc_str = vertex_code.c_str();
    const char *fc_str = fragment_code.c_str();
    glShaderSource(vertex_shader, 1, &vc_str, new int[1] {(int)vertex_code.size()});
    glShaderSource(fragment_shader, 1, &fc_str, new int[1] {(int)fragment_code.size()});
    glCompileShader(vertex_shader);
    GLint is_vert_compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_vert_compiled);
    glCompileShader(fragment_shader);
    GLint is_frag_compiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_frag_compiled);
    glAttachShader(program_object, vertex_shader);
    glAttachShader(program_object, fragment_shader);
    glLinkProgram(program_object);
    GLint is_linked = 0;
    glGetProgramiv(program_object, GL_LINK_STATUS, (int *)&is_linked);

    LOG(INFO)<<"Test shader created!";
    
    return program_object;
}

Graphics::Renderable setup_renderable_with_data(bool multiple_vertex_data, bool use_index) {
  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding); 
  glBindVertexArray(good_binding);
  glBindVertexArray(0);

  Graphics::VertexData vertex_data(GL_TRIANGLES);
  std::vector<unsigned int> index_data;
  int num_indices = 12;
  int max = 6;
  if(multiple_vertex_data) {
    vertex_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(max));
    for(int i=0; i<9; i++) {
      vertex_data.addVec<glm::vec3>((Graphics::VertexData::DATA_TYPE)(Graphics::VertexData::DATA_TYPE::RESERVED1 + i), setup_random_vertex_data(max));
    }
  } 
  else {
    vertex_data.addVec<glm::vec3>(Graphics::VertexData::DATA_TYPE::GEOMETRY, setup_random_vertex_data(max));
  }
  if(use_index) {
    vertex_data.addIndices(setup_random_index_data(num_indices, max));
  }

  Graphics::Renderable renderable = Graphics::Renderable(good_binding, vertex_data, new TestAttributeTrait());
  renderable.setActive();
  return renderable;
}

Graphics::Renderable setup_initialized_renderable(bool multiple_vertex_data, bool use_index) {
  Graphics::Renderable renderable = setup_renderable_with_data(multiple_vertex_data, use_index);
  renderable.setShader(setup_shader());
  renderable.initialize();
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
        auto construct = [&]() {Graphics::Renderable renderable(shit_binding, Graphics::VertexData(GL_TRIANGLES), new TestAttributeTrait());};
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
      Graphics::Renderable renderable(good_binding, GL_TRIANGLES, new TestAttributeTrait());
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
    Graphics::Renderable renderable = setup_renderable_with_data(false, false);
    WHEN("initialize is called") {
      renderable.initialize();
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
  GIVEN("a renderable and multiple sets of well-formed vertex data") {
    Graphics::Renderable renderable = setup_renderable_with_data(true, false);
    WHEN("initialize is called") {
      renderable.initialize();
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED1)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED2)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED3)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED4)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED5)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED6)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED7)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED8)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED9)));
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
    Graphics::Renderable renderable = setup_renderable_with_data(false, true);
    WHEN("initialize is called") {
      renderable.initialize();
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
    Graphics::Renderable renderable = setup_renderable_with_data(true, true);
    WHEN("initialize is called") {
      renderable.initialize();
      THEN("isInitialized should return true") {
        REQUIRE(renderable.isInitialized() == true);
      }
      THEN("it should have valid vertex buffer bindings") {
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::GEOMETRY)));
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED1))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED2))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED3))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED4))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED5))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED6))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED7))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED8))); 
        REQUIRE(glIsBuffer(renderable.getVertexBufferBinding(Graphics::VertexData::DATA_TYPE::RESERVED9))); 
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

SCENARIO("A renderable can be given a valid shader object handle") {
  setup_opengl();
  GIVEN("A constructed renderable") {
    Graphics::Renderable renderable = setup_renderable();
    WHEN("setShader is called with a completely valid shader object") {
      unsigned int shader_object = setup_shader();
      renderable.setShader(shader_object);
      THEN("the renderable should contain the shader object as it was passed") {
        REQUIRE(renderable.getShaderBinding() == shader_object);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("A renderable cannot be given an invalid shader object handle") {
  setup_opengl();
  GIVEN("A constructed renderable") {
    Graphics::Renderable renderable = setup_renderable();
    unsigned int shader_object = 840399;
    WHEN("setShader is called with an invalid shader object") {
      THEN("it should throw a InvalidShaderObject exception") {
        REQUIRE_THROWS_AS(renderable.setShader(shader_object), Exceptions::InvalidShaderObjectException);
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
