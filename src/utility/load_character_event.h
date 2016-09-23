#ifndef LOAD_CHARACTER_EVENT_H
#define LOAD_CHARACTER_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class LoadCharacterEvent : public Events::Event {
    private:
      std::string name;
    public:
      LoadCharacterEvent(const std::string name) : Event(Events::EventType::LOAD_CHARACTER), name(name) {}
      static std::shared_ptr<LoadCharacterEvent> create(const std::string name) { return std::make_shared<LoadCharacterEvent>(name); }
      const std::string getName() const noexcept { return name; }
  };
}

#endif
