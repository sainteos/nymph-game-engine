#ifndef KEY_REPEAT_EVENT_H
#define KEY_REPEAT_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class KeyRepeatEvent : public Events::Event {
    private:
      int key;
    public:
      KeyRepeatEvent(const int key) : Event(Events::EventType::KEY_REPEAT), key(key) {}
      static std::shared_ptr<KeyRepeatEvent> create(const int key) { return std::make_shared<KeyRepeatEvent>(key); }
      const int getKey() const noexcept { return key; }
  };
}

#endif
