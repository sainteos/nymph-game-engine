#ifndef SET_UNIFORM_EVENT_H
#define SET_UNIFORM_EVENT_H
#include "graphics/uniform.h"

namespace Graphics {
  class SetUniformEvent : public Events::Event {
    private:
      Uniform uniform;
    public:
      SetUniformEvent(const Uniform& uniform) : Event(Events::EventType::SET_UNIFORM), uniform(uniform) {}
      const Uniform getUniform() const noexcept { return uniform; }
  };
}
#endif
