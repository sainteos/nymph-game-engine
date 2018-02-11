#ifndef CHARACTER_TYPED_EVENT_H
#define CHARACTER_TYPED_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"
namespace Input {
  /**
   * @brief      Class for character typed event.
   */
  class [[scriptable]] CharacterTypedEvent : public Events::Event {
    private:
      unsigned char character;
    public:
      //= BEGIN SCRIPTABLE

      /**
       * @brief      CharacterTypedEvent constructor
       *
       * @param[in]  character  The character
       */
      [[scriptable]] CharacterTypedEvent(const unsigned char character) : Event(Events::EventType::CHARACTER_TYPED), character(character) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  character  The character
       *
       * @return     Newly constructed CharacterTypedEvent
       */
      [[scriptable]] static std::shared_ptr<CharacterTypedEvent> create(const unsigned char character) { return std::make_shared<CharacterTypedEvent>(character); }
      /**
       * @brief      Gets the character.
       *
       * @return     The character.
       */
      [[scriptable]] unsigned char getCharacter() const noexcept { return character; }
  };
}

#endif
