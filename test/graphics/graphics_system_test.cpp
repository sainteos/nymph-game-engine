#include <easylogging++.h>
#include <catch.hpp>
#include <iostream>
#include <random>
#include <thread>
#include <memory>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/tile_attribute_trait.h"
#include "exceptions/system_already_initialized_exception.h"
#include "exceptions/system_not_initialized_exception.h"
#include "exceptions/system_already_running_exception.h"
#include "exceptions/system_not_running_exception.h"
#include "exceptions/invalid_vertex_array_exception.h"

//I'm lazy
using RenderablePtr = std::shared_ptr<Graphics::Renderable>;

RenderablePtr constructRenderable() {
  unsigned int good_binding;
  glGenVertexArrays(1, &good_binding);
  glBindVertexArray(good_binding);
  RenderablePtr renderable = std::make_shared<Graphics::Renderable>(good_binding, Graphics::VertexData(GL_TRIANGLES));
  glBindVertexArray(0);
  return renderable;
}

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

      graphics_system.initialize(width, height, name, Graphics::WindowExitFunctor());

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
          REQUIRE_THROWS_AS(graphics_system.initialize(width, height, name, Graphics::WindowExitFunctor()), Exceptions::SystemAlreadyInitializedException);
        }
      }
    }
  }
}

SCENARIO("the graphics system can be destroyed", "[graphics]") {
  GIVEN("a recently initialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    WHEN("destroy is called") {
      graphics_system.destroy();
      THEN("isInitialized is false") {
        REQUIRE(graphics_system.isInitialized() == false);
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
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    graphics_system.destroy();
    WHEN("destroy is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.destroy(), Exceptions::SystemNotInitializedException);
      }
    }
  }
}

SCENARIO("the graphics system can have objects for rendering added to it", "[graphics]") {
  GIVEN("an initialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());

    WHEN("addRenderable is called") {
      RenderablePtr renderable = constructRenderable();
      int id = graphics_system.addRenderable(renderable);
      THEN("a non-negative id is returned") {
        REQUIRE(id > 0);
      }

      THEN("the number of renderables is 1") {
        REQUIRE(graphics_system.renderablesCount() == 1);
      }
    }
  }

  GIVEN("an uninitialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("a renderable is constructed") {
      THEN("a InvalidVertexArray exception is thrown") {
        REQUIRE_THROWS_AS(constructRenderable(), Exceptions::InvalidVertexArrayException);
      }
    }
  }

  GIVEN("a destroyed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    graphics_system.destroy();
    
    WHEN("a renderable is constructed") {
      THEN("a InvalidVertexArray exception is thrown") {
        REQUIRE_THROWS_AS(constructRenderable(), Exceptions::InvalidVertexArrayException);
      }
    }
  }

  GIVEN("a graphics system with an object already in it") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    RenderablePtr renderable = constructRenderable();
    RenderablePtr renderable2 = constructRenderable();
    int id = graphics_system.addRenderable(renderable);
    WHEN("addRenderable is called") {
      int id2 = graphics_system.addRenderable(renderable2);
      THEN("a non-negative id is returned") {
        REQUIRE(id2 > 0);
      }

      THEN("the two renderables can't have the same id") {
        REQUIRE(id != id2);
      }

      THEN("the number of renderables is 2") {
        REQUIRE(graphics_system.renderablesCount() == 2);      
      }
    }
  }

  GIVEN("a graphics system with 10000 objects already in it") {
    Graphics::GraphicsSystem graphics_system;
    std::set<int> ids;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    for(int i = 0; i < 10000; i++) {
      int id = graphics_system.addRenderable(constructRenderable());
      ids.insert(id);
    }
    
    RenderablePtr renderable(constructRenderable());
    WHEN("addRenderable is called") {
      int id = graphics_system.addRenderable(renderable);
      THEN("a non-negative id is returned") {
        REQUIRE(id > 0);
      }
      
      THEN("the renderable's id can't be the same as the other 10000") {
        REQUIRE(ids.count(id) == 0);
      }

      THEN("the number of renderables is 10001") {
        REQUIRE(graphics_system.renderablesCount() == 10001);   
      }
    }
  }
}

