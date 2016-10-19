#ifndef TILE_ANIMATOR_H
#define TILE_ANIMATOR_H
#include <glm/glm.hpp>
#include "component.h"
#include "graphics/vertex_data.h"
#include "events/subject.h"
#include "graphics/set_uniform_event.h"
#include "set_active_event.h"
#include "game/animation_trigger_event.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Component

namespace Graphics {
  class TileAnimator : public Component {
    private:
      glm::vec2 multiplier;
      //in ms
      float frame_time_accumulator;
      unsigned int tile_width;
      unsigned int tile_height;
      unsigned int tileset_width;
      unsigned int tileset_height;

      std::map<std::string, std::list<std::pair<glm::ivec2, unsigned int>>> triggerable_animations;
      std::string current_state;

    public:
      TileAnimator() = delete;
      //= BEGIN SCRIPTABLE
      TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels);

      static std::shared_ptr<TileAnimator> create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width = 32, const unsigned int tile_height = 32);

      void setStartingState(const std::string& state);

      void addFrameFront(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false);

      void addFrameBack(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false);

      void popFrameFront(const std::string& state);

      void popFrameBack(const std::string& state);

      void triggerAnimation(const std::string& state);
      virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE

      virtual void onStart() override;

      virtual const bool onUpdate(const double delta) override;

      virtual void onDestroy() override;

      void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      void onNotifyNow(std::shared_ptr<Events::Event> event) override;

      virtual const unsigned long long getValueForSorting() const noexcept override;


      virtual void log(el::base::type::ostream_t& os) const;
  };
}

#endif
