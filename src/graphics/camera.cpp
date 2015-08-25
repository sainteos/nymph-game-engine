#include <easylogging++.h>
#include "camera.h"
#include "glm/ext.hpp"
#include "graphics/shader_manager.h"

namespace Graphics {
  
  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager) : shader_manager(shader_manager), projection_matrix(1.0), transform(std::make_shared<Transform>()) {

  }

  Camera::Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near, const float far) 
    : viewport_width(viewport_width), viewport_height(viewport_height), near(near), far(far), projection_matrix(1.0), shader_manager(shader_manager), transform(std::make_shared<Transform>()) {
  }

  void Camera::onStart() { 
    projection_matrix = glm::ortho(viewport_width / -2.0f, viewport_width / 2.0f, viewport_height / -2.0f, viewport_height / 2.0f, near, far);
    shader_manager->setUniformForAllPrograms<glm::mat4>("projection", projection_matrix);

    shader_manager->setUniformForAllPrograms<glm::mat4>("view", negateTransformForScreen(transform).getAbsoluteTransformationMatrix());
    last_projection_matrix = projection_matrix;
    last_transform = *transform;
  }

  const bool Camera::onUpdate(const double delta) {
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

  void Camera::destroy() {
    transform = std::make_shared<Transform>();
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

  void Camera::setTransform(const std::shared_ptr<Transform> transform) noexcept {
    this->transform = transform;
  }

  std::shared_ptr<Transform> Camera::getTransform() const noexcept {
    return transform;
  }

  const bool Camera::isRenderableWithin(std::shared_ptr<Renderable> renderable) const {
    auto translation = renderable->getTransform()->getAbsoluteTranslation();
    auto camera_translation = transform->getAbsoluteTranslation();
    
    auto camera_bound_left = -(viewport_width + 1.0) / 2.0 + camera_translation.x;
    auto camera_bound_right = (viewport_width + 1.0) / 2.0 + camera_translation.x;
    auto camera_bound_bottom = -(viewport_height + 1.0) / 2.0 + camera_translation.y;
    auto camera_bound_top = (viewport_height + 1.0) / 2.0 + camera_translation.y;

    return camera_bound_left <= translation.x &&
           camera_bound_right >= translation.x &&
           camera_bound_bottom <= translation.y &&
           camera_bound_top >= translation.y;
  }

  Transform Camera::negateTransformForScreen(std::shared_ptr<Transform> trans) {
    //Gotta do this to make the camera move the opposite the renderables
    Transform negated_for_screen = *transform;
    negated_for_screen.translate(glm::vec3(-2.0, -2.0, 1.0) * negated_for_screen.getLocalTranslation());
    return negated_for_screen;
  }
}