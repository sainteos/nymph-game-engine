#ifndef KEY_UP_EVENT_H
#define KEY_UP_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Input {
  /**
   * @brief      Class for key up event.
   */
  class [[scriptable]] KeyUpEvent : public Events::Event {
    private:
      int key;
    public:

      /**
       * @brief      KeyUpEvent constructor
       *
       * @param[in]  key   The key
       */
      [[scriptable]] KeyUpEvent(const int key) : Event(Events::EventType::KEY_UP), key(key) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  key   The key
       *
       * @return     Newly constructed KeyUpEvent
       */
      [[scriptable]] static std::shared_ptr<KeyUpEvent> create(const int key) { return std::make_shared<KeyUpEvent>(key); }
      [[scriptable]] int getKey() const noexcept { return key; }
  };
}

#endif
