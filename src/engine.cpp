#include <easylogging++.h>
#include "engine.h"
#include "scene_generator.h"
#include "graphics/ui/text_field.h"
#include "graphics/ui/quit_button.h"
#include "graphics/ui/button.h"
#include "graphics/ui/skin.h"
#include "sprite_movement.h"
#include "utility/load_map_event.h"
#include "input/key_down_event.h"


Engine::Engine() : time_to_exit(false), free_camera(false) {
  config_manager = std::make_shared<Utility::ConfigManager>();
  graphics_system = std::make_shared<Graphics::GraphicsSystem>();
  component_manager = std::make_shared<ComponentManager>();
  shader_manager = std::make_shared<Graphics::ShaderManager>();
  texture_manager = std::make_shared<Graphics::TextureManager>();
  debug = std::make_shared<Utility::DebugParser>();
  debug_console = std::make_shared<Entity>();
  map_list_ui = std::make_shared<Entity>();
}

void Engine::onNotifyNow(std::shared_ptr<Events::Event> event) {
  handleQueuedEvent(event);
}

void Engine::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
  switch(event->getEventType()) {
    case Events::EventType::LIST_CHARACTERS:
      //showCharacterList();
      break;

    case Events::EventType::LIST_MAPS: {
      std::stringstream map_list;

      for(auto scene : scenes) {
        map_list << scene.first << '\n';
      }
      map_list_text_area->getText()->setText(map_list.str());
      map_list_ui->setActive(true);
      break;
    }

    case Events::EventType::LIST_LAYERS:
      //showLayerList();
      break;

    case Events::EventType::LOAD_MAP: {
      auto casted_event = std::static_pointer_cast<Utility::LoadMapEvent>(event);
      deactivateScene(active_scenes.begin()->first);
      activateScene(casted_event->getName());
      break;
    }
    case Events::EventType::LOAD_CHARACTER:
      break;
    case Events::EventType::TOGGLE_FREE_CAMERA: {
      LOG(INFO)<<"Spong";
      if(free_camera) {
        free_camera = false;
        input_system->removeObserver(camera_component);
        input_system->addObserver(sprite_movement);
      }
      else {
        LOG(INFO)<<"FARP";
        free_camera = true;
        input_system->removeObserver(sprite_movement);
        input_system->addObserver(camera_component);
      }
      break;
    }

    case Events::EventType::TOGGLE_LAYER:
      break;
    case Events::EventType::TOGGLE_LIGHTS:
      break;
    case Events::EventType::WINDOW_EXIT:
      time_to_exit = true;
      break;

  }
}


void Engine::activateScene(const std::string& name) {
  LOG(INFO)<<name;
  if(active_scenes.count(name) == 0 && scenes.count(name) > 0) {
    active_scenes[name] = scenes[name];
    component_manager->addComponents(active_scenes[name]->getComponents());
  }
}

