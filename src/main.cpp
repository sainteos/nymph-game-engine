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
#include "utility/config_manager.h"
#include "graphics/light.h"

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
  graphics.initialize(config.getInt("screen_width"), config.getInt("screen_height"), config.getString("window_name"), Graphics::WindowExitFunctor());

  Input::InputSystem input_system(graphics.getWindow());

  RenderableFactory renderable_factory;

  std::shared_ptr<ShaderManager> shader_manager = std::make_shared<ShaderManager>();
  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->getShader("diffuse_lighting")->setUniform("resolution", glm::vec2(config.getFloat("screen_width"), config.getFloat("screen_height")));

  float viewport_tile_width = config.getFloat("screen_width_tiles");
  float viewport_tile_height = config.getFloat("screen_height_tiles");

  std::shared_ptr<Camera> camera = std::make_shared<Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config.getFloat("near_plane"), config.getFloat("far_plane"));
  camera->setTransform(std::make_shared<Transform>());
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

  sprite->addObserver(camera);
  
  auto transform = std::make_shared<Transform>();

  for(auto& i : renderables.tiles) {
    transform->addChild(i->getTransform());
    i->setAmbientLight(glm::vec3(config.getFloat("ambient_color_r"), config.getFloat("ambient_color_g"), config.getFloat("ambient_color_b")));
    i->setAmbientIntensity(config.getFloat("ambient_intensity"));
    graphics.addRenderable(i);
  }

  for(auto& i : renderables.animated_tiles) {
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
  //camera->getTransform()->translate(glm::vec2(sprite->getTransform()->getAbsoluteTranslation()));
  camera->getTransform()->translate(glm::vec2(config.getFloat("camera_x"), config.getFloat("camera_y")));

  std::shared_ptr<Light> light = std::make_shared<Light>();
  light->setTransform(std::make_shared<Transform>());
  light->getTransform()->translate(glm::vec3(config.getFloat("light_position_x"), config.getFloat("light_position_y"), config.getFloat("light_position_z")));
  light->setColor(glm::vec3(config.getFloat("light_color_r"), config.getFloat("light_color_g"), config.getFloat("light_color_b")));
  light->setIntensity(config.getFloat("light_intensity"));
  light->setNumberOfQuantizedBands(config.getInt("number_quantized_bands"));
  light->setLinearAttenuation(config.getFloat("linear_attenuation"));
  light->setQuadraticAttenuation(config.getFloat("quadratic_attenuation"));
  //transform->addChild(light->getTransform());
  graphics.addLight(light);

  std::shared_ptr<Light> light2 = std::make_shared<Light>(Light::Type::SPOT);
  light2->setTransform(std::make_shared<Transform>());
  light2->getTransform()->translate(glm::vec3(config.getFloat("light2_position_x"), config.getFloat("light2_position_y"), config.getFloat("light2_position_z")));
  light2->setColor(glm::vec3(config.getFloat("light2_color_r"), config.getFloat("light2_color_g"), config.getFloat("light2_color_b")));
  light2->setIntensity(config.getFloat("light2_intensity"));
  light2->setNumberOfQuantizedBands(config.getInt("2_number_quantized_bands"));
  light2->setLinearAttenuation(config.getFloat("2_linear_attenuation"));
  light2->setQuadraticAttenuation(config.getFloat("2_quadratic_attenuation"));
  light2->setConeAngle(config.getFloat("2_cone_angle"));
  light2->setConeDirection(glm::vec3(config.getFloat("2_cone_x"), config.getFloat("2_cone_y"), config.getFloat("2_cone_z")));
  
  graphics.addLight(light2);
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

    sprite->onUpdate(delta);
    graphics.renderFrame(delta);
    input_system.pollForInput();
    delta = fps_counter.assessCountAndGetDelta();
  }

  graphics.destroy();

  return 0;
}
