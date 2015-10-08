#include "component.h"
#include "set_active_event.h"
#include <easylogging++.h>


unsigned int Component::next_id = 0;

Component::Component() : active(false), id(next_id) {
  next_id++;
}

const bool Component::onUpdate(const double delta) {
  std::queue<std::shared_ptr<Events::Event>> unused;
  while(!events.empty()) {
    auto event = events.front();
    events.pop();
    switch(event->getEventCode()) {
      case Events::EventType::SET_ACTIVE: {
        auto casted_event = std::static_pointer_cast<SetActiveEvent>(event);
        if(casted_event->getComponentId() == id) {
          setActive(casted_event->getActive());
        }
      }
      default:
        unused.push(event);
    }
  }

  events.swap(unused);
  return true;
}

void Component::setTransform(std::shared_ptr<Transform> transform) {
  this->transform = transform;
}

std::shared_ptr<Transform> Component::getTransform() const noexcept {
  return transform.lock();
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

const bool Component::operator<(const Component& other) const noexcept {
  return id < other.id;
}
