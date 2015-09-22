#include "component.h"

Component::Component() : active(false), transform(std::make_shared<Transform>()) {
}

void Component::setTransform(std::shared_ptr<Transform> transform) {
  this->transform = transform;
}

std::shared_ptr<Transform> Component::getTransform() const noexcept {
  return transform;
}

void Component::setActive() noexcept {
  active = true;
}

void Component::setInactive() noexcept {
  active = false;
}

const bool Component::isActive() const noexcept {
  return active;
}