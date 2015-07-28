#include <catch.hpp>
#include "opengl_setup.h"
#include "graphics/shader_manager.h"
#include "exceptions/invalid_filename_exception.h"
#include "exceptions/invalid_shader_name_exception.h"

SCENARIO("One shader can be loaded") {
  setup_opengl();
  GIVEN("A constructed shader manager") {
    Graphics::ShaderManager manager;
    WHEN("loadShader is called with a valid name") {
      bool success = manager.loadShader("test_shader");
      THEN("it should return true for sucess") {
        REQUIRE(success == true);
      }
      THEN("it should be able to be gotten by square brackets") {
        REQUIRE(manager["test_shader"] != nullptr);
        REQUIRE_NOTHROW(manager["test_shader"]);
      }
      THEN("it should be able to be gotten by getShader") {
        REQUIRE(manager.getShader("test_shader") != nullptr);
        REQUIRE_NOTHROW(manager["test_shader"]);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("Two shaders can be loaded") {
  setup_opengl();
  GIVEN("A constructed shader manager") {
    Graphics::ShaderManager manager;
    WHEN("loadShader is called with valid shader filenames") {
      bool success1 = manager.loadShader("test_shader", "test_shader.vert", "test_shader.frag", "test_shader.geom");
      bool success2 = manager.loadShader("test_shader2", "test_shader2.vert", "test_shader2.frag", "");
      THEN("it should return true for sucess") {
        REQUIRE(success1 == true);
        REQUIRE(success2 == true);
      }
      THEN("it should be able to be gotten by square brackets") {
        REQUIRE(manager["test_shader"] != nullptr);
        REQUIRE_NOTHROW(manager["test_shader"]);
        REQUIRE(manager["test_shader2"] != nullptr);
        REQUIRE_NOTHROW(manager["test_shader2"]);
      }
      THEN("it should be able to be gotten by getShader") {
        REQUIRE(manager.getShader("test_shader") != nullptr);
        REQUIRE_NOTHROW(manager["test_shader"]);
        REQUIRE(manager.getShader("test_shader2") != nullptr);
        REQUIRE_NOTHROW(manager["test_shader2"]);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("loadShader can't be called with invalid filenames") {
  setup_opengl();
  GIVEN("A constructed shader manager") {
    Graphics::ShaderManager manager;
    WHEN("loadShader is called with an invalid name") {
      THEN("it should throw an InvalidFilename exception") {
        REQUIRE_THROWS_AS(manager.loadShader("asdfasdfasdf"), Exceptions::InvalidFilenameException);
      }
    }
  }
  destroy_opengl();
}

SCENARIO("getShader and [] brackets with an invalid name") {
  setup_opengl();
  GIVEN("A constructed shader manager") {
    Graphics::ShaderManager manager;
    WHEN("getShader is called with a name") {
      THEN("it should throw an InvalidShaderName exception") {
        REQUIRE_THROWS_AS(manager.getShader("asdfasdfasdf"), Exceptions::InvalidShaderNameException);
      }
    }
    WHEN("[] is called with a name") {
      THEN("it should throw an InvalidShaderName exception") {
        REQUIRE_THROWS_AS(manager.getShader("asdfasdfasdf"), Exceptions::InvalidShaderNameException);
      }
    }
  }
  destroy_opengl();
}