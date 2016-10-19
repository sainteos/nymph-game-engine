#ifndef SET_UNIFORM_EVENT_H
#define SET_UNIFORM_EVENT_H
#include "graphics/uniform.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Graphics {
  class SetUniformEvent : public Events::Event {
    private:
      Uniform uniform;
    public:
      SetUniformEvent() = delete;
      //= BEGIN SCRIPTABLE
      SetUniformEvent(const Uniform& uniform) : Event(Events::EventType::SET_UNIFORM), uniform(uniform) {}
      static std::shared_ptr<SetUniformEvent> create(const Uniform& uniform) { return std::make_shared<SetUniformEvent>(uniform); }
      const Uniform getUniform() const noexcept { return uniform; }
      //= END SCRIPTABLE
  };
}
#endif
