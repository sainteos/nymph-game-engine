#ifndef OBSERVER_H
#define OBSERVER_H
#include <easylogging++.h>
#include <memory>
#include <queue>
#include "events/event.h"
#include "events/event_type.h"

namespace Events {
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

      void onNotify(std::shared_ptr<Event> event) { events.push(event); }
      virtual void onNotifyNow(std::shared_ptr<Event> event) = 0;
      virtual void handleQueuedEvent(std::shared_ptr<Event> event) = 0;
      void processEventQueue() {
        while(eventsWaiting()) {
          handleQueuedEvent(getEvent());
        }
      }
  };
}

#endif