SCENARIO("the graphics system can have objects for rendering removed from it", "[graphics]") {
  GIVEN("an initialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());

    WHEN("removeRenderable is called") {
      bool removed = graphics_system.removeRenderable(1);
      THEN("it returns false") {
        REQUIRE(removed == false);
      }
      THEN("the number of renderables is 0") {
        REQUIRE(graphics_system.renderablesCount() == 0);
      }
    }
  }

  GIVEN("an uninitialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("removeRenderable is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.removeRenderable(10), Exceptions::SystemNotInitializedException);
      }
    }
  }

  GIVEN("a destroyed graphics system") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    graphics_system.destroy();
    WHEN("removeRenderable is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.removeRenderable(1), Exceptions::SystemNotInitializedException);
      }
    }
  }

  GIVEN("a graphics system with an object already in it") {
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    RenderablePtr renderable = constructRenderable();
    int id = graphics_system.addRenderable(renderable);

    WHEN("removeRenderable is called") {
      WHEN("a negative number is supplied as id") {
        THEN("a std::out_of_range error is thrown") {
          REQUIRE_THROWS_AS(graphics_system.removeRenderable(-1), std::out_of_range);
        }
      }
      WHEN("a wrong number is supplied as id") {
        bool removed = graphics_system.removeRenderable(id + 1);
        THEN("it returns false") {
          REQUIRE(removed == false);
        }
        THEN("the number of renderables is 1") {
          REQUIRE(graphics_system.renderablesCount() == 1);
        }
      }
      WHEN("a correct number is suppled as id") {
        bool removed = graphics_system.removeRenderable(id);
        THEN("it returns true") {
          REQUIRE(removed == true);
        }
        THEN("the number of renderables is 0") {
          REQUIRE(graphics_system.renderablesCount() == 0);
        }
      }
    }
  }

  GIVEN("a graphics system with 10000 objects already in it") {
    Graphics::GraphicsSystem graphics_system;
    std::set<int> ids;
    graphics_system.initialize(300, 400, "test_window", Graphics::WindowExitFunctor());
    for(int i = 0; i < 10000; i++) {
      int id = graphics_system.addRenderable(constructRenderable());
      ids.insert(id);
    }
    WHEN("removeRenderable is called") {
      WHEN("a negative number is supplied as id") {
        THEN("a std::out_of_range error is thrown") {
          REQUIRE_THROWS_AS(graphics_system.removeRenderable(-1), std::out_of_range);
        }
      }
      WHEN("a wrong number is supplied as id") {
        bool removed = graphics_system.removeRenderable(10002);
        THEN("it returns false") {
          REQUIRE(removed == false);
        }
        THEN("the number of renderables is 10000") {
          REQUIRE(graphics_system.renderablesCount() == 10000);
        }
      }
      WHEN("a correct number is supplied as id") {
        bool removed = graphics_system.removeRenderable(1);
        THEN("it returns true") {
          REQUIRE(removed == true);
        }

        THEN("the number of renderables is 9999") {
          REQUIRE(graphics_system.renderablesCount() == 9999);
        }
      }
    }
  }
}

SCENARIO("the window's width and height can be retrieved") {
  //Setup rand
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(200, 500);
  GIVEN("an initialized graphics system") {
    int width = dist(mt);
    int height = dist(mt);
    std::string name = "test_window";
    Graphics::GraphicsSystem graphics_system;
    graphics_system.initialize(width, height, name, Graphics::WindowExitFunctor());

    WHEN("windowWidth is called") {
      THEN("the window's set width is returned") {
        REQUIRE(graphics_system.windowWidth() == width);
      }
    }

    WHEN("windowHeight is called") {
      THEN("the window's set height is returned") {
        REQUIRE(graphics_system.windowHeight() == height);
      }
    }
  }
  
  GIVEN("an uninitialized graphics system") {
    Graphics::GraphicsSystem graphics_system;
    WHEN("windowWidth is called") {
      THEN("a SystemNotInitialized exception is thrown") {
        REQUIRE_THROWS_AS(graphics_system.windowWidth(), Exceptions::SystemNotInitializedException);
      }
    }

    WHEN("windowHeight is called") {
      THEN("a SystemNotInitialized excpetion is thrown") {
        REQUIRE_THROWS_AS(graphics_system.windowHeight(), Exceptions::SystemNotInitializedException);        
      }
    }
  }
}