void Engine::deactivateScene(const std::string& name) {
  if(active_scenes.count(name) > 0) {
    component_manager->removeComponents(active_scenes[name]->getComponents());
    active_scenes.erase(name);
  }
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
  camera_component = std::make_shared<Graphics::Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config_manager->getFloat("near_plane"), config_manager->getFloat("far_plane"));
  camera_component->setActive(true);
  camera_component->setFreeCameraSpeed(config_manager->getFloat("camera_speed"));
  component_manager->addComponent(camera_component);
  //Initialize input system
  input_system = std::make_shared<Input::InputSystem>(graphics_system->getWindow(), viewport_tile_width, viewport_tile_height, camera_component->getTransform()->getAbsoluteTransformationMatrix(), camera_component->getProjectionMatrix());

  //Initialize fps counter
  fps_counter = std::make_shared<Utility::FPSCounter>(60.0f);

  //texture and font nonsense will be elsewhere

  texture_manager->loadTexture("./project-spero-assets/grayscale_tex.png");

  font_generator->loadFont("8bit.ttf", 18, "8bit");

  //Shader loading will be done elsewhere

  shader_manager->loadShader("simple_texture");
  shader_manager->loadShader("tile_animation");
  shader_manager->loadShader("diffuse_lighting");
  shader_manager->loadShader("simple_text");
  shader_manager->loadShader("simple_ui");

  //Setup Debug Console
  debug->addObserver(shared_from_this());

  auto default_text = std::make_shared<Graphics::UI::WrappableText>();
  default_text->setFont(font_generator->getFont("8bit"));
  auto text = std::make_shared<Graphics::UI::WrappableText>();
  text->setFont(font_generator->getFont("8bit"));
        
  text->setShader(shader_manager->getShader("simple_text"));
  text->setHorizontalAlignment(Graphics::UI::WrappableText::HorizontalAlignment::LEFT);
  text->setVerticalAlignment(Graphics::UI::WrappableText::VerticalAlignment::CENTER);
  text->setColor(glm::vec4(0.1, 1.0, 0.1, 0.9));
  text->setKerning(0.12);
  default_text->setShader(shader_manager->getShader("simple_text"));
  default_text->setHorizontalAlignment(Graphics::UI::WrappableText::HorizontalAlignment::LEFT);
  default_text->setVerticalAlignment(Graphics::UI::WrappableText::VerticalAlignment::CENTER);
  default_text->setColor(glm::vec4(0.4, 0.4, 0.4, 0.9));
  default_text->setKerning(0.12);

  text->setActive(false);
  default_text->setActive(true);

  auto skin = std::make_shared<Graphics::UI::Skin>(Graphics::UI::Skin { texture_manager->getTexture("grayscale_tex"), (*shader_manager)["simple_ui"] });
  auto field = Graphics::UI::TextField::create(skin, default_text, text, glm::vec4(0.2, 0.2, 0.2, 0.8), 0.1, viewport_tile_width, viewport_tile_height, 0.0, 0.0, 16.0, 1.5);
  
  field->setActive(false);
  field->getTransform()->addChild(text->getTransform());
  field->getTransform()->addChild(default_text->getTransform());

  input_system->addObserver(field);
  field->addObserver(input_system);
  field->addObserver(debug);

  default_text->setText("click to type");

  debug_console->addComponent(text);
  debug_console->addComponent(default_text);
  debug_console->addComponent(field);

  camera_component->getTransform()->addChild(debug_console->getTransform());

  component_manager->addComponents(std::vector<std::shared_ptr<Component>> { debug_console->getComponents().begin(), debug_console->getComponents().end() });

  //Setup Map List Entity
  auto map_list_text = std::make_shared<Graphics::UI::WrappableText>();
  map_list_text->setFont(font_generator->getFont("8bit"));
  map_list_text->setShader(shader_manager->getShader("simple_text"));
  map_list_text->setHorizontalAlignment(Graphics::UI::WrappableText::HorizontalAlignment::CENTER);
  map_list_text->setVerticalAlignment(Graphics::UI::WrappableText::VerticalAlignment::TOP);
  map_list_text->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
  map_list_text->setKerning(0.12);

  map_list_text_area = Graphics::UI::TextArea::create(skin, map_list_text, glm::vec4(0.2, 0.2, 0.2, 0.8), glm::vec4(1.0, 1.0, 1.0, 1.0), 0.5, viewport_tile_width, viewport_tile_height, 0.0, 0.0, 16.0, 11.5);
  
  map_list_ui->addComponent(map_list_text);
  map_list_ui->addComponent(map_list_text_area);

  auto map_list_quit_button_text = std::make_shared<Graphics::UI::WrappableText>();
  map_list_quit_button_text->setFont(font_generator->getFont("8bit"));
  map_list_quit_button_text->setShader(shader_manager->getShader("simple_text"));

  auto map_list_quit_button = Graphics::UI::QuitButton::create(skin, map_list_quit_button_text, glm::vec4(0.6, 0.6, 0.6, 0.8), glm::vec4(0.8, 0.0, 0.0, 0.8), viewport_tile_width, viewport_tile_height, 0.0, 0.0, 1.0, 1.0);
  map_list_quit_button->getTransform()->translate(glm::vec3(map_list_text_area->getWidth() / 2.0, map_list_text_area->getHeight() / 2.0, 0.0));

  map_list_ui->addComponent(map_list_quit_button_text);
  map_list_ui->addComponent(map_list_quit_button);
  input_system->addObserver(map_list_quit_button);
  
  map_list_ui->setActive(false);

  component_manager->addComponents(std::vector<std::shared_ptr<Component>> { map_list_ui->getComponents().begin(), map_list_ui->getComponents().end()});

  camera_component->getTransform()->translate(glm::vec2(config_manager->getFloat("camera_x"), config_manager->getFloat("camera_y")));
  ///THIS WILL ALL BE MOVED TO SCRIPTING


  Tmx::Map *animation_map = new Tmx::Map();
  animation_map->ParseFile(config_manager->getString("animation_database"));

  SceneGenerator scene_generator(*animation_map, texture_manager, shader_manager);

  for(auto map_name : config_manager->getStringVector("maps")) {
    Tmx::Map *map = new Tmx::Map();
    map->ParseFile(map_name);

    auto stripped_name = map_name.substr(map_name.find_last_of("/") + 1, map_name.size() - map_name.find_last_of("/") - 5);
    
    auto scene = scene_generator.createSceneFromMap(config_manager->getInt("patch_width"), config_manager->getInt("patch_height"), *map);
    

    scenes[stripped_name] = scene;

    delete map;
  }

  auto sprite = scene_generator.getDynamicEntityByName("Aidan");

  sprite_movement = std::make_shared<SpriteMovement>();
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
  input_system->addObserver(sprite_movement);

  sprite->setActive(true);

  auto stripped_name = config_manager->getString("active_map").substr(config_manager->getString("active_map").find_last_of("/") + 1, config_manager->getString("active_map").size() - config_manager->getString("active_map").find_last_of("/") - 5);
  scenes[stripped_name]->addEntity(sprite);

  activateScene(stripped_name);
}

void Engine::mainLoop() {

  float delta = 0.0f;
  float fps = fps_counter->getCurrentFPS();
  graphics_system->startRender();
  component_manager->onStart();

  while(graphics_system->isRunning() && !time_to_exit) {
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
    debug->handleEvents();
    while(eventsWaiting()) {
      handleQueuedEvent(getEvent());
    }
    delta = fps_counter->assessCountAndGetDelta();
  }
}

void Engine::cleanUp() {
   graphics_system->destroy();
   component_manager->destroy();
}