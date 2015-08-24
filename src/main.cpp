
#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include <tmx/Tmx.h>
#include <sstream>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/tile_attribute_trait.h"
#include "graphics/shader.h"
#include "graphics/base_texture.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable_factory.h"
#include "graphics/animated_tile.h"
#include "transform.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;

int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  LOG(INFO)<<"Butts";


  Tmx::Map *map = new Tmx::Map();
  map->ParseFile(std::string(argv[1]));

  GraphicsSystem graphics;
  graphics.initialize(1280, 720, "Dick Butts (tm)", Graphics::WindowExitFunctor(), 60.0);

  RenderableFactory renderable_factory;

  ShaderManager shader_manager;
  shader_manager.loadShader("simple_texture");
  shader_manager.loadShader("tile_animation");
  TextureManager texture_manager;
  texture_manager.loadTexture("./project-spero-assets/sprites/Veldes.png", "tileset");
  
  auto renderables = renderable_factory.createFromMap(*map, texture_manager, shader_manager);
  
  auto transform = std::make_shared<Transform>();

  for(auto i : renderables) {
    transform->addChild(i->getTransform());
    i->setActive();
    graphics.addRenderable(i);
  }
  LOG(INFO)<<"Done with renderables";
  transform->translate(glm::vec2(-25 * 32.0, -25 * 32.0));
  transform->scale(glm::vec2(32.0, 32.0));
  glViewport(0, 0, 800, 800);
  graphics.renderLoop();
  graphics.destroy();

  return 0;
}
