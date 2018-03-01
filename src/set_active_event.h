#ifndef SET_ACTIVE_EVENT_H
#define SET_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

/**
 * @brief      Class for set active event.
 */
class [[scriptable]] SetActiveEvent : public Events::Event {
  private:
    bool active;
  public:
    /**
     * @brief      Constructs a SetActiveEvent
     *
     * @param[in]  active  True if active
     */
    [[scriptable]] SetActiveEvent(const bool active) : Event(Events::EventType::SET_ACTIVE), active(active) {}
    /**
     * @brief      Factory function
     *
     * @param[in]  active  True if active
     *
     * @return     Newly constructed SetActiveEvent
     */
    [[scriptable]] static std::shared_ptr<SetActiveEvent> create(const bool active) { return std::make_shared<SetActiveEvent>(active); }
    /**
     * @brief      Gets active.
     *
     * @return     True if active
     */
    [[scriptable]] bool getActive() const noexcept { return active; }
};

#endif
