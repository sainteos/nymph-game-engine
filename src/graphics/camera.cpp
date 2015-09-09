#include <easylogging++.h>
#include "camera.h"
#include "glm/ext.hpp"
#include "graphics/shader_manager.h"
#include "sprite_move_event.h"

namespace Graphics {
  
  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager) : shader_manager(shader_manager), projection_matrix(1.0), screen_padding_in_tiles(2) {

  }

  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near, const float far) 
    : viewport_width(viewport_width), viewport_height(viewport_height), near(near), far(far), projection_matrix(1.0), shader_manager(shader_manager), screen_padding_in_tiles(2) {
  }

  void Camera::onStart() { 
    projection_matrix = glm::ortho(viewport_width / -2.0f, viewport_width / 2.0f, viewport_height / -2.0f, viewport_height / 2.0f, near, far);
    shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);

    shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(transform).getAbsoluteTransformationMatrix());
    last_projection_matrix = projection_matrix;
    last_transform = *transform;
    target_position = glm::vec2(transform->getLocalTranslation());
    velocity = glm::vec2(0.0, 0.0);
  }

  const bool Camera::onUpdate(const double delta) {
    if(velocity != glm::vec2(0.0, 0.0)) {
      if(glm::distance(target_position, glm::vec2(transform->getLocalTranslation())) < 1.0 / 1000.0 * delta) {
        transform->translate(target_position - glm::vec2(transform->getLocalTranslation()));
        velocity = glm::vec2(0.0, 0.0);
      }
      else {
        transform->translate(velocity * 1.0 / 1000.0 * delta);
      }
    }
    if(projection_matrix != last_projection_matrix) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);
      last_projection_matrix = projection_matrix;
    }

    if(last_transform != *transform) {
      shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(transform).getAbsoluteTransformationMatrix());
      last_transform = *transform;
    }
    return true;
  }

  void Camera::onDestroy() {
    transform = std::make_shared<Transform>();
  }

  void Camera::onNotify(const Events::Event& event) {
    if(event.getEventCode() == Events::EventType::SPRITE_MOVE) {
      auto casted_event = static_cast<const SpriteMoveEvent*>(&event);

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
    }
  }

  void Camera::setScreenPaddingInTiles(const int padding) noexcept {
    screen_padding_in_tiles = padding;
  }

  const int Camera::getScreenPaddingInTiles() const noexcept {
    return screen_padding_in_tiles;
  }

  void Camera::setWidth(const float width) noexcept {
    viewport_width = width;
  }

  const float Camera::getWidth() const noexcept {
    return viewport_width;
  }

  void Camera::setHeight(const float height) noexcept {
    viewport_height = height;
  }

  const float Camera::getHeight() const noexcept {
    return viewport_height;
  }

  void Camera::setNear(const float near) noexcept {
    this->near = near;
  }

  const float Camera::getNear() const noexcept {
    return near;
  }

  void Camera::setFar(const float far) noexcept {
    this->far = far;
  }

  const float Camera::getFar() const noexcept {
    return far;
  }

  const bool Camera::isRenderableWithin(std::shared_ptr<Renderable> renderable) const {
    auto translation = renderable->getTransform()->getAbsoluteTranslation();
    auto camera_translation = transform->getAbsoluteTranslation();
    
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
    Transform negated_for_screen = *transform;
    negated_for_screen.translate(glm::vec3(-2.0, -2.0, 1.0) * negated_for_screen.getAbsoluteTranslation());
    return negated_for_screen;
  }
}