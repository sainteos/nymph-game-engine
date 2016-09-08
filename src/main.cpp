#include <easylogging++.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <tmx/Tmx.h>
#include <sstream>
#include <cstdlib>
#include <functional>
#include <typeinfo>
#include "graphics/graphics_system.h"
#include "graphics/renderable.h"
#include "graphics/vertex_data.h"
#include "graphics/window_exit_functor.h"
#include "graphics/shader.h"
#include "graphics/base_texture.h"
#include "graphics/shader_manager.h"
#include "graphics/map_helper.h"
#include "transform.h"
#include "sprite_movement.h"
#include "input/input_system.h"
#include "utility/fps_counter.h"
#include "utility/config_manager.h"
#include "graphics/light.h"
#include "utility/utility_functions.h"
#include "graphics/ui/font_generator.h"
#include "entity.h"
#include "component_manager.h"
#include "graphics/camera.h"

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

  std::shared_ptr<ComponentManager> component_manager = std::make_shared<ComponentManager>();


  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();

  std::shared_ptr<TextureManager> texture_manager = std::make_shared<TextureManager>();

  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->loadShader("simple_text");

  float viewport_tile_width = config.getFloat("screen_width_tiles");
  float viewport_tile_height = config.getFloat("screen_height_tiles");

  std::shared_ptr<Entity> camera = std::make_shared<Entity>();
  std::shared_ptr<Camera> camera_component = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config.getFloat("near_plane"), config.getFloat("far_plane"));
  camera->addComponent(camera_component);
  camera->setActive(true);

  component_manager->addComponent(camera_component);

  UI::FontGenerator font_generator("./project-spero-assets/");
  font_generator.loadFont("Jack.ttf", 64, "jack");


  MapHelper map_helper(component_manager, texture_manager, shader_manager);

  
  auto renderables = map_helper.createRenderablesFromMap(config.getInt("patch_width"), config.getInt("patch_height"), *map);
  auto animations = map_helper.createAnimationsFromAnimationMap(*animation_map);
  auto static_animations = map_helper.createStaticallyAnimatedTilesFromMap(*map);
  auto lights = map_helper.createLightsFromMap(*map);
  auto text = map_helper.createText(font_generator.getFont("jack"), glm::vec4(0.1, 0.4, 0.2, 0.5));

  
  auto transform = std::make_shared<Transform>();

  std::vector<std::shared_ptr<Entity>> entities;

  for(auto& i : renderables.entities) {
    transform->addChild(i->getTransform());
  }

  for(auto& i : static_animations) {
    transform->addChild(i->getTransform());
  }

  // for(auto& i : lights) {
  //   transform->addChild(i->getTransform());
  //   graphics.addLight(i);
  //   component_manager.addComponent(i);
  // }

  transform->translate(glm::vec2(-map->GetWidth() / 2.0, -map->GetHeight() / 2.0));
  
  auto matcher = [](std::shared_ptr<DynamicAnimation> a, const std::string& sprite) {
    return a->sprite_name == sprite;
  };

  auto aidan = *std::find_if(animations.begin(), animations.end(), std::bind(matcher, _1, "Aidan"));

  auto sprite = aidan->entity;

  for(auto& i : renderables.dynamic_animations) {
    if(i.sprite_name == "Aidan") {
      sprite->getTransform()->translate(glm::vec3((float)i.x_pos, (float)i.y_pos, (float)i.z_order));
      aidan->animator->setStartingState(i.default_animation);
    }
  }

  std::shared_ptr<SpriteMovement> sprite_movement = std::make_shared<SpriteMovement>();
  sprite_movement->setAnimationStringState(SpriteState::MOVE_UP, "Up_Movement");
  sprite_movement->setAnimationStringState(SpriteState::MOVE_DOWN, "Down_Movement");
  sprite_movement->setAnimationStringState(SpriteState::MOVE_LEFT, "Left_Movement");
  sprite_movement->setAnimationStringState(SpriteState::MOVE_RIGHT, "Right_Movement");
  sprite_movement->setAnimationStringState(SpriteState::FACE_UP, "Up_Still");
  sprite_movement->setAnimationStringState(SpriteState::FACE_DOWN, "Down_Still");
  sprite_movement->setAnimationStringState(SpriteState::FACE_LEFT, "Left_Still");
  sprite_movement->setAnimationStringState(SpriteState::FACE_RIGHT, "Right_Still");
  sprite_movement->addObserver(camera_component);
  sprite_movement->setMovingSpeed(2.0);
  component_manager->addComponent(sprite_movement);

  sprite->addComponent(sprite_movement);
  input_system.addObserver(sprite_movement);

  sprite->setActive(true);

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

  graphics.startRender();
  component_manager->onStart();

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
    graphics.startFrame();
    component_manager->onUpdate(delta);
    graphics.stopFrame();
    input_system.pollForInput();
    delta = fps_counter.assessCountAndGetDelta();
  }

  graphics.destroy();
  component_manager->destroy();

  return 0;
}
