#ifndef TRIGGERABLE_H
#define TRIGGERABLE_H

#include <easylogging++.h>
#include <memory>
#include <map>
#include "graphics/renderable.h"
#include "events/subject.h"
#include "component.h"
#include "component_trigger_event.hpp"
#include "set_active_event.h"

namespace Graphics {
  template <typename T>
  class Triggerable : public Component {
    private:
      std::map<T, unsigned int> triggerable_components;
      T current_state;

    public:
      Triggerable() = delete;
      Triggerable(const T& start_state) : current_state(start_state) {

      }

      void addTriggerableComponent(const T& state, const unsigned int component_id) {
        triggerable_components[state] = component_id;
      }

      void triggerComponent(const T& state) {
        notify(SetActiveEvent(triggerable_components[state], false));
        current_state = state;
        notify(SetActiveEvent(triggerable_components[current_state], true));
      }

      virtual void onStart() override {
        for(auto i : triggerable_components) {
          if(i.first == current_state) {
            notify(SetActiveEvent(i.second, true));
          }
          else {
            notify(SetActiveEvent(i.second, false));
          }
        }
      }
      virtual const bool onUpdate(const double delta) override { return true; }
      virtual void onDestroy() override {}

      void onNotify(const Events::Event& event) override {
        switch(event.getEventCode()) {
          case Events::EventType::COMPONENT_TRIGGER: {
            auto casted_event = static_cast<const ComponentTriggerEvent<T>*>(&event);
            triggerComponent(casted_event->getState());
            break;
          }
          default:
            break;
        }
        Component::onNotify(event);
      }
  };
}

#endif
