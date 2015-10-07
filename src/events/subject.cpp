#include "events/subject.h"
#include "events/event_type.h"

namespace Events {
  void Subject::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
  }

  void Subject::removeObserver(std::shared_ptr<Observer> observer) {
    observers.remove(observer);
  }

  void Subject::notify(std::shared_ptr<Event> event) {
    for(auto i : observers) {
      i->onNotify(event);
    }
  }
}