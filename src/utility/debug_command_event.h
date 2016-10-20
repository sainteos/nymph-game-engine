#ifndef DEBUG_COMMAND_EVENT_H
#define DEBUG_COMMAND_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for debug command event.
   */
  class DebugCommandEvent : public Events::Event {
    private:
      std::string debug_command;
    public:
      /**
       * @brief      DebugCommandEvent constructor
       *
       * @param[in]  debug_command  The debug command
       */
      DebugCommandEvent(const std::string debug_command) : Event(Events::EventType::DEBUG_COMMAND), debug_command(debug_command) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  debug_command  The debug command
       *
       * @return     creates a new DebugCommandEvent
       */
      static std::shared_ptr<DebugCommandEvent> create(const std::string debug_command) { return std::make_shared<DebugCommandEvent>(debug_command); }
      /**
       * @brief      Gets the debug command.
       *
       * @return     The debug command.
       */
      const std::string getDebugCommand() const noexcept { return debug_command; }
  };
}

#endif
