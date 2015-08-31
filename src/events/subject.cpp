#include "events/subject.h"

namespace Events {
  void Subject::addObserver(std::shared_ptr<Observer> observer) {
    observers.push_back(observer);
  }

  void Subject::removeObserver(std::shared_ptr<Observer> observer) {
    observers.remove(observer);
  }

  void Subject::notify(const Events::Event& event) {
    for(auto i : observers) {
      i->onNotify(event);
    }
  }
}