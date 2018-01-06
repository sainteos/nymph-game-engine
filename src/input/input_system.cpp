#include <easylogging++.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "input_system.h"
#include "cursor_enter_event.h"
#include "cursor_leave_event.h"
#include "key_down_event.h"
#include "key_up_event.h"
#include "key_repeat_event.h"
#include "mouse_button_up_event.h"
#include "mouse_button_down_event.h"
#include "mouse_cursor_event.h"
#include "mouse_scroll_event.h"
#include "character_typed_event.h"

namespace Input {
  std::map<int, int> InputSystem::keys_to_actions {};
  std::map<int, int> InputSystem::mouse_buttons_to_actions {};
  glm::dvec2 InputSystem::cursor_position = glm::dvec2(0.0, 0.0);
  glm::dvec2 InputSystem::scroll_offset = glm::dvec2(0.0, 0.0);
  bool InputSystem::cursor_entered = false;
  bool InputSystem::cursor_left = false;
  std::queue<unsigned char> InputSystem::character_typed_buffer = std::queue<unsigned char>();
  bool InputSystem::key_input_suspended = false;

  InputSystem::InputSystem(GLFWwindow* window, float viewport_width, float viewport_height, glm::mat4 camera_transform_matrix, glm::mat4 projection_matrix) : viewport_width(viewport_width), viewport_height(viewport_height), camera_transform_matrix(camera_transform_matrix), projection_matrix(projection_matrix) {
    glfwSetKeyCallback(window, InputSystem::keyCallback);
    glfwSetCharCallback(window, InputSystem::characterCallback);
    glfwSetCursorPosCallback(window, InputSystem::cursorPositionCallback);
    glfwSetCursorEnterCallback(window, InputSystem::cursorEnterCallback);
    glfwSetMouseButtonCallback(window, InputSystem::mouseButtonCallback);
    glfwSetScrollCallback(window, InputSystem::mouseScrollCallback);
    int width, height;

    glfwGetWindowSize(window, &width, &height);
    window_width = width;
    window_height = height;
  }

  void InputSystem::pollForInput() {
    while(!character_typed_buffer.empty()) {
      notify(CharacterTypedEvent::create(character_typed_buffer.front()));
      character_typed_buffer.pop();
    }
    if(cursor_position != glm::dvec2(0.0, 0.0)) {

      //Transform mouse cursor into GL Space first
      auto transformed_cursor_3d = glm::unProject(glm::vec3(cursor_position, 0.0), camera_transform_matrix, projection_matrix, glm::vec4(0.0, 0.0, window_width, window_height));
      auto transformed_cursor = glm::vec2(transformed_cursor_3d.x * viewport_width / 2.0, -transformed_cursor_3d.y * viewport_height / 2.0);
      notify(MouseCursorEvent::create(transformed_cursor));;
      cursor_position = glm::dvec2(0.0, 0.0);
    }

    if(scroll_offset != glm::dvec2(0.0, 0.0)) {
      notify(MouseScrollEvent::create(scroll_offset));
      scroll_offset = glm::dvec2(0.0, 0.0);
    }

    if(cursor_entered) {
      notify(CursorEnterEvent::create());
      cursor_entered = false;
    }

    if(cursor_left) {
      notify(CursorLeaveEvent::create());
      cursor_left = false;
    }

    for(auto& key : keys_to_actions) {
      if(key.second == GLFW_PRESS && last_keys_to_actions.count(key.first) == 0 ||
         key.second == GLFW_PRESS && last_keys_to_actions.count(key.first) > 0 && last_keys_to_actions[key.first] != key.second && last_keys_to_actions[key.first] != GLFW_REPEAT) {
        notify(KeyDownEvent::create(key.first));
      }
      else if(key.second == GLFW_RELEASE && last_keys_to_actions.count(key.first) == 0 ||
         key.second == GLFW_RELEASE && last_keys_to_actions.count(key.first) > 0 && last_keys_to_actions[key.first] != key.second) {
        notify(KeyUpEvent::create(key.first));
      }
      else if(key.second == GLFW_REPEAT) {
        notify(KeyRepeatEvent::create(key.first));
      }
    }

    last_keys_to_actions = keys_to_actions;
    keys_to_actions.clear();

    for(auto& button : mouse_buttons_to_actions) {
      if(button.second == GLFW_PRESS && last_mouse_buttons_to_actions.count(button.first) == 0 ||
         button.second == GLFW_PRESS && last_mouse_buttons_to_actions.count(button.first) > 0 && last_mouse_buttons_to_actions[button.first] != button.second) {
        notify(MouseButtonDownEvent::create(button.first));
      }
      else if(button.second == GLFW_RELEASE && last_mouse_buttons_to_actions.count(button.first) == 0 ||
         button.second == GLFW_RELEASE && last_mouse_buttons_to_actions.count(button.first) > 0 && last_mouse_buttons_to_actions[button.first] != button.second) {
        notify(MouseButtonUpEvent::create(button.first));
      }
    }

    last_mouse_buttons_to_actions = mouse_buttons_to_actions;
    mouse_buttons_to_actions.clear();

    glfwPollEvents();
  }

  void InputSystem::keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods) {
    if(!key_input_suspended || key_input_suspended && key > GLFW_KEY_GRAVE_ACCENT) {
      keys_to_actions[key] = action;
    }
  }

  void InputSystem::characterCallback(GLFWwindow* window, unsigned int key) {
    character_typed_buffer.push((unsigned char)key);
  }

  void InputSystem::cursorPositionCallback(GLFWwindow* window, double x_pos, double y_pos) {
    cursor_position = glm::dvec2(x_pos, y_pos);
  }

  void InputSystem::cursorEnterCallback(GLFWwindow* window, int entered) {
    cursor_entered = (bool)entered;
    cursor_left = !(bool)entered;
  }

  void InputSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    mouse_buttons_to_actions[button] = action;
  }

  void InputSystem::mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset) {
    scroll_offset = glm::dvec2(x_offset, y_offset);
  }

  void InputSystem::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void InputSystem::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    if(event->getEventType() == Events::EventType::SUSPEND_KEY_INPUT) {
      key_input_suspended = true;
    }
    else if(event->getEventType() == Events::EventType::RESUME_KEY_INPUT) {
      key_input_suspended = false;
    }
  }
}