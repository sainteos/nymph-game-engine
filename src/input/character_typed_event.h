#ifndef CHARACTER_TYPED_EVENT_H
#define CHARACTER_TYPED_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Input {
  /**
   * @brief      Class for character typed event.
   */
  class CharacterTypedEvent : public Events::Event {
    private:
      unsigned char character;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      CharacterTypedEvent constructor
       *
       * @param[in]  character  The character
       */
      CharacterTypedEvent(const unsigned char character) : Event(Events::EventType::CHARACTER_TYPED), character(character) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  character  The character
       *
       * @return     Newly constructed CharacterTypedEvent
       */
      static std::shared_ptr<CharacterTypedEvent> create(const unsigned char character) { return std::make_shared<CharacterTypedEvent>(character); }
      /**
       * @brief      Gets the character.
       *
       * @return     The character.
       */
      const unsigned char getCharacter() const noexcept { return character; }
      //= END SCRIPTABLE
  };
}

#endif
