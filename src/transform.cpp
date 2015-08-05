#include <easylogging++.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include "transform.h"
#include "exceptions/child_does_not_exist_exception.h"

Transform::Transform() : parent(nullptr) {
  absolute_matrix = glm::mat4(1.0);
  local_translation = glm::vec3(0.0);
  absolute_translation = glm::vec3(0.0);
  local_rotation = glm::quat(glm::mat4(1.0));
  absolute_rotation = glm::quat(glm::mat4(1.0));
  local_scale = glm::vec3(1.0, 1.0, 1.0);
  absolute_scale = glm::vec3(1.0, 1.0, 1.0);
}

Transform::~Transform() {
}

const bool Transform::operator==(const Transform& other) {
  return absolute_matrix == other.absolute_matrix &&
         absolute_translation == other.absolute_translation && local_translation == other.local_translation &&
         absolute_rotation == other.absolute_rotation && local_rotation == other.local_rotation &&
         absolute_scale == other.absolute_scale && local_scale == other.local_scale;
}

const bool Transform::operator!=(const Transform& other) {
  return !(*this == other);
}

void Transform::addChild(const std::shared_ptr<Transform>& transform) {
  transform->parent = shared_from_this();
  transform->translate_absolute(absolute_translation + local_translation);
  transform->rotate_absolute(local_rotation * absolute_rotation);
  transform->scale_absolute(absolute_scale * local_scale);
  children.push_back(transform);
}

std::list<std::shared_ptr<Transform>> Transform::getChildren() const {
  return children;
}

const unsigned int Transform::getTreeSize() const {
  unsigned int size = 1;
  for(auto& child : children)
    size += child->getTreeSize();
  return size;
}

void Transform::translate_absolute(const glm::vec3& translation) {
  absolute_translation += translation;
  absolute_matrix = glm::translate(absolute_matrix, translation);

  for(auto& child : children) 
    child->translate_absolute(translation);
}

void Transform::rotate_absolute(const glm::quat& rotation) {
  absolute_rotation = rotation * absolute_rotation;
  absolute_matrix *= mat4_cast(rotation);

  for(auto& child : children) 
    child->rotate_absolute(rotation);
}

void Transform::scale_absolute(const glm::vec3& scale) {
  absolute_scale *= scale;
  absolute_matrix = glm::scale(absolute_matrix, scale);

  for(auto& child : children)
    child->scale_absolute(scale);
}

template<>
void Transform::translate(const glm::vec2& translation) {
  local_translation += glm::vec3(translation, 0.0f);
  
  for(auto& child : children)
    child->translate_absolute(glm::vec3(translation, 0.0f));
}

template<>
void Transform::translate(const glm::vec3& translation) {
  local_translation += translation;
  
  for(auto& child : children)
    child->translate_absolute(translation);
}

const glm::vec3 Transform::getAbsoluteTranslation() const noexcept {
  return absolute_translation;
}

const glm::vec3 Transform::getLocalTranslation() const noexcept {
  return local_translation;
}

void Transform::rotate(const float angle, const glm::vec3& axis) {
  glm::quat quaternion = glm::angleAxis(angle, axis);
  local_rotation = quaternion * local_rotation;

  for(auto& child : children)
    child->rotate_absolute(quaternion);
}

void Transform::rotate(const float angle_x, const float angle_y, const float angle_z) {
  glm::quat quaternion = glm::quat(glm::vec3(angle_x, angle_y, angle_z));
  local_rotation = quaternion * local_rotation;

  for(auto& child : children)
    child->rotate_absolute(quaternion);
}

void Transform::rotate(const glm::vec3& euler_angles) {
  glm::quat quaternion = glm::quat(euler_angles);
  local_rotation = quaternion * local_rotation;

  for(auto& child : children)
    child->rotate_absolute(quaternion);
}

void Transform::rotate(const glm::quat& quat) {
  local_rotation = quat;

  for(auto& child : children)
    child->rotate_absolute(quat);
}

const glm::quat Transform::getAbsoluteRotation() const noexcept {
  return absolute_rotation;
}

const float Transform::getAbsoluteRotationAngle() const noexcept {
  return glm::angle(absolute_rotation);
}

const glm::vec3 Transform::getAbsoluteRotationAxis() const noexcept {
  return glm::axis(absolute_rotation);
}

const glm::vec3 Transform::getAbsoluteEulerAngles() const noexcept {
  return glm::eulerAngles(absolute_rotation);
}

const glm::quat Transform::getLocalRotation() const noexcept {
  return local_rotation;
}

const float Transform::getLocalRotationAngle() const noexcept {
  return glm::angle(local_rotation);
}

const glm::vec3 Transform::getLocalRotationAxis() const noexcept {
  return glm::axis(local_rotation);
}

const glm::vec3 Transform::getLocalEulerAngles() const noexcept {
  return glm::eulerAngles(local_rotation);
}

template<>
void Transform::scale(const glm::vec3& scale) {
  local_scale *= scale;

  for(auto& child : children)
    child->scale_absolute(scale);
}

template<>
void Transform::scale(const glm::vec2& scale) {
  local_scale *= glm::vec3(scale, 1.0);

  for(auto& child : children)
    child->scale_absolute(glm::vec3(scale, 1.0));
}

template<>
void Transform::scale(const float& scale) {
  local_scale *= glm::vec3(scale);

  for(auto& child : children)
    child->scale_absolute(glm::vec3(scale));
}

const glm::vec3 Transform::getAbsoluteScale() const noexcept {
  return absolute_scale;
}

const glm::vec3 Transform::getLocalScale() const noexcept {
  return local_scale;
}

const glm::mat4 Transform::getAbsoluteTransformationMatrix() const noexcept {
  glm::mat4 out = absolute_matrix * glm::translate(glm::mat4(1.0), local_translation);
  out = out * glm::mat4_cast(local_rotation);
  out = out * glm::scale(glm::mat4(1.0), local_scale);
  return out;
}

const glm::mat4 Transform::getLocalTransformationMatrix() const noexcept {
  glm::mat4 out = glm::translate(glm::mat4(1.0), local_translation);
  out = out * glm::mat4_cast(local_rotation);
  out = out * glm::scale(glm::mat4(1.0), local_scale);
  return out;
}

const bool Transform::onUpdate(const double delta) {
  return true;
}

void Transform::destroy() {
  if(parent != nullptr) {
    auto new_end = std::remove_if(parent->children.begin(), parent->children.end(), [&](const std::shared_ptr<Transform>& transform) { return this == transform.get();});
    parent->children.erase(new_end, parent->children.end());
    parent = nullptr;
  }

  absolute_matrix = glm::mat4(1.0);
  local_translation = glm::vec3(0.0);
  absolute_translation = glm::vec3(0.0);
  local_rotation = glm::quat(glm::mat4(1.0));
  absolute_rotation = glm::quat(glm::mat4(1.0));
  local_scale = glm::vec3(1.0, 1.0, 1.0);
  absolute_scale = glm::vec3(1.0, 1.0, 1.0);
  
  for(auto& child : children) {
    child->parent = nullptr;
    child->destroy();
    child.reset(new Transform());
  }
  parent = nullptr;
  children.clear();
}