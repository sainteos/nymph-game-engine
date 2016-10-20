#ifndef DEBUG_PARSER_H
#define DEBUG_PARSER_H

#include <queue>
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Subject Observer

namespace Utility {
  /**
   * @brief      Class for debug parser.
   */
  class DebugParser : public Events::Subject, public Events::Observer {
    private:
      std::queue<std::string> tokenSplit(std::string command);
    public:
      void onNotifyNow(std::shared_ptr<Events::Event> event);
      void handleQueuedEvent(std::shared_ptr<Events::Event> event);


      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      DebugParser constructor
       */
      DebugParser() = default;
      /**
       * @brief      Parses a debug command
       *
       * @param[in]  command  The command
       */
      void parseCommand(const std::string& command);
      //Must be called every engine loop to get events
      //out of the event queue and use them.
      
      /**
       * @brief      Handles queued debug parser events
       * 
       * @detail     Must be called every engine loop to get events
       * out of the event queue and use them
       */
      void handleEvents();
      //= END SCRIPTABLE
      
      /**
       * @brief      Destroys the object.
       */
      virtual ~DebugParser() = default;
  };
}

#endif
