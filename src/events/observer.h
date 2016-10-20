#ifndef OBSERVER_H
#define OBSERVER_H
#include <easylogging++.h>
#include <memory>
#include <queue>
#include "events/event.h"
#include "events/event_type.h"

namespace Events {
  /**
   * @brief      Interface to be notified of an item's changes.
   */
  class Observer {
    private:
      std::queue<std::shared_ptr<Event>> events;
    protected:
      std::shared_ptr<Event> getEvent() {
        if(eventsWaiting()) {
          auto e = events.front();
          events.pop();
          return e;
        }
        else {
          return nullptr;
        }
      }

      const bool eventsWaiting() const noexcept {
        return !events.empty();
      }

    public:
      virtual ~Observer() {}

      /**
       * @brief      When receiving an event that is non-immediate, onNotify is used.
       * The event is added to the event queue that is processed when processEventQueue is called.
       *
       * @param[in]  event  The event
       */
      void onNotify(std::shared_ptr<Event> event) { events.push(event); }
      /**
       * @brief      When receiving an event that is immediate, onNotifyNow is used.
       * It acts as an interrupt to make sure the object is notified before another engine loop.
       * This is pure virtual and is implemented on a case by case basis in derived classes.
       * @param[in]  event  The event
       */
      virtual void onNotifyNow(std::shared_ptr<Event> event) = 0;

      /**
       * @brief      HandleQueuedEvent allows derived classes to define behaviour for when queuedEvents are received.
       *
       * @param[in]  event  The event
       */
      virtual void handleQueuedEvent(std::shared_ptr<Event> event) = 0;

      /**
       * @brief      This should be called once ever loop for every Observer to properly use queued events.
       */
      void processEventQueue() {
        while(eventsWaiting()) {
          handleQueuedEvent(getEvent());
        }
      }
  };
}

#endif
