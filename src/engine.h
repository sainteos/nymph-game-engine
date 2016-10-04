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
#include "scene.h"
#include "utility/debug_parser.h"
#include "events/subject.h"
#include "events/observer.h"
#include "graphics/ui/text_area.h"
#include "sprite_movement.h"

class Engine : public std::enable_shared_from_this<Engine>, public Events::Subject, public Events::Observer {
  private:
    std::shared_ptr<ComponentManager> component_manager;
    std::shared_ptr<Utility::FPSCounter> fps_counter;
    std::shared_ptr<Input::InputSystem> input_system;
    std::shared_ptr<Graphics::UI::FontGenerator> font_generator;
    std::shared_ptr<Graphics::GraphicsSystem> graphics_system;
    std::shared_ptr<Graphics::ShaderManager> shader_manager;
    std::shared_ptr<Graphics::TextureManager> texture_manager;
    std::shared_ptr<Utility::ConfigManager> config_manager;
    
    //Bool represents if the scene is active;
    std::map<std::shared_ptr<Scene>, bool> scenes;
    
    float viewport_tile_width;
    float viewport_tile_height;

    bool time_to_exit;
    bool free_camera;

    std::shared_ptr<Utility::DebugParser> debug;
    std::shared_ptr<Entity> debug_console;
    std::shared_ptr<Entity> map_list_ui;
    std::shared_ptr<Graphics::UI::TextArea> map_list_text_area;
    std::shared_ptr<Entity> character_list_ui;
    std::shared_ptr<Entity> layer_list_ui;
    std::shared_ptr<Graphics::UI::TextArea> name_area;

    std::shared_ptr<SpriteMovement> sprite_movement;
    std::shared_ptr<Graphics::Camera> camera_component;

    std::shared_ptr<Scene> findSceneByName(const std::string& name) noexcept;

    void activateScene(const std::string& name);
    void deactivateScene(const std::string& name);
  public:
    Engine();
    void setup(const std::string config_path);
    void mainLoop();
    void cleanUp();


    virtual void onNotifyNow(std::shared_ptr<Events::Event> event);
    virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event);

};

#endif
