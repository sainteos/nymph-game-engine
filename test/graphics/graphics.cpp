#include <catch.hpp>
#include <random>
#include "graphics/graphics_system.h"
#include "exceptions/system_already_initialized_exception.h"

SCENARIO("the graphics system can be initialized", "[graphics]") {
  //Setup rand
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(200, 500);

  GIVEN("a newly created graphics system object") {
    Graphics::GraphicsSystem graphics_system;
    REQUIRE(graphics_system.isInitialized() == false);

    WHEN("initialize is called") {
      int width = dist(mt);
      int height = dist(mt);
      std::string name = "test_window";

      graphics_system.initialize(width, height, name);

      THEN("a window context is created with the correct size") {
        REQUIRE(graphics_system.windowWidth() == width);
        REQUIRE(graphics_system.windowHeight() == height);
      }

      THEN("a window context is created with the correct name") {
        REQUIRE(graphics_system.windowName() == name);
      }

      THEN("isInitialized returns true") {
        REQUIRE(graphics_system.isInitialized() == true);
      }

      WHEN("initialize is called again") {
        THEN("a SystemAlreadyInitialized exception is thrown") {
          REQUIRE_THROWS_AS(graphics_system.initialize(width, height, name), Exceptions::SystemAlreadyInitializedException);
        }
      }
    }
  }
}

SCENARIO("the graphics system can be started", "[graphics]") {
  
}

SCENARIO("the graphics system can be stopped", "[graphics]") {
  
}