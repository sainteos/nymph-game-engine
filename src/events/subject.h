#ifndef SUBJECT_H
#define SUBJECT_H
#include <list>
#include <memory>
#include "events/observer.h"
#include "events/event.h"

namespace Events {
  class Subject {
    private: 
      std::list<std::shared_ptr<Observer>> observers;
    protected:
      void notify(std::shared_ptr<Event> event);
    public:
      void addObserver(std::shared_ptr<Observer> observer);
      void removeObserver(std::shared_ptr<Observer> observer);
  };
}
#endif
