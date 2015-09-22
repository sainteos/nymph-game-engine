#include "component.h"


Component::Component() : transform(std::make_shared<Transform>()) {

}

void Component::setTransform(std::shared_ptr<Transform> transform) {
  this->transform = transform;
}

std::shared_ptr<Transform> Component::getTransform() const noexcept {
  return transform;
}