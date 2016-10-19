#ifndef ENGINE_H
#define ENGINE_H

#define GLFW_INCLUDE_GLCOREARB

#include <memory>
#include "component_manager.h"
#include "utility/fps_counter.h"
#include "input/input_system.h"
#include "graphics/ui/font_generator.h"
#include "graphics/graphics_system.h"
#include "graphics/shader_manager.h"
#include "graphics/texture_manager.h"
#include "utility/config_manager.h"
#include "game/scene.h"
#include "utility/debug_parser.h"
#include "graphics/ui/text_area.h"
#include "script/scripting_system.h"

//= SCRIPTABLE

class Engine : public std::enable_shared_from_this<Engine> {
  private:
    std::shared_ptr<ComponentManager> component_manager;
    std::shared_ptr<Utility::FPSCounter> fps_counter;
    std::shared_ptr<Input::InputSystem> input_system;
    std::shared_ptr<Graphics::UI::FontGenerator> font_generator;
    std::shared_ptr<Graphics::GraphicsSystem> graphics_system;
    std::shared_ptr<Graphics::ShaderManager> shader_manager;
    std::shared_ptr<Graphics::TextureManager> texture_manager;
    std::shared_ptr<Utility::ConfigManager> config_manager;
    std::shared_ptr<Script::ScriptingSystem> scripting_system;
    
    //Bool represents if the scene is active;
    std::map<std::shared_ptr<Game::Scene>, bool> scenes;
    
    float viewport_tile_width;
    float viewport_tile_height;

    bool time_to_exit;
    std::shared_ptr<Graphics::Camera> camera_component;

  public:
    Engine();
    virtual ~Engine() = default;

    //= BEGIN SCRIPTABLE
    void addScene(std::shared_ptr<Game::Scene> scene) noexcept;
    std::shared_ptr<Game::Scene> findSceneByName(const std::string& name) noexcept;

    std::map<std::shared_ptr<Game::Scene>, bool> getScenes() const noexcept;

    void activateScene(const std::string& name);
    void deactivateScene(const std::string& name);

    const std::vector<std::string> getSceneNames() const noexcept;

    const std::vector<std::string> getActiveSceneNames() const noexcept;

    void timeToExit() noexcept;
    //= END SCRIPTABLE

    void setup(const std::string config_path);
    void mainLoop();
    void cleanUp();

};

#endif
