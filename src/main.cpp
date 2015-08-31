#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <OpenGL/gl3.h>
#include <glfw3.h>
#include <tmx/Tmx.h>
#include <sstream>
#include <cstdlib>
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
#include "graphics/camera.h"
#include "sprite.h"
#include "input/input_system.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;

int main(int argc, char** argv) { 
  if(argc < 4) {
    std::cout<<"USAGE:"<<std::endl;
    std::cout<<"./TileEngine.out [map name with path] [camera x in tiles] [camera y in tiles]"<<std::endl;
    return 0;
  }

  START_EASYLOGGINGPP(argc, argv);

  Tmx::Map *map = new Tmx::Map();
  map->ParseFile(std::string(argv[1]));

  GraphicsSystem graphics;
  graphics.initialize(1280, 720, "Dick Butts (tm)", Graphics::WindowExitFunctor(), 60.0);

  Input::InputSystem input_system(graphics.getWindow());

  RenderableFactory renderable_factory;

  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();
  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");;

  float viewport_tile_width = 21.0;
  float viewport_tile_height = 12.0;

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, 0.1f, 40.0f);
  graphics.setCamera(camera);
  camera->getTransform()->translate(glm::vec2(atof(argv[2]), atof(argv[3])));
  TextureManager texture_manager;
  
  auto renderables = renderable_factory.createFromMap(*map, texture_manager, shader_manager);
  
  auto transform = std::make_shared<Transform>();

  for(auto i : renderables.tiles) {
    transform->addChild(i->getTransform());
    graphics.addRenderable(i);
  }

  transform->translate(glm::vec2(-map->GetWidth() / 2.0, -map->GetHeight() / 2.0));
  graphics.addRenderable(renderables.animated_tiles.back());

  std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
  input_system.addObserver(sprite);
  sprite->addTile("move up", renderables.animated_tiles.back());
  sprite->addTile("move down", renderables.animated_tiles.back());
  sprite->addTile("move left", renderables.animated_tiles.back());
  sprite->addTile("move right", renderables.animated_tiles.back());
  sprite->addTile("stop up", renderables.animated_tiles.back());
  sprite->addTile("stop down", renderables.animated_tiles.back());
  sprite->addTile("stop left", renderables.animated_tiles.back());
  sprite->addTile("stop right", renderables.animated_tiles.back());
  sprite->setMovingSpeed(2.0);
  transform->addChild(sprite->getTransform());

  sprite->onStart();
  graphics.startRender();
  
  while(graphics.isRunning()) {
    sprite->onUpdate(0.0);
    graphics.renderFrame();
    input_system.pollForInput();
  }

  graphics.destroy();

  return 0;
}
