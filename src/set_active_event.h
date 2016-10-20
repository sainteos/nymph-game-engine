#ifndef SET_ACTIVE_EVENT_H
#define SET_ACTIVE_EVENT_H
#include <glm/glm.hpp>
#include "events/event.h"
#include "events/event_type.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Event

/**
 * @brief      Class for set active event.
 */
class SetActiveEvent : public Events::Event {
  private:
    bool active;
  public:
    //= BEGIN SCRIPTABLE
    
    /**
     * @brief      Constructs a SetActiveEvent
     *
     * @param[in]  active  True if active
     */
    SetActiveEvent(const bool active) : Event(Events::EventType::SET_ACTIVE), active(active) {}
    /**
     * @brief      Factory function
     *             
     * @param[in]  active  True if active
     *
     * @return     Newly constructed SetActiveEvent
     */
    static std::shared_ptr<SetActiveEvent> create(const bool active) { return std::make_shared<SetActiveEvent>(active); }
    /**
     * @brief      Gets active.
     *
     * @return     True if active
     */
    const bool getActive() const noexcept { return active; }
    //= END SCRIPTABLE
};

#endif
