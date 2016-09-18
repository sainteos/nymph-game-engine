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
#include "graphics/ui/skin.h"
#include "graphics/ui/area.h"
#include "entity.h"
#include "component_manager.h"
#include "graphics/camera.h"
#include "graphics/ui/wrappable_text.h"
#include "graphics/ui/text_area.h"
#include "graphics/ui/button.h"
#include "graphics/ui/text_field.h"

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

  float viewport_tile_width = config.getFloat("screen_width_tiles");
  float viewport_tile_height = config.getFloat("screen_height_tiles");


  std::shared_ptr<ComponentManager> component_manager = std::make_shared<ComponentManager>();
  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();
  std::shared_ptr<TextureManager> texture_manager = std::make_shared<TextureManager>();

  std::shared_ptr<Entity> camera = std::make_shared<Entity>();
  std::shared_ptr<Camera> camera_component = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config.getFloat("near_plane"), config.getFloat("far_plane"));
  camera->addComponent(camera_component);
  camera->setActive(true);

  camera->getTransform()->translate(glm::vec2(config.getFloat("camera_x"), config.getFloat("camera_y")));

  component_manager->addComponent(camera_component);

  std::shared_ptr<Input::InputSystem> input_system = std::make_shared<Input::InputSystem>(graphics.getWindow(), viewport_tile_width, viewport_tile_height, camera_component->getTransform()->getAbsoluteTransformationMatrix(), camera_component->getProjectionMatrix());

  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->loadShader("simple_text");
  shader_manager->loadShader("simple_ui");

  texture_manager->loadTexture("./project-spero-assets/grayscale_tex.png");


  UI::FontGenerator font_generator("./project-spero-assets/", 32);
  font_generator.loadFont("Jack.TTF", 18, "jack");

  
  auto default_text = std::make_shared<UI::WrappableText>();
  default_text->setFont(font_generator.getFont("jack"));
  auto text = std::make_shared<UI::WrappableText>();
  text->setFont(font_generator.getFont("jack"));

  auto text_transform = std::make_shared<Transform>();
  text->setTransform(text_transform);
  text->setShader(shader_manager->getShader("simple_text"));
  text->setHorizontalAlignment(UI::WrappableText::HorizontalAlignment::LEFT);
  text->setVerticalAlignment(UI::WrappableText::VerticalAlignment::CENTER);
  text->setColor(glm::vec4(0.9, 0.8, 0.8, 1.0));

  auto default_text_transform = std::make_shared<Transform>();
  default_text->setTransform(default_text_transform);
  default_text->setShader(shader_manager->getShader("simple_text"));
  default_text->setHorizontalAlignment(UI::WrappableText::HorizontalAlignment::LEFT);
  default_text->setVerticalAlignment(UI::WrappableText::VerticalAlignment::CENTER);
  default_text->setColor(glm::vec4(0.7, 0.7, 0.7, 1.0));

  text->setActive(false);
  default_text->setActive(true);
  component_manager->addComponent(text);
  component_manager->addComponent(default_text);

  std::shared_ptr<UI::Skin> skin = std::make_shared<UI::Skin> (UI::Skin { texture_manager->getTexture("grayscale_tex"), (*shader_manager)["simple_ui"] });
  std::shared_ptr<UI::TextField> field = UI::TextField::create(skin, default_text, text, glm::vec4(0.2, 0.2, 0.2, 0.8), 0.3, viewport_tile_width, viewport_tile_height, 0.0, 0.0, 8.0, 2.0);
  
  field->setActive(true);
  auto field_transform = std::make_shared<Transform>();
  field->setTransform(field_transform);
  field->getTransform()->addChild(text->getTransform());
  field->getTransform()->addChild(default_text->getTransform());

  input_system->addObserver(field);
  field->addObserver(input_system);

  component_manager->addComponent(field);

  default_text->setText("click to type");

  camera->getTransform()->addChild(field->getTransform());
  

  MapHelper map_helper(component_manager, texture_manager, shader_manager);

  
  auto renderables = map_helper.createRenderablesFromMap(config.getInt("patch_width"), config.getInt("patch_height"), *map);
  auto animations = map_helper.createAnimationsFromAnimationMap(*animation_map);
  auto static_animations = map_helper.createStaticallyAnimatedTilesFromMap(*map);
  auto lights = map_helper.createLightsFromMap(*map);
  
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
  input_system->addObserver(sprite_movement);

  sprite->setActive(true);

  transform->addChild(sprite->getTransform());


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
    input_system->pollForInput();
    delta = fps_counter.assessCountAndGetDelta();
  }

  graphics.destroy();
  component_manager->destroy();

  return 0;
}
