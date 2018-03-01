#ifndef LOAD_MAP_EVENT_H
#define LOAD_MAP_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for load map event.
   */
  class [[scriptable]] LoadMapEvent : public Events::Event {
    private:
      std::string name;
    public:

      /**
       * @brief      Creates a LoadMapEvent
       *
       * @param[in]  name  The name
       */
      [[scriptable]] LoadMapEvent(const std::string name) : Event(Events::EventType::LOAD_MAP), name(name) {}
      /**
       * @brief      Factory Function
       *
       * @param[in]  name  The name
       *
       * @return     newly created LoadMapEvent
       */
      [[scriptable]] static std::shared_ptr<LoadMapEvent> create(const std::string name) { return std::make_shared<LoadMapEvent>(name); }
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      [[scriptable]] std::string getName() const noexcept { return name; }
  };
}

#endif
