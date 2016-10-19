#ifndef EVENT_H
#define EVENT_H
#include "event_type.h"
//= SCRIPTABLE

namespace Events {
  class Event {
    private:
      EventType type;
    public:
      //= BEGIN SCRIPTABLE
      Event(const EventType& type) : type(type) {}
      const EventType getEventType() const { return type; }
      //= END SCRIPTABLE
  };
}

#endif
