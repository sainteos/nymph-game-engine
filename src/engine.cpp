#include <easylogging++.h>
#include <algorithm>
#include "engine.h"
#include "utility/load_map_event.h"
#include "input/key_down_event.h"
#include "graphics/ui/change_text_event.h"


Engine::Engine() : time_to_exit(false) {
  config_manager = std::make_shared<Utility::ConfigManager>();
  graphics_system = std::make_shared<Graphics::GraphicsSystem>();
  component_manager = std::make_shared<ComponentManager>();
  shader_manager = std::make_shared<Graphics::ShaderManager>();
  texture_manager = std::make_shared<Graphics::TextureManager>();
  
  scripting_system = std::make_shared<Script::ScriptingSystem>("./scripts/");
}

void Engine::timeToExit() noexcept {
  time_to_exit = true;
}

void Engine::addScene(std::shared_ptr<Game::Scene> scene) noexcept {
  scenes[scene] = false;
}

std::shared_ptr<Game::Scene> Engine::findSceneByName(const std::string& name) noexcept {
  auto find_by_name = [&](std::pair<std::shared_ptr<Game::Scene>, bool> pair) {
    return pair.first->getName() == name;
  };

  auto scene_iter = std::find_if(scenes.begin(), scenes.end(), find_by_name);

  if(scene_iter == scenes.end()) {
    return nullptr;
  }
  else {
    return scene_iter->first;
  }
}

std::map<std::shared_ptr<Game::Scene>, bool> Engine::getScenes() const noexcept {
  return scenes;
}


void Engine::activateScene(const std::string& name) {
  auto scene = findSceneByName(name);

  if(scene != nullptr) {
    scenes[scene] = true;
    component_manager->addComponents(scene->getComponents());
    for(auto c : scene->getComponents()) {
      c->onStart();
    }
  }
}

void Engine::deactivateScene(const std::string& name) {
  auto scene = findSceneByName(name);
  
  if(scene != nullptr && scenes[scene] == true) {
    scenes[scene] = false;
    component_manager->removeComponents(scene->getComponents());
  }
}

const std::vector<std::string> Engine::getSceneNames() const noexcept {
  auto names = std::vector<std::string>();

  for(auto scene : scenes) {
    names.push_back(scene.first->getName());
  }
  return names;
}

const std::vector<std::string> Engine::getActiveSceneNames() const noexcept {
  auto names = std::vector<std::string>();
  for(auto scene : scenes) {
    if(scene.second) {
      names.push_back(scene.first->getName());
    }
  }
  return names;
}

void Engine::setup(const std::string config_path) {
  //Load Config
  if(!config_manager->loadConfig(config_path)) {
    LOG(FATAL)<<"Could not load config: "<<config_path;
    throw;
  }
  
  LOG(INFO)<<"Using sounds location: "<<config_manager->getString("sounds_location");
  sound_system = std::make_shared<Sound::SoundSystem>(config_manager->getString("sounds_location"));
  
  scripting_system->addGlobalObject<ComponentManager>(component_manager, "component_manager");
  scripting_system->addGlobalObject<Utility::ConfigManager>(config_manager, "config");
  scripting_system->addGlobalObject<Engine>(shared_from_this(), "engine");

  //Initialize graphics system
  graphics_system->initialize(config_manager->getInt("screen_width"), config_manager->getInt("screen_height"), config_manager->getString("window_title"), config_manager->getBool("fullscreen"), Graphics::WindowExitFunctor());
  scripting_system->addGlobalObject<Graphics::GraphicsSystem>(graphics_system, "graphics_system");
  scripting_system->addGlobalObject<Graphics::ShaderManager>(shader_manager, "shader_manager");
  scripting_system->addGlobalObject<Graphics::TextureManager>(texture_manager, "texture_manager");
  scripting_system->addGlobalObject<Sound::SoundSystem>(sound_system, "sound_system");
  
  //Initialize font generator
  font_generator = std::make_shared<Graphics::UI::FontGenerator>(config_manager->getString("fonts_location"), config_manager->getInt("pixels_per_unit"));

  scripting_system->addGlobalObject<Graphics::UI::FontGenerator>(font_generator, "font_generator");

  viewport_tile_width = config_manager->getFloat("screen_width_tiles");
  viewport_tile_height = config_manager->getFloat("screen_height_tiles");

  //Initizlie camera
  camera_component = std::make_shared<Graphics::Camera>(shader_manager, viewport_tile_width, viewport_tile_height, config_manager->getFloat("near_plane"), config_manager->getFloat("far_plane"));
  camera_component->setActive(true);
  camera_component->setFreeCameraSpeed(config_manager->getFloat("camera_speed"));

  scripting_system->addGlobalObject<Graphics::Camera>(camera_component, "camera");

  component_manager->addComponent(camera_component);
  //Initialize input system
  input_system = std::make_shared<Input::InputSystem>(graphics_system->getWindow(), viewport_tile_width, viewport_tile_height, camera_component->getTransform()->getAbsoluteTransformationMatrix(), camera_component->getProjectionMatrix());
  scripting_system->addGlobalObject<Input::InputSystem>(input_system, "input_system");

  //Initialize fps counter
  fps_counter = std::make_shared<Utility::FPSCounter>(60.0f);
  scripting_system->addGlobalObject<Utility::FPSCounter>(fps_counter, "fps_counter");


  scripting_system->loadScripts();

  camera_component->getTransform()->translate(glm::vec2(config_manager->getFloat("camera_x"), config_manager->getFloat("camera_y")));
}

void Engine::mainLoop() {

  float delta = 0.0f;
  graphics_system->startRender();
  scripting_system->start();
  component_manager->onStart();

  while(graphics_system->isRunning() && !time_to_exit) {
    graphics_system->startFrame();
    component_manager->onUpdate(delta);
    graphics_system->stopFrame();
    sound_system->update(delta);
    input_system->pollForInput();
    scripting_system->update(delta);
    delta = fps_counter->assessCountAndGetDelta();
  }
}

void Engine::cleanUp() {
   graphics_system->destroy();
   component_manager->destroy();
   scripting_system->destroy();
   sound_system->destroy();
}