#ifndef SUBJECT_H
#define SUBJECT_H
#include <list>
#include <memory>
#include "events/observer.h"
#include "events/event.h"
//= SCRIPTABLE

namespace Events {
  class Subject {
    private: 
      std::list<std::shared_ptr<Observer>> observers;
    public:
      virtual ~Subject() = default;
      //= BEGIN SCRIPTABLE
      virtual void notify(std::shared_ptr<Event> event);
      virtual void notifyNow(std::shared_ptr<Event> event);
      virtual void addObserver(std::shared_ptr<Observer> observer);
      virtual void removeObserver(std::shared_ptr<Observer> observer);
      //= END SCRIPTABLE
  };
}
#endif
