#ifndef KEY_UP_EVENT_H
#define KEY_UP_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Input {
  /**
   * @brief      Class for key up event.
   */
  class KeyUpEvent : public Events::Event {
    private:
      int key;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      KeyUpEvent constructor
       *
       * @param[in]  key   The key
       */
      KeyUpEvent(const int key) : Event(Events::EventType::KEY_UP), key(key) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  key   The key
       *
       * @return     Newly constructed KeyUpEvent
       */
      static std::shared_ptr<KeyUpEvent> create(const int key) { return std::make_shared<KeyUpEvent>(key); }
      int getKey() const noexcept { return key; }
      //= END SCRIPTABLE
  };
}

#endif
