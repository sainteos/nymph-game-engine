#ifndef OBSERVER_H
#define OBSERVER_H
#include <memory>
#include <queue>
#include "events/event.h"

namespace Events {
  class Observer {
    protected:
      std::queue<std::shared_ptr<Event>> events;
    public:
      virtual ~Observer() {}
      void onNotify(std::shared_ptr<Events::Event> event) { events.push(event); }
  };
}

#endif
