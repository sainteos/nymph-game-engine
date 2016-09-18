#ifndef CHARACTER_TYPED_EVENT_H
#define CHARACTER_TYPED_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

namespace Input {
  class CharacterTypedEvent : public Events::Event {
    private:
      unsigned char character;
    public:
      CharacterTypedEvent(const unsigned char character) : Event(Events::EventType::CHARACTER_TYPED), character(character) {}
      static std::shared_ptr<CharacterTypedEvent> create(const unsigned char character) { return std::make_shared<CharacterTypedEvent>(character); }
      const unsigned char getCharacter() const noexcept { return character; }
  };
}

#endif
