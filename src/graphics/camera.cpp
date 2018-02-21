#include <easylogging++.h>
#include <sstream>
#include <iomanip>
#include <glm/ext.hpp>
#include "camera.h"
#include "shader_manager.h"
#include "game/sprite_move_event.h"
#include "input/key_down_event.h"
#include "input/key_repeat_event.h"
#include "input/key_up_event.h"
#include "ui/change_text_event.h"
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>

namespace Graphics {

  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager) : shader_manager(shader_manager), projection_matrix(1.0), screen_padding_in_tiles(2), free_camera(false), free_camera_speed(1.0) {

  }

  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near, const float far)
    : viewport_width(viewport_width), viewport_height(viewport_height), near(near), far(far), projection_matrix(1.0), shader_manager(shader_manager), screen_padding_in_tiles(2), free_camera_speed(1.0) {
  }

  void Camera::onStart() {
    projection_matrix = glm::ortho(viewport_width / -2.0f, viewport_width / 2.0f, viewport_height / -2.0f, viewport_height / 2.0f, near, far);
    shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);

    shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(getTransform()).getAbsoluteTransformationMatrix());
    last_projection_matrix = projection_matrix;
    last_transform = *getTransform();
    target_position = glm::vec2(getTransform()->getLocalTranslation());
    velocity = glm::vec2(0.0, 0.0);
    std::stringstream pos_string;

    pos_string << getTransform()->getAbsoluteTranslation().x << ", "<<getTransform()->getAbsoluteTranslation().y;

    notify(Graphics::UI::ChangeTextEvent::create(pos_string.str()));
  }

  bool Camera::onUpdate(const double delta) {
    std::stringstream pos_string;

    pos_string << std::fixed << std::setprecision(2) << getTransform()->getAbsoluteTranslation().x << ", "<<getTransform()->getAbsoluteTranslation().y;

    notify(Graphics::UI::ChangeTextEvent::create(pos_string.str()));

    if(!active)
      return false;

    if(free_camera) {
      getTransform()->translate(velocity * 1.0 / 1000.0 * delta);
    }
    else {
      if(velocity != glm::vec2(0.0, 0.0)) {
        if(glm::distance(target_position, glm::vec2(getTransform()->getLocalTranslation())) < 1.0 / 1000.0 * delta) {
          getTransform()->translate(target_position - glm::vec2(getTransform()->getLocalTranslation()));
          velocity = glm::vec2(0.0, 0.0);
        }
        else {
          getTransform()->translate(velocity * 1.0 / 1000.0 * delta);
        }
      }
    }

    if(projection_matrix != last_projection_matrix) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);
      last_projection_matrix = projection_matrix;
    }

    if(last_transform != *getTransform()) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(getTransform()).getAbsoluteTransformationMatrix());
      last_transform = *getTransform();
    }
    return true;
  }

  void Camera::onDestroy() {
  }

  void Camera::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    switch(event->getEventType()) {
      case Events::EventType::SPRITE_MOVE: {
        free_camera = false;
        auto casted_event = std::static_pointer_cast<Game::SpriteMoveEvent>(event);

        if(casted_event->getVelocity().x > 0.0) {
          if(casted_event->getNextPosition().x > getTransform()->getAbsoluteTranslation().x + viewport_width / 2.0 - screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(1.0, 0.0);
          }
        }
        else if(casted_event->getVelocity().x < 0.0) {
          if(casted_event->getNextPosition().x < getTransform()->getAbsoluteTranslation().x - viewport_width / 2.0 + screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(-1.0, 0.0);
          }
        }
        else if(casted_event->getVelocity().y > 0.0) {
          if(casted_event->getNextPosition().y > getTransform()->getAbsoluteTranslation().y + viewport_height / 2.0 - screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(0.0, 1.0);
          }
        }
        else if(casted_event->getVelocity().y < 0.0) {
          if(casted_event->getNextPosition().y < getTransform()->getAbsoluteTranslation().y - viewport_height / 2.0 + screen_padding_in_tiles) {
            velocity = casted_event->getVelocity();
            target_position = glm::vec2(getTransform()->getAbsoluteTranslation()) + glm::vec2(0.0, -1.0);
          }
        }
        break;
      }
      case Events::EventType::KEY_DOWN: {
        free_camera = true;
        auto casted_event = std::static_pointer_cast<Input::KeyDownEvent>(event);
        if(casted_event->getKey() == GLFW_KEY_W) {
          velocity = glm::vec2(velocity.x, free_camera_speed);
        }
        else if(casted_event->getKey() == GLFW_KEY_A) {
          velocity = glm::vec2(-free_camera_speed, velocity.y);
        }
        else if(casted_event->getKey() == GLFW_KEY_S) {
          velocity = glm::vec2(velocity.x, -free_camera_speed);
        }
        else if(casted_event->getKey() == GLFW_KEY_D) {
          velocity = glm::vec2(free_camera_speed, velocity.y);
        }
        break;
      }
      case Events::EventType::KEY_REPEAT: {
        auto casted_event = std::static_pointer_cast<Input::KeyRepeatEvent>(event);
        if(casted_event->getKey() == GLFW_KEY_W) {
          velocity = glm::vec2(velocity.x, free_camera_speed);
        }
        else if(casted_event->getKey() == GLFW_KEY_A) {
          velocity = glm::vec2(-free_camera_speed, velocity.y);
        }
        else if(casted_event->getKey() == GLFW_KEY_S) {
          velocity = glm::vec2(velocity.x, -free_camera_speed);
        }
        else if(casted_event->getKey() == GLFW_KEY_D) {
          velocity = glm::vec2(free_camera_speed, velocity.y);
        }
        break;
      }
      case Events::EventType::KEY_UP: {
        auto casted_event = std::static_pointer_cast<Input::KeyUpEvent>(event);
        if(casted_event->getKey() == GLFW_KEY_W) {
          velocity = glm::vec2(velocity.x, 0.0);
        }
        else if(casted_event->getKey() == GLFW_KEY_A) {
          velocity = glm::vec2(0.0, velocity.y);
        }
        else if(casted_event->getKey() == GLFW_KEY_S) {
          velocity = glm::vec2(velocity.x, 0.0);
        }
        else if(casted_event->getKey() == GLFW_KEY_D) {
          velocity = glm::vec2(0.0, velocity.y);
        }
        break;
      }

      default:
        Component::handleQueuedEvent(event);
    }
  }

  void Camera::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void Camera::setScreenPaddingInTiles(const int padding) noexcept {
    screen_padding_in_tiles = padding;
  }

  int Camera::getScreenPaddingInTiles() const noexcept {
    return screen_padding_in_tiles;
  }

  void Camera::setWidth(const float width) noexcept {
    viewport_width = width;
  }

  float Camera::getWidth() const noexcept {
    return viewport_width;
  }

  void Camera::setHeight(const float height) noexcept {
    viewport_height = height;
  }

  float Camera::getHeight() const noexcept {
    return viewport_height;
  }

  void Camera::setNear(const float near) noexcept {
    this->near = near;
  }

  float Camera::getNear() const noexcept {
    return near;
  }

  void Camera::setFar(const float far) noexcept {
    this->far = far;
  }

  float Camera::getFar() const noexcept {
    return far;
  }

  void Camera::setFreeCameraSpeed(const float speed) noexcept {
    this->free_camera_speed = speed;
  }

  float Camera::getFreeCameraSpeed() const noexcept {
    return this->free_camera_speed;
  }

  glm::mat4 Camera::getProjectionMatrix() const noexcept {
    return projection_matrix;
  }

  unsigned long long Camera::getValueForSorting() const noexcept {
    return getId();
  }

  bool Camera::isComponentWithin(const Component& component) const {
    auto translation = component.getTransform()->getAbsoluteTranslation();
    auto camera_translation = getTransform()->getAbsoluteTranslation();

    auto camera_bound_left = -(viewport_width + 2.0) / 2.0 + camera_translation.x;
    auto camera_bound_right = (viewport_width + 2.0) / 2.0 + camera_translation.x;
    auto camera_bound_bottom = -(viewport_height + 2.0) / 2.0 + camera_translation.y;
    auto camera_bound_top = (viewport_height + 2.0) / 2.0 + camera_translation.y;

    return camera_bound_left <= translation.x &&
           camera_bound_right >= translation.x &&
           camera_bound_bottom <= translation.y &&
           camera_bound_top >= translation.y;
  }

  Transform Camera::negateTransformForScreen(std::shared_ptr<Transform> trans) {
    //Gotta do this to make the camera move the opposite the renderables
    Transform negated_for_screen = *trans;
    negated_for_screen.translate(glm::vec3(-2.0, -2.0, 1.0) * negated_for_screen.getAbsoluteTranslation());
    return negated_for_screen;
  }

  void Camera::log(el::base::type::ostream_t& os) const {
    os << "Viewport Width: "<<viewport_width<<" Viewport Height: "<<viewport_height<<" Near: "<<near<<" Far: "<<far<<" Velocity: "<<glm::to_string(velocity)<<" Target Position: "<<glm::to_string(target_position)<<" Free Camera: "<<free_camera<<" Speed: "<<free_camera_speed;
    Component::log(os);
  }

  std::string Camera::className() const noexcept  {
    return "Graphics::Camera";
  }
}
