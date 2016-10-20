#ifndef KEY_DOWN_EVENT_H
#define KEY_DOWN_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  /**
   * @brief      Class for key down event.
   */
  class KeyDownEvent : public Events::Event {
    private:
      int key;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Constructor for KeyDownEvent
       *
       * @param[in]  key   The key
       */
      KeyDownEvent(const int key) : Event(Events::EventType::KEY_DOWN), key(key) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  key   The key
       *
       * @return     Newly constructed KeyDownEvent
       */
      static std::shared_ptr<KeyDownEvent> create(const int key) { return std::make_shared<KeyDownEvent>(key); }
      /**
       * @brief      Gets the key.
       *
       * @return     The key.
       */
      const int getKey() const noexcept { return key; }
      //= END SCRIPTABLE
  };
}

#endif
