#ifndef KEY_REPEAT_EVENT_H
#define KEY_REPEAT_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event
namespace Input {
  /**
   * @brief      Class for key repeat event.
   */
  class KeyRepeatEvent : public Events::Event {
    private:
      int key;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      Constructor for KeyRepeatEvent
       *
       * @param[in]  key   The key
       */
      KeyRepeatEvent(const int key) : Event(Events::EventType::KEY_REPEAT), key(key) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  key   The key
       *
       * @return     Newly constructed KeyRepeatEvent
       */
      static std::shared_ptr<KeyRepeatEvent> create(const int key) { return std::make_shared<KeyRepeatEvent>(key); }
      /**
       * @brief      Gets the key.
       *
       * @return     The key.
       */
      int getKey() const noexcept { return key; }
      //= END SCRIPTABLE
  };
}

#endif
