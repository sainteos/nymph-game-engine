#ifndef SET_ENTITY_ACTIVE_EVENT_H
#define SET_ENTITY_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

/**
 * @brief      Class for set entity active event.
 */
class [[scriptable]] SetEntityActiveEvent : public Events::Event {
  private:
    bool active;
  public:

    /**
     * @brief      Constructor for SetEntityActiveEvent
     *
     * @param[in]  active  True if active
     */
    [[scriptable]] SetEntityActiveEvent(const bool active) : Event(Events::EventType::SET_ENTITY_ACTIVE), active(active) {}
    /**
     * @brief      Factory function
     *
     * @param[in]  active  The active
     *
     * @return     Newly created SetEntityActiveEvent
     */
    [[scriptable]] static std::shared_ptr<SetEntityActiveEvent> create(const bool active) { return std::make_shared<SetEntityActiveEvent>(active); }\
    /**
     * @brief      Gets active.
     *
     * @return     True if active
     */
    [[scriptable]] bool getActive() const noexcept { return active; }
};

#endif
