#include "easylogging++.h"
#include "debug_parser.h"
#include "events/event_type.h"
#include "debug_command_event.h"
#include "list_maps_event.h"
#include "list_characters_event.h"
#include "list_layers_event.h"
#include "toggle_free_camera_event.h"
#include "toggle_lights_event.h"
#include "toggle_layer_event.h"
#include "load_map_event.h"
#include "load_character_event.h"
#include "debug_command_event.h"
#include "window_exit_event.h"

namespace Utility {
  void DebugParser::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  void DebugParser::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    switch(event->getEventType()) {
      case Events::EventType::DEBUG_COMMAND:
        auto casted_event = std::static_pointer_cast<DebugCommandEvent>(event);
        parseCommand(casted_event->getDebugCommand());
        break;
    }

  }

  void DebugParser::handleEvents() {
    processEventQueue();
  }

  std::queue<std::string> DebugParser::tokenSplit(std::string command) {
    std::queue<std::string> tokens;
    while(command.find_first_of(" ") != std::string::npos) {
      tokens.push(command.substr(0, command.find_first_of(" ")));
      command.erase(0, command.find(" ") + 1);
    }
    tokens.push(command);
    return tokens;
  }


  void DebugParser::parseCommand(const std::string& command) {
    auto tokens = tokenSplit(command);
    
    if(tokens.front() == "load") {
      tokens.pop();
      if(tokens.front() == "map") {
        tokens.pop();
        notify(LoadMapEvent::create(tokens.front()));
      }
      else if(tokens.front() == "character") {
        tokens.pop();
        notify(LoadCharacterEvent::create(tokens.front()));
      }
    }
    else if(tokens.front() == "camera") {
      tokens.pop();
      notify(ToggleFreeCameraEvent::create());
    }
    else if(tokens.front() == "list") {
      tokens.pop();
      if(tokens.front() == "maps") {
        tokens.pop();
        notify(ListMapsEvent::create());
      }
      else if(tokens.front() == "characters") {
        tokens.pop();
        notify(ListCharactersEvent::create());
      }
      else if(tokens.front() == "layers") {
        tokens.pop();
        notify(ListLayersEvent::create());
      }

    }
    else if(tokens.front() == "lights") {
      tokens.pop();
      notify(ToggleLightsEvent::create(tokens.front() == "on"));
    }
    else if(tokens.front() == "layer") {
      tokens.pop();
      
      unsigned int layer_number = std::stoul(tokens.front());
      tokens.pop();
      notify(ToggleLayerEvent::create(layer_number, tokens.front() == "on"));
    }
    else if(tokens.front() == "exit" || tokens.front() == "quit") {
      notify(WindowExitEvent::create());
    }
  }
}