#ifndef EVENT_H
#define EVENT_H
#include "event_type.h"
//= SCRIPTABLE

namespace Events {
  /**
   * @brief      Class for event.
   */
  class Event {
    private:
      EventType type;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      Event constructor
       *
       * @param[in]  type  The event type.
       */
      Event(const EventType& type) : type(type) {}
      /**
       * @brief      Gets the event type.
       *
       * @return     The event type.
       */
      EventType getEventType() const { return type; }
      //= END SCRIPTABLE
  };
}

#endif
