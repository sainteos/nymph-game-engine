#include "component.h"
#include "set_active_event.h"
#include "set_entity_active_event.h"
#include "entity.h"
#include <easylogging++.h>
#include <chaiscript/utility/utility.hpp>


unsigned int Component::next_id = 0;

Component::Component() : active(true), id(next_id) {
  transform = std::make_shared<Transform>();
  next_id++;
}

void Component::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
  switch(event->getEventType()) {
    case Events::EventType::SET_ACTIVE: {
      auto casted_event = std::static_pointer_cast<SetActiveEvent>(event);
      setActive(casted_event->getActive());
      break;
    }
    case Events::EventType::SET_ENTITY_ACTIVE: {
      auto casted_event = std::static_pointer_cast<SetEntityActiveEvent>(event);
      if(entity.lock() != nullptr) {
        entity.lock()->setActive(casted_event->getActive());
      }
      break;
    }
    default:
      break;
  }
}

void Component::onNotifyNow(std::shared_ptr<Events::Event> event) {
  handleQueuedEvent(event);
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

const bool Component::operator<(Component& other) noexcept {
  return getValueForSorting() < other.getValueForSorting();
}

const std::string Component::to_string() const noexcept {
  std::stringstream str;
  str<<className()<<":: id: "<<getId()<<"  active: "<<isActive()<<"  "<<getTransform()->to_string()<<"  sort value: "<<getValueForSorting();
  return str.str();
}

const std::string Component::className() const noexcept {
  return "Component";
}

void Component::log(el::base::type::ostream_t& os) const {
  os << to_string();
}
