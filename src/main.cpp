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
#include <functional>
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
#include "utility/fps_counter.h"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;
using namespace std::placeholders;

int main(int argc, char** argv) { 
  if(argc < 4) {
    std::cout<<"USAGE:"<<std::endl;
    std::cout<<"./TileEngine.out [map name with path] [camera x in tiles] [camera y in tiles]"<<std::endl;
    return 0;
  }

  START_EASYLOGGINGPP(argc, argv);

  Tmx::Map *map = new Tmx::Map();
  map->ParseFile(std::string(argv[1]));

  Tmx::Map *animation_map = new Tmx::Map();
  animation_map->ParseFile(std::string("./project-spero-assets/Maps/Animations.tmx"));

  GraphicsSystem graphics;
  graphics.initialize(1280, 720, "Dick Butts (tm)", Graphics::WindowExitFunctor());

  Input::InputSystem input_system(graphics.getWindow());

  RenderableFactory renderable_factory;

  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();
  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");;

  float viewport_tile_width = 21.0;
  float viewport_tile_height = 12.0;

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, 0.1f, 40.0f);
  camera->setTransform(std::make_shared<Transform>());
  camera->getTransform()->translate(glm::vec2(atof(argv[2]), atof(argv[3])));
  graphics.setCamera(camera);
  TextureManager texture_manager;
  
  auto renderables = renderable_factory.createFromMap(*map, texture_manager, shader_manager);
  auto animations = renderable_factory.createAnimationsFromAnimationMap(*animation_map, texture_manager, shader_manager);

  for(auto& i : animations) {
    i.tile->setTransform(std::make_shared<Transform>());
  }

  std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();

  for(auto& i : renderables.dynamic_animations) {
    if(i.sprite_name == "Aidan") {
      sprite->getTransform()->translate(glm::vec3((float)i.x_pos, (float)i.y_pos, (float)i.z_order));
    }
  }
  
  auto transform = std::make_shared<Transform>();

  for(auto i : renderables.tiles) {
    transform->addChild(i->getTransform());
    graphics.addRenderable(i);
  }

  for(auto i : renderables.animated_tiles) {
    transform->addChild(i->getTransform());
    i->setActive();
    graphics.addRenderable(i);
  }

  transform->translate(glm::vec2(-map->GetWidth() / 2.0, -map->GetHeight() / 2.0));
  
  auto matcher = [](const Animation& a, const std::string& sprite, const std::string& anim) {
    return a.sprite_name == sprite && a.animation_name == anim;
  };

  auto move_up = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Up_Movement"));
  auto move_down = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Down_Movement"));
  auto move_left = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Left_Movement"));
  auto move_right = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Right_Movement"));
  auto stop_up = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Up_Still"));
  auto stop_down = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Down_Still"));
  auto stop_left = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Left_Still"));
  auto stop_right = std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan", "Right_Still"));

  input_system.addObserver(sprite);
  sprite->addTile(Sprite::AnimationState::MOVE_UP, move_up->tile);
  graphics.addRenderable(move_up->tile);
  sprite->addTile(Sprite::AnimationState::MOVE_DOWN, move_down->tile);
  graphics.addRenderable(move_down->tile);
  sprite->addTile(Sprite::AnimationState::MOVE_LEFT, move_left->tile);
  graphics.addRenderable(move_left->tile);
  sprite->addTile(Sprite::AnimationState::MOVE_RIGHT, move_right->tile);
  graphics.addRenderable(move_right->tile);
  sprite->addTile(Sprite::AnimationState::FACE_UP, stop_up->tile);
  graphics.addRenderable(stop_up->tile);
  sprite->addTile(Sprite::AnimationState::FACE_DOWN, stop_down->tile);
  graphics.addRenderable(stop_down->tile);
  sprite->addTile(Sprite::AnimationState::FACE_LEFT, stop_left->tile);
  graphics.addRenderable(stop_left->tile);
  sprite->addTile(Sprite::AnimationState::FACE_RIGHT, stop_right->tile);
  graphics.addRenderable(stop_right->tile);
  sprite->setMovingSpeed(2.0);
  transform->addChild(sprite->getTransform());

  sprite->onStart();
  graphics.startRender();

  Utility::FPSCounter fps_counter(60.0f);
  float delta = 0.0f;
  float fps = fps_counter.getCurrentFPS();
  
  while(graphics.isRunning()) {
    if(fps != fps_counter.getCurrentFPS()) {
      std::stringstream window_name;
      window_name << "Dick Butts(tm)"<<"        "<<fps_counter.getCurrentFPS();
      graphics.setWindowName(window_name.str());
      fps = fps_counter.getCurrentFPS();
    }

    sprite->onUpdate(delta);
    graphics.renderFrame(delta);
    input_system.pollForInput();
    delta = fps_counter.assessCountAndGetDelta();
  }

  graphics.destroy();

  return 0;
}
