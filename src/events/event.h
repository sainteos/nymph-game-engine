#ifndef EVENT_H
#define EVENT_H
#include "event_type.h"

namespace Events {
  class Event {
    private:
      EventType type;
    public:
      Event(const EventType& type) : type(type) {}
      const EventType getEventType() const { return type; }
  };
}

#endif
