#ifndef DEBUG_COMMAND_EVENT_H
#define DEBUG_COMMAND_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Utility {
  class DebugCommandEvent : public Events::Event {
    private:
      std::string debug_command;
    public:
      DebugCommandEvent(const std::string debug_command) : Event(Events::EventType::DEBUG_COMMAND), debug_command(debug_command) {}
      static std::shared_ptr<DebugCommandEvent> create(const std::string debug_command) { return std::make_shared<DebugCommandEvent>(debug_command); }
      const std::string getDebugCommand() const noexcept { return debug_command; }
  };
}

#endif
