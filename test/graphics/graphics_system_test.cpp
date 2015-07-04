#include <catch.hpp>
#include <iostream>
#include <random>
#include <thread>
#include "graphics/graphics_system.h"
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"

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
  GIVEN("a recently initialized graphics system object") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");

    REQUIRE(graphics_system.isInitialized() == true);

    WHEN("start is called") {
      std::thread::id thread_id = graphics_system.start();

      THEN("a graphics update loop should start in a separate thread") {
        REQUIRE(thread_id != std::this_thread::get_id());
      }

      THEN("isRunning returns true") {
        REQUIRE(graphics_system.isRunning() == true);
      }
    }
  }
  
  GIVEN("an already started graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.start();
    WHEN("start is called") {
      THEN("a SystemAlreadyRunning exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.start(), Exceptions::SystemAlreadyRunningException);
      }
    }
  }

  GIVEN("a stopped graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.start();
    graphics_system.stop();
    WHEN("start is called") {
      std::thread::id thread_id = graphics_system.start();
      THEN("a graphics update loop should start in a separate thread from the main thread") {
        REQUIRE(thread_id != std::this_thread::get_id());
      }

      THEN("isRunning returns true") {
        REQUIRE(graphics_system.isRunning() == true);
      }
    }
  }

  GIVEN("an uninitialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("start is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.start(), Exceptions::SystemNotInitializedException);
      }

      THEN("isRunning returns false") {
        REQUIRE(graphics_system.isRunning() == false);
      }
    }
  }

  GIVEN("a destroyed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.start();
    graphics_system.destroy();
    WHEN("start is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.start(), Exceptions::SystemNotInitializedException);
      }

      THEN("isRunning returns false") {
        REQUIRE(graphics_system.isRunning() == false);
      }
    }
  }
}

SCENARIO("the graphics system can be stopped", "[graphics]") {
  GIVEN("a recently initialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");

    WHEN("the graphics system has been started") {
      graphics_system.start();
      REQUIRE(graphics_system.isRunning() == true);

      WHEN("stop is called") {
        graphics_system.stop();
        THEN("isRunning returns false") {
          REQUIRE(graphics_system.isRunning() == false);
        }
      }
    }

    WHEN("stop is called") {
      THEN("a SystemNotRunning exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.stop(), Exceptions::SystemNotRunningException);
      }
    }
  }
  
  GIVEN("an uninitialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("stop is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.stop(), Exceptions::SystemNotInitializedException);
      }
    }
  }

  GIVEN("a stopped graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.start();
    graphics_system.stop();
    WHEN("stop is called") {
      THEN("a SystemNotRunning exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.stop(), Exceptions::SystemNotRunningException);
      }
    }
  }

  GIVEN("a destroyed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.destroy();
    WHEN("stop is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.stop(), Exceptions::SystemNotInitializedException);
      }
    }
  }
}

SCENARIO("the graphics system can be destroyed", "[graphics]") {
  GIVEN("a recently initialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    WHEN("destroy is called") {
      graphics_system.destroy();
      THEN("isInitialized is false") {
        REQUIRE(graphics_system.isInitialized() == false);
      }
    }
  }

  GIVEN("a started graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.start();
    WHEN("destroy is called") {
      graphics_system.destroy();
      THEN("isInitialized is false") {
        REQUIRE(graphics_system.isInitialized() == false);
      }
      THEN("isRunning is false") {
        REQUIRE(graphics_system.isRunning() == false);
      }
    }
  }

  GIVEN("an uninitialzed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("destroy is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.destroy(), Exceptions::SystemNotInitializedException);
      }
    }
  }

  GIVEN("a destroyed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window");
    graphics_system.destroy();
    WHEN("destroy is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.destroy(), Exceptions::SystemNotInitializedException);
      }
    }
  }
}

SCENARIO("the graphics system can have objects for rendering added to it", "[graphics]") {

}

SCENARIO("the graphics system can have objects for rendering removed from it", "[graphics]") {

}

SCENARIO("the window's width and height can be retrieved") {

}