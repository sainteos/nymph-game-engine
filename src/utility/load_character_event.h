#ifndef LOAD_CHARACTER_EVENT_H
#define LOAD_CHARACTER_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for load character event.
   */
  class [[scriptable]] LoadCharacterEvent : public Events::Event {
    private:
      std::string name;
    public:

      /**
       * @brief      Constructs a LoadCharacterEvent
       *
       * @param[in]  name  The name
       */
      [[scriptable]] LoadCharacterEvent(const std::string name) : Event(Events::EventType::LOAD_CHARACTER), name(name) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  name  The name
       *
       * @return     Newly constructed LoadCharacterEvent
       */
      [[scriptable]] static std::shared_ptr<LoadCharacterEvent> create(const std::string name) { return std::make_shared<LoadCharacterEvent>(name); }
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      [[scriptable]] std::string getName() const noexcept { return name; }
  };
}

#endif
