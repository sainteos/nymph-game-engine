#ifndef OBSERVER_H
#define OBSERVER_H
#include <memory>
#include <list>
#include "events/event.h"

namespace Events {
  class Observer {
    public:
      virtual ~Observer() {}
      virtual void onNotify(const Event& event) = 0;
  };
}

#endif
