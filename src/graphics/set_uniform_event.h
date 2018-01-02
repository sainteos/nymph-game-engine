#ifndef SET_UNIFORM_EVENT_H
#define SET_UNIFORM_EVENT_H
#include "graphics/uniform.h"
#include "events/event.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Graphics {
  /**
   * @brief      Class for set uniform event.
   */
  class SetUniformEvent : public Events::Event {
    private:
      Uniform uniform;
    public:
      SetUniformEvent() = delete;
      //= BEGIN SCRIPTABLE

      /**
       * @brief      Constructor for setUniformEvent
       *
       * @param[in]  uniform  The uniform
       */
      SetUniformEvent(const Uniform& uniform) : Event(Events::EventType::SET_UNIFORM), uniform(uniform) {}
      /**
       * @brief      Factory function for SetUniformEvent
       *
       * @param[in]  uniform  The uniform
       *
       * @return     A newly constructed SetUniformEvent
       */
      static std::shared_ptr<SetUniformEvent> create(const Uniform& uniform) { return std::make_shared<SetUniformEvent>(uniform); }
      /**
       * @brief      Gets the uniform.
       *
       * @return     The uniform.
       */
      Uniform getUniform() const noexcept { return uniform; }
      //= END SCRIPTABLE
  };
}
#endif
