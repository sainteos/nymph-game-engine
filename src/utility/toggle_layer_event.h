#ifndef TOGGLE_LAYER_EVENT_H
#define TOGGLE_LAYER_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

namespace Utility {
  /**
   * @brief      Class for toggle layer event.
   */
  class ToggleLayerEvent : public Events::Event {
    private:
      unsigned int layer_number;
      bool on;
    public:
      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      ToggleLayerEvent Constructor
       *
       * @param[in]  layer_number  The layer number
       * @param[in]  on            Layer on or off
       */
      ToggleLayerEvent(const unsigned int layer_number, const bool on) : Event(Events::EventType::TOGGLE_LAYER), layer_number(layer_number), on(on) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  layer_number  The layer number
       * @param[in]  on            Layer on or off
       *
       * @return     newly created ToggleLayerEvent
       */
      static std::shared_ptr<ToggleLayerEvent> create(const unsigned int layer_number, const bool on) { return std::make_shared<ToggleLayerEvent>(layer_number, on); }
      /**
       * @brief      Turn the layer on?
       *
       * @return     True if yes
       */
      const bool turnOn() const noexcept { return on; }
      /**
       * @brief      Gets the layer number.
       *
       * @return     The layer number.
       */
      const unsigned int getLayerNumber() const noexcept { return layer_number; }
      //= END SCRIPTABLE
  };
}

#endif