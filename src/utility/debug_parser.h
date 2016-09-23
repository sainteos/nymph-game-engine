#ifndef DEBUG_PARSER_H
#define DEBUG_PARSER_H

#include <queue>
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"

namespace Utility {
  class DebugParser : public Events::Subject, public Events::Observer {
    private:
      std::queue<std::string> tokenSplit(std::string command);
    public:
      void onNotifyNow(std::shared_ptr<Events::Event> event);
      void handleQueuedEvent(std::shared_ptr<Events::Event> event);

      //Must be called every engine loop to get events
      //out of the event queue and use them.
      void handleEvents();
      void parseCommand(const std::string& command);
  };
}

#endif
