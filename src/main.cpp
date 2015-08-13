
#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
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

  GraphicsSystem graphics;
  graphics.initialize(800, 800, "Dick Butts (tm)", Graphics::WindowExitFunctor(), 60.0);

  RenderableFactory renderable_factory;

  ShaderManager shader_manager;
  shader_manager.loadShader("simple_texture");
  shader_manager.loadShader("tile_animation");
  std::shared_ptr<BaseTexture> texture_ptr = std::make_shared<BaseTexture>("tileset", GL_TEXTURE_2D, 0);
  texture_ptr->load("./project-spero-assets/sprites/Leon2.png");
  auto animated_tile_ptr = renderable_factory.create<AnimatedTile>();
  auto tile_ptr = renderable_factory.create<Tile>();
  try {
    animated_tile_ptr->setShader(shader_manager["tile_animation"]);
    tile_ptr->setShader(shader_manager["simple_texture"]);
  }
  catch(std::exception& e) {
    LOG(ERROR)<<e.what();
    return 0;
  }
  animated_tile_ptr->setSizeInPixels(32);
  animated_tile_ptr->addFrameBack(glm::ivec2(0, 3), 400);
  animated_tile_ptr->addFrameBack(glm::ivec2(1, 3), 400);
  animated_tile_ptr->addFrameBack(glm::ivec2(2, 3), 400);
  animated_tile_ptr->addFrameBack(glm::ivec2(1, 3), 400);
  animated_tile_ptr->setTexture(texture_ptr);
  tile_ptr->setTexture(texture_ptr);
  animated_tile_ptr->initialize();
  tile_ptr->initialize();
  animated_tile_ptr->setActive();
  tile_ptr->setActive();
  graphics.addRenderable(animated_tile_ptr);
  //graphics.addRenderable(tile_ptr);
  //std::shared_ptr<Transform> transform = std::make_shared<Transform>();
  //transform->rotate(45., glm::vec3(0.0, 0.0, 1.0));
  //transform->translate(glm::vec3(-0.3, 0.3, 0.0));
  //transform->scale(glm::vec2(0.3, 1.3));
  //animated_tile_ptr->setTransform(transform);
  glViewport(0, 0, 800, 800);
  graphics.renderLoop();
  graphics.destroy();

  return 0;
}
