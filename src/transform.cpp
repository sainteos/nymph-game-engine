#include <easylogging++.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include "transform.h"
#include "exceptions/child_does_not_exist_exception.h"

Transform::Transform() {
  local_translation = glm::vec3(0.0);
  local_rotation = glm::quat(glm::mat4(1.0));
  local_scale = glm::vec3(1.0, 1.0, 1.0);
}

Transform::~Transform() {
  if(parent.lock() != nullptr) {
    auto new_end = std::remove_if(parent.lock()->children.begin(), parent.lock()->children.end(), [&](const std::shared_ptr<Transform>& transform) { return this == transform.get();});
    parent.lock()->children.erase(new_end, parent.lock()->children.end());
    parent.reset();
  }

  local_translation = glm::vec3(0.0);
  local_rotation = glm::quat(glm::mat4(1.0));
  local_scale = glm::vec3(1.0, 1.0, 1.0);
  
  for(auto& child : children) {
    child.reset(new Transform());
  }
  children.clear();
}

const bool Transform::operator==(const Transform& other) {
  return local_translation == other.local_translation &&
         local_rotation == other.local_rotation &&
         local_scale == other.local_scale;
}

const bool Transform::operator!=(const Transform& other) {
  return !(*this == other);
}

Transform Transform::operator*(const Transform& other) const {
  Transform t;
  t.local_translation = local_translation + other.local_translation;
  t.local_rotation = glm::slerp(local_rotation, other.local_rotation, 0.5f);
  t.local_scale = local_scale * other.local_scale;
  return t;
}

const unsigned int Transform::getTreeSize() const {
  unsigned int size = 1;
  for(auto& child : children)
    size += child->getTreeSize();
  return size;
}

template<>
void Transform::translate(const glm::vec2& translation) {
  local_translation += glm::vec3(translation, 0.0f);
}

template<>
void Transform::translate(const glm::vec3& translation) {
  local_translation += translation;
}

const glm::vec3 Transform::getAbsoluteTranslation() const noexcept {
  if(parent.lock() != nullptr) 
    return local_translation + parent.lock()->getAbsoluteTranslation();
  else
    return local_translation;
}

const glm::vec3 Transform::getLocalTranslation() const noexcept {
  return local_translation;
}

void Transform::rotate(const float angle, const glm::vec3& axis) {
  glm::quat quaternion = glm::angleAxis(angle, axis);
  local_rotation = quaternion * local_rotation;
}

void Transform::rotate(const float angle_x, const float angle_y, const float angle_z) {
  glm::quat quaternion = glm::quat(glm::vec3(angle_x, angle_y, angle_z));
  local_rotation = quaternion * local_rotation;
}

void Transform::rotate(const glm::vec3& euler_angles) {
  glm::quat quaternion = glm::quat(euler_angles);
  local_rotation = quaternion * local_rotation;
}

void Transform::rotate(const glm::quat& quat) {
  local_rotation = quat * local_rotation;
}

const glm::quat Transform::getAbsoluteRotation() const noexcept {
  if(parent.lock() != nullptr)
    return parent.lock()->getAbsoluteRotation() * local_rotation;
  else
    return local_rotation;
}

const float Transform::getAbsoluteRotationAngle() const noexcept {
  return glm::angle(getAbsoluteRotation());
}

const glm::vec3 Transform::getAbsoluteRotationAxis() const noexcept {
  return glm::axis(getAbsoluteRotation());
}

const glm::vec3 Transform::getAbsoluteEulerAngles() const noexcept {
  return glm::eulerAngles(getAbsoluteRotation());
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
}

template<>
void Transform::scale(const glm::vec2& scale) {
  local_scale *= glm::vec3(scale, 1.0);
}

template<>
void Transform::scale(const float& scale) {
  local_scale *= glm::vec3(scale);
}

const glm::vec3 Transform::getAbsoluteScale() const noexcept {
  if(parent.lock() != nullptr)
    return local_scale * parent.lock()->getAbsoluteScale();
  else
    return local_scale;
}

const glm::vec3 Transform::getLocalScale() const noexcept {
  return local_scale;
}

const glm::mat4 Transform::getAbsoluteTransformationMatrix() const noexcept {
  if(parent.lock() != nullptr)
    return getLocalTransformationMatrix() * parent.lock()->getAbsoluteTransformationMatrix();
  else
    return getLocalTransformationMatrix();
}

const glm::mat4 Transform::getLocalTransformationMatrix() const noexcept {
  glm::mat4 out = glm::translate(glm::mat4(1.0), local_translation);
  out = out * glm::mat4_cast(local_rotation);
  out = out * glm::scale(glm::mat4(1.0), local_scale);
  return out;
}

void Transform::addChild(std::shared_ptr<Transform> transform) {
  transform->parent = shared_from_this();
  children.push_back(transform);
}

void Transform::removeChild(std::shared_ptr<Transform> transform) {
  transform->parent.reset();
  children.remove(transform);
}

std::list<std::shared_ptr<Transform>> Transform::getChildren() const {
  return children;
}
