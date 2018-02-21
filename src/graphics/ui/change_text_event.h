#ifndef CHANGE_TEXT_EVENT_H
#define CHANGE_TEXT_EVENT_H
#include <glm/glm.hpp>
#include "../../events/event.h"
#include "../../events/event_type.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for change text event.
     */
    class [[scriptable]] ChangeTextEvent : public Events::Event {
      private:
        std::string text;
      public:

        /**
         * @brief      ChangeTextEvent constructor
         *
         * @param[in]  text  The text
         */
        [[scriptable]] ChangeTextEvent(const std::string& text) : Event(Events::EventType::CHANGE_TEXT), text(text) {}
        /**
         * @brief      ChangeTextEvent factory function
         *
         * @param[in]  text  The text
         *
         * @return     A newly created ChangeTextEvent
         */
        [[scriptable]] static std::shared_ptr<ChangeTextEvent> create(const std::string& text) { return std::make_shared<ChangeTextEvent>(text); }
        /**
         * @brief      Gets the text.
         *
         * @return     The text.
         */
        [[scriptable]] std::string getText() const noexcept { return text; }
    };
  }
}

#endif
