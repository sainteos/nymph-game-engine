#ifndef KEY_UP_EVENT_H
#define KEY_UP_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class KeyUpEvent : public Events::Event {
    private:
      int key;
    public:
      KeyUpEvent(const int key) : Event(Events::EventType::KEY_UP), key(key) {}
      const int getKey() const noexcept { return key; }
  };
}

#endif
