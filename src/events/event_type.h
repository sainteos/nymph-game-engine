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
    SPRITE_MOVE,
    SET_SHADER,
    ADD_TEXTURE,
    REMOVE_TEXTURE,
    SET_ACTIVE,
    SET_UNIFORM,
    ANIMATION_TRIGGER
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
      {SPRITE_MOVE, "Sprite Move"},
      {SET_SHADER, "Set Shader"},
      {ADD_TEXTURE, "Add Texture"},
      {REMOVE_TEXTURE, "Remove Texture"},
      {SET_ACTIVE, "Set Active"},
      {SET_UNIFORM, "Set Uniform"},
      {ANIMATION_TRIGGER, "Animation Trigger"}
    };
  }
}

#endif
