#include <easylogging++.h>
#include "engine.h"
#include "scene_generator.h"
#include "graphics/ui/text_field.h"
#include "graphics/ui/skin.h"
#include "sprite_movement.h"


Engine::Engine() {
  config_manager = std::make_shared<Utility::ConfigManager>();
  graphics_system = std::make_shared<Graphics::GraphicsSystem>();
  component_manager = std::make_shared<ComponentManager>();
  shader_manager = std::make_shared<Graphics::ShaderManager>();
  texture_manager = std::make_shared<Graphics::TextureManager>();
}

void Engine::setup(const std::string config_path) {
  //Load Config
  if(!config_manager->loadConfig(config_path)) {
    LOG(FATAL)<<"Could not load config: "<<config_path;
    throw;
  }

  //Initialize graphics system
  graphics_system->initialize(config_manager->getInt("screen_width"), config_manager->getInt("screen_height"), config_manager->getString("window_name"), config_manager->getBool("fullscreen"), Graphics::WindowExitFunctor());
  
  //Initialize font generator
  font_generator = std::make_shared<Graphics::UI::FontGenerator>(config_manager->getString("fonts_location"), config_manager->getInt("pixels_per_unit"));

  viewport_tile_width = config_manager->getFloat("screen_width_tiles");
  viewport_tile_height = config_manager->getFloat("screen_height_tiles");

  //Initizlie camera
  std::shared_ptr<Graphics::Camera> camera_component = std::make_shared<Graphics::Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config_manager->getFloat("near_plane"), config_manager->getFloat("far_plane"));
  camera_component->setActive(true);
  component_manager->addComponent(camera_component);
  //Initialize input system
  input_system = std::make_shared<Input::InputSystem>(graphics_system->getWindow(), viewport_tile_width, viewport_tile_height, camera_component->getTransform()->getAbsoluteTransformationMatrix(), camera_component->getProjectionMatrix());

  //Initialize fps counter
  fps_counter = std::make_shared<Utility::FPSCounter>(60.0f);


  camera_component->getTransform()->translate(glm::vec2(config_manager->getFloat("camera_x"), config_manager->getFloat("camera_y")));
  ///THIS WILL ALL BE MOVED TO SCRIPTING

  Tmx::Map *map = new Tmx::Map();
  map->ParseFile(config_manager->getString("map"));

  Tmx::Map *animation_map = new Tmx::Map();
  animation_map->ParseFile(config_manager->getString("animation_database"));

  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->loadShader("simple_text");
  shader_manager->loadShader("simple_ui");

  SceneGenerator scene_generator(*animation_map, component_manager, texture_manager, shader_manager);


  texture_manager->loadTexture("./project-spero-assets/grayscale_tex.png");

  font_generator->loadFont("Jack.TTF", 18, "jack");

  auto scene = scene_generator.createSceneFromMap(config_manager->getInt("patch_width"), config_manager->getInt("patch_height"), *map);
  
  auto default_text = std::make_shared<Graphics::UI::WrappableText>();
  default_text->setFont(font_generator->getFont("jack"));
  auto text = std::make_shared<Graphics::UI::WrappableText>();
  text->setFont(font_generator->getFont("jack"));
        
  text->setShader(shader_manager->getShader("simple_text"));
  text->setHorizontalAlignment(Graphics::UI::WrappableText::HorizontalAlignment::LEFT);
  text->setVerticalAlignment(Graphics::UI::WrappableText::VerticalAlignment::CENTER);
  text->setColor(glm::vec4(0.9, 0.8, 0.8, 1.0));

  default_text->setShader(shader_manager->getShader("simple_text"));
  default_text->setHorizontalAlignment(Graphics::UI::WrappableText::HorizontalAlignment::LEFT);
  default_text->setVerticalAlignment(Graphics::UI::WrappableText::VerticalAlignment::CENTER);
  default_text->setColor(glm::vec4(0.7, 0.7, 0.7, 1.0));

  text->setActive(false);
  default_text->setActive(true);

  auto skin = std::make_shared<Graphics::UI::Skin>(Graphics::UI::Skin { texture_manager->getTexture("grayscale_tex"), (*shader_manager)["simple_ui"] });
  auto field = Graphics::UI::TextField::create(skin, default_text, text, glm::vec4(0.2, 0.2, 0.2, 0.8), 0.3, viewport_tile_width, viewport_tile_height, 0.0, 0.0, 8.0, 2.0);
  
  field->setActive(true);
  field->getTransform()->addChild(text->getTransform());
  field->getTransform()->addChild(default_text->getTransform());

  input_system->addObserver(field);
  field->addObserver(input_system);

  default_text->setText("click to type");

  camera_component->getTransform()->addChild(field->getTransform());

  auto sprite = scene_generator.getDynamicEntityByName("Aidan");

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

  sprite->addComponent(sprite_movement);
  scene->addComponent(sprite_movement);
  input_system->addObserver(sprite_movement);

  sprite->setActive(true);

  scenes[config_manager->getString("map")] = scene;
  component_manager->addComponents(scene->getComponents());
}

void Engine::mainLoop() {

  float delta = 0.0f;
  float fps = fps_counter->getCurrentFPS();
  graphics_system->startRender();
  component_manager->onStart();

  while(graphics_system->isRunning()) {
    if(fps != fps_counter->getCurrentFPS()) {
      std::stringstream window_name;
      window_name <<config_manager->getString("window_title")<<" "<<fps_counter->getCurrentFPS();
      graphics_system->setWindowName(window_name.str());
      fps = fps_counter->getCurrentFPS();
    }
    graphics_system->startFrame();
    component_manager->onUpdate(delta);
    graphics_system->stopFrame();
    input_system->pollForInput();
    delta = fps_counter->assessCountAndGetDelta();
  }
}

void Engine::cleanUp() {
   graphics_system->destroy();
   component_manager->destroy();
}