#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include <glm/glm.hpp>
#include <map>
#include <queue>
#include <glfw3.h>
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"

namespace Input {
  class InputSystem : public Events::Subject, public Events::Observer {
    private:
      static std::map<int, int> keys_to_actions;
      static std::map<int, int> mouse_buttons_to_actions;
      static glm::dvec2 cursor_position;
      static glm::dvec2 scroll_offset;
      static bool cursor_entered;
      static bool cursor_left;
      static std::queue<unsigned char> character_typed_buffer;
      static bool key_input_suspended;

      std::map<int, int> last_keys_to_actions;
      std::map<int, int> last_mouse_buttons_to_actions;

      glm::mat4 camera_transform_matrix;
      glm::mat4 projection_matrix;
      int window_width;
      int window_height;
      float viewport_width;
      float viewport_height;

      static void keyCallback(GLFWwindow* window, int key, int scan_code, int action, int mods);
      static void characterCallback(GLFWwindow* window, unsigned int key);
      static void cursorPositionCallback(GLFWwindow* window, double x_pos, double y_pos);
      static void cursorEnterCallback(GLFWwindow* window, int entered);
      static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
      static void mouseScrollCallback(GLFWwindow* window, double x_offset, double y_offset);
    public:
      InputSystem(GLFWwindow* window, float viewport_width, float viewport_height, glm::mat4 camera_transform_matrix, glm::mat4 projection_matrix);
      void pollForInput();

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
  };
}

#endif
