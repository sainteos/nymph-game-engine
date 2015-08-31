#ifndef KEY_DOWN_EVENT_H
#define KEY_DOWN_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class KeyDownEvent : public Events::Event {
    private:
      int key;
    public:
      KeyDownEvent(const int key) : Event(Events::EventType::KEY_DOWN), key(key) {}
      const int getKey() const noexcept { return key; }
  };
}

#endif
