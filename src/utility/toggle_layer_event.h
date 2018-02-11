#ifndef TOGGLE_LAYER_EVENT_H
#define TOGGLE_LAYER_EVENT_H
#include <glm/glm.hpp>
#include "../events/event.h"
#include "../events/event_type.h"

namespace Utility {
  /**
   * @brief      Class for toggle layer event.
   */
  class [[scriptable]] ToggleLayerEvent : public Events::Event {
    private:
      unsigned int layer_number;
      bool on;
    public:

      /**
       * @brief      ToggleLayerEvent Constructor
       *
       * @param[in]  layer_number  The layer number
       * @param[in]  on            Layer on or off
       */
      [[scriptable]] ToggleLayerEvent(const unsigned int layer_number, const bool on) : Event(Events::EventType::TOGGLE_LAYER), layer_number(layer_number), on(on) {}
      /**
       * @brief      Factory function
       *
       * @param[in]  layer_number  The layer number
       * @param[in]  on            Layer on or off
       *
       * @return     newly created ToggleLayerEvent
       */
      [[scriptable]] static std::shared_ptr<ToggleLayerEvent> create(const unsigned int layer_number, const bool on) { return std::make_shared<ToggleLayerEvent>(layer_number, on); }
      /**
       * @brief      Turn the layer on?
       *
       * @return     True if yes
       */
      [[scriptable]] bool turnOn() const noexcept { return on; }
      /**
       * @brief      Gets the layer number.
       *
       * @return     The layer number.
       */
      [[scriptable]] unsigned int getLayerNumber() const noexcept { return layer_number; }
  };
}

#endif
