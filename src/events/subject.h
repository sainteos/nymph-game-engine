#ifndef SUBJECT_H
#define SUBJECT_H
#include <list>
#include <memory>
#include "events/observer.h"
#include "events/event.h"
//= SCRIPTABLE

namespace Events {
  /**
   * @brief      Class for a subject that an observer would observe for changes.
   */
  class Subject {
    private: 
      std::list<std::shared_ptr<Observer>> observers;
    public:
      virtual ~Subject() = default;
      //= BEGIN SCRIPTABLE
      /**
       * @brief      notify is used to tell observers of an event.
       *
       * @param[in]  event  The event
       */
      virtual void notify(std::shared_ptr<Event> event);
      /**
       * @brief      notifyNow is used to tell observers of an event as an interrupt.
       *
       * @param[in]  event  The event
       */
      virtual void notifyNow(std::shared_ptr<Event> event);
      /**
       * @brief      Adds an observer.
       *
       * @param[in]  observer  The observer
       */
      virtual void addObserver(std::shared_ptr<Observer> observer);
      /**
       * @brief      Removes an observer.
       *
       * @param[in]  observer  The observer
       */
      virtual void removeObserver(std::shared_ptr<Observer> observer);
      //= END SCRIPTABLE
  };
}
#endif
