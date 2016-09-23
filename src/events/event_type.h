#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H
#include <map>
#include <string>

namespace Events {
  enum EventType {
    MOUSE_CURSOR,
    MOUSE_SCROLL,
    MOUSE_BUTTON_DOWN,
    MOUSE_BUTTON_UP,
    CURSOR_ENTER,
    CURSOR_LEAVE,
    KEY_DOWN,
    KEY_UP,
    KEY_REPEAT,
    CHARACTER_TYPED,
    SPRITE_MOVE,
    SET_SHADER,
    ADD_TEXTURE,
    REMOVE_TEXTURE,
    SET_ACTIVE,
    SET_UNIFORM,
    ANIMATION_TRIGGER,
    SUSPEND_KEY_INPUT,
    RESUME_KEY_INPUT,
    DEBUG_COMMAND,
    LOAD_MAP,
    LOAD_CHARACTER,
    TOGGLE_FREE_CAMERA,
    LIST_MAPS,
    LIST_CHARACTERS,
    LIST_LAYERS,
    TOGGLE_LIGHTS,
    TOGGLE_LAYER
  };

  inline std::map<EventType, std::string> EventTypesToString() {
    return std::map<EventType, std::string> {
      {MOUSE_CURSOR, "Mouse Cursor"},
      {MOUSE_SCROLL, "Mouse Scroll"},
      {MOUSE_BUTTON_DOWN, "Mouse Button Down"},
      {MOUSE_BUTTON_UP, "Mouse Button Up"},
      {CURSOR_ENTER, "Cursor Enter"},
      {CURSOR_LEAVE, "Cursor Leave"},
      {KEY_DOWN, "Key Down"},
      {KEY_UP, "Key Up"},
      {KEY_UP, "Key Repeat"},
      {CHARACTER_TYPED, "Character Typed"},
      {SPRITE_MOVE, "Sprite Move"},
      {SET_SHADER, "Set Shader"},
      {ADD_TEXTURE, "Add Texture"},
      {REMOVE_TEXTURE, "Remove Texture"},
      {SET_ACTIVE, "Set Active"},
      {SET_UNIFORM, "Set Uniform"},
      {ANIMATION_TRIGGER, "Animation Trigger"},
      {SUSPEND_KEY_INPUT, "Suspend Key Input"},
      {RESUME_KEY_INPUT, "Resume Key Input"},
      {DEBUG_COMMAND, "Debug Command"},
      {LOAD_MAP, "Load Map"},
      {LOAD_CHARACTER, "Load Character"},
      {TOGGLE_FREE_CAMERA, "Toggle Free Camera"},
      {LIST_MAPS, "List Maps"},
      {LIST_CHARACTERS, "List Characters"},
      {LIST_LAYERS, "List Layers"},
      {TOGGLE_LIGHTS, "Toggle Lights"},
      {TOGGLE_LAYER, "Toggle Layer"}
    };
  }
}

#endif
