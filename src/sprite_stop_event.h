#ifndef SPRITE_STOP_EVENT_H
#define SPRITE_STOP_EVENT_H
#include "events/event.h"
#include <glm/glm.hpp>

class SpriteStopEvent : public Events::Event {
  public:
    SpriteStopEvent()
      : Event(Events::EventType::SPRITE_STOP) {}
};

#endif
