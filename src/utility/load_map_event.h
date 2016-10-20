#ifndef LOAD_MAP_EVENT_H
#define LOAD_MAP_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  /**
   * @brief      Class for load map event.
   */
  class LoadMapEvent : public Events::Event {
    private:
      std::string name;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Creates a LoadMapEvent
       *
       * @param[in]  name  The name
       */
      LoadMapEvent(const std::string name) : Event(Events::EventType::LOAD_MAP), name(name) {}
      /**
       * @brief      Factory Function
       *
       * @param[in]  name  The name
       *
       * @return     newly created LoadMapEvent
       */
      static std::shared_ptr<LoadMapEvent> create(const std::string name) { return std::make_shared<LoadMapEvent>(name); }
      /**
       * @brief      Gets the name.
       *
       * @return     The name.
       */
      const std::string getName() const noexcept { return name; }
      //= END SCRIPTABLE
  };
}

#endif
