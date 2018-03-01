#ifndef EVENT_H
#define EVENT_H
#include "event_type.h"

namespace Events {
  /**
   * @brief      Class for event.
   */
  class [[scriptable]] Event {
    private:
      EventType type;
    public:
      /**
       * @brief      Event constructor
       *
       * @param[in]  type  The event type.
       */
      [[scriptable]] Event(const EventType& type) : type(type) {}
      /**
       * @brief      Gets the event type.
       *
       * @return     The event type.
       */
      [[scriptable]] EventType getEventType() const { return type; }
  };
}

#endif
