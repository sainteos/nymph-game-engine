#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <tmx/Tmx.h>
#include <sstream>
#include <cstdlib>
#include <functional>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/shader.h"
#include "graphics/base_texture.h"
#include "graphics/shader_manager.h"
#include "graphics/map_helper.h"
#include "transform.h"
#include "graphics/camera.h"
#include "sprite_movement.h"
#include "input/input_system.h"
#include "utility/fps_counter.h"
#include "utility/config_manager.h"
#include "graphics/light.h"
#include "utility/utility_functions.h"
#include "graphics/ui/font_generator.h"
#include "entity.h"
#include "triggerable.hpp"

INITIALIZE_EASYLOGGINGPP
#define ELPP_THREAD_SAFE

using namespace Graphics;
using namespace std::placeholders;

int main(int argc, char** argv) {
  START_EASYLOGGINGPP(argc, argv);
  
  LOG(INFO)<<"Number of args: "<<argc;
  std::string config_path;
  if(argc > 2) {
    config_path = std::string(argv[1]);
  }
  else {
    config_path = "./config/default.json";
  }

  Utility::ConfigManager config;
  if(!config.loadConfig(config_path)) {
    LOG(FATAL)<<"Could not load config: "<<config_path;
    return 0;
  }

  Tmx::Map *map = new Tmx::Map();
  map->ParseFile(config.getString("map"));

  Tmx::Map *animation_map = new Tmx::Map();
  animation_map->ParseFile(config.getString("animation_database"));

  GraphicsSystem graphics;
  graphics.initialize(config.getInt("screen_width"), config.getInt("screen_height"), config.getString("window_name"), config.getBool("fullscreen"), Graphics::WindowExitFunctor());

  Input::InputSystem input_system(graphics.getWindow());

  MapHelper map_helper;

  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();
  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->loadShader("simple_text");

  float viewport_tile_width = config.getFloat("screen_width_tiles");
  float viewport_tile_height = config.getFloat("screen_height_tiles");

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config.getFloat("near_plane"), config.getFloat("far_plane"));
  camera->setTransform(std::make_shared<Transform>());
  graphics.setCamera(camera);

  UI::FontGenerator font_generator("./project-spero-assets/");
  font_generator.loadFont("Jack.ttf", 64, "jack");

  TextureManager texture_manager;
  
  auto renderables = map_helper.createRenderablesFromMap(*map, texture_manager, shader_manager);
  auto animations = map_helper.createAnimationsFromAnimationMap(*animation_map, texture_manager, shader_manager);
  auto static_animations = map_helper.createStaticallyAnimatedTilesFromMap(*map, texture_manager, shader_manager);
  auto lights = map_helper.createLightsFromMap(*map);
  auto text = map_helper.createText(font_generator.getFont("jack"), glm::vec4(0.1, 0.4, 0.2, 0.5));

  std::shared_ptr<Entity> sprite = std::make_shared<Entity>();

  for(auto& i : renderables.dynamic_animations) {
    if(i.sprite_name == "Aidan") {
      sprite->getTransform()->translate(glm::vec3((float)i.x_pos, (float)i.y_pos, (float)i.z_order));
    }
  }

  std::shared_ptr<SpriteMovement> sprite_movement = std::make_shared<SpriteMovement>();
  sprite_movement->addObserver(camera);
  sprite_movement->setMovingSpeed(2.0);
  sprite_movement->setActive(true);

  sprite->addComponent(sprite_movement);
  input_system.addObserver(sprite_movement);
  
  auto transform = std::make_shared<Transform>();

  for(auto& i : renderables.renderables) {
    transform->addChild(i->getTransform());
    graphics.addRenderable(i);
  }

  for(auto& i : static_animations) {
    transform->addChild(i.renderable->getTransform());
    graphics.addRenderable(i.renderable);
    i.animator->addObserver(i.renderable);
  }

  for(auto& i : lights) {
    transform->addChild(i->getTransform());
    graphics.addLight(i);
  }

  transform->translate(glm::vec2(-map->GetWidth() / 2.0, -map->GetHeight() / 2.0));
  
  auto matcher = [](const DynamicAnimation& a, const std::string& sprite, const std::string& anim) {
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

  std::shared_ptr<Triggerable<SpriteState>> triggerable = std::make_shared<Triggerable<SpriteState>>(SpriteState::FACE_DOWN);
  triggerable->setActive(true);
  sprite->addComponent(triggerable);

  triggerable->addTriggerableComponent(SpriteState::MOVE_UP, move_up->renderable->getId());
  sprite->addComponent(move_up->renderable);
  //graphics.addRenderable(move_up->renderable);

  triggerable->addTriggerableComponent(SpriteState::MOVE_DOWN, move_down->renderable->getId());
  sprite->addComponent(move_down->renderable);
 // graphics.addRenderable(move_down->renderable);

  triggerable->addTriggerableComponent(SpriteState::MOVE_LEFT, move_left->renderable->getId());
  sprite->addComponent(move_left->renderable);
  //graphics.addRenderable(move_left->renderable);

  triggerable->addTriggerableComponent(SpriteState::MOVE_RIGHT, move_right->renderable->getId());
  sprite->addComponent(move_right->renderable);
 // graphics.addRenderable(move_right->renderable);


  triggerable->addTriggerableComponent(SpriteState::FACE_UP, stop_up->renderable->getId());
  sprite->addComponent(stop_up->renderable);
  //graphics.addRenderable(stop_up->renderable);

  triggerable->addTriggerableComponent(SpriteState::FACE_DOWN, stop_down->renderable->getId());
  sprite->addComponent(stop_down->renderable);
 // graphics.addRenderable(stop_down->renderable);

  triggerable->addTriggerableComponent(SpriteState::FACE_LEFT, stop_left->renderable->getId());
  sprite->addComponent(stop_left->renderable);
 // graphics.addRenderable(stop_left->renderable);

  triggerable->addTriggerableComponent(SpriteState::FACE_RIGHT, stop_right->renderable->getId());
  sprite->addComponent(stop_right->renderable);
 // graphics.addRenderable(stop_right->renderable);


  transform->addChild(sprite->getTransform());
  camera->getTransform()->translate(glm::vec2(config.getFloat("camera_x"), config.getFloat("camera_y")));

  // text->getTransform()->translate(glm::vec3(0.0, 0.0, -0.3));
  // text->getTransform()->scale(glm::vec2(1.0 / 64.0, 1.0 / 64.0));
  // text->setShader(shader_manager->getShader("simple_text"));
  // text->setText("Banana Hammock");
  // if(config.getString("text_horizontal_alignment") == "left") {
  //   text->setHorizontalAlignment(UI::Text::HorizontalAlignment::LEFT);
  // }
  // else if(config.getString("text_horizontal_alignment") == "center") {
  //   text->setHorizontalAlignment(UI::Text::HorizontalAlignment::CENTER);
  // }
  // else if(config.getString("text_horizontal_alignment") == "right") {
  //   text->setHorizontalAlignment(UI::Text::HorizontalAlignment::RIGHT);
  // }

  // if(config.getString("text_vertical_alignment") == "top") {
  //   text->setVerticalAlignment(UI::Text::VerticalAlignment::TOP);
  // }
  // else if(config.getString("text_vertical_alignment") == "center") {
  //   text->setVerticalAlignment(UI::Text::VerticalAlignment::CENTER);
  // }
  // else if(config.getString("text_vertical_alignment") == "bottom") {
  //   text->setVerticalAlignment(UI::Text::VerticalAlignment::BOTTOM);
  // }
  // text->setActive();
  //camera->getTransform()->addChild(text->getTransform());
  //graphics.addRenderable(text);

  for(auto i : static_animations) {
    i.animator->onStart();
  }
  sprite->onStart();

  graphics.startRender();

  Utility::FPSCounter fps_counter(60.0f);
  float delta = 0.0f;
  float fps = fps_counter.getCurrentFPS();
  
  while(graphics.isRunning()) {
    if(fps != fps_counter.getCurrentFPS()) {
      std::stringstream window_name;
      window_name <<config.getString("window_title")<<" "<<fps_counter.getCurrentFPS();
      graphics.setWindowName(window_name.str());
      fps = fps_counter.getCurrentFPS();
    }
    for(auto i : static_animations) {
      i.animator->onUpdate(delta);
    }
    
    sprite->onUpdate(delta);
    graphics.renderFrame(delta);
    input_system.pollForInput();
    delta = fps_counter.assessCountAndGetDelta();
  }

  graphics.destroy();

  return 0;
}
