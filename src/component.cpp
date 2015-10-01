#include "component.h"
#include "set_active_event.h"
#include <easylogging++.h>


unsigned int Component::next_id = 0;

Component::Component() : transform(std::make_shared<Transform>()), active(false), id(next_id) {
  next_id++;
}

void Component::setTransform(std::shared_ptr<Transform> transform) {
  this->transform = transform;
}

std::shared_ptr<Transform> Component::getTransform() const noexcept {
  return transform;
}

  
void Component::setActive(const bool active) noexcept {
  this->active = active;
}

const bool Component::isActive() const noexcept {
  return active;
}

const unsigned int Component::getId() const noexcept {
  return id;
}

void Component::onNotify(const Events::Event& event) {
  switch(event.getEventCode()) {
    case Events::EventType::SET_ACTIVE: {
      auto casted_event = static_cast<const SetActiveEvent*>(&event);
      if(casted_event->getComponentId() == id) {
        setActive(casted_event->getActive());
      }
    }
  }
}
