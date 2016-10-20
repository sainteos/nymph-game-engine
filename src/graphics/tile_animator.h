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
  /**
   * @brief      Class for tile animator.
   */
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
      
      /**
       * @brief      TileAnimator Constructor
       *
       * @param[in]  tileset_width       The tileset width
       * @param[in]  tileset_height      The tileset height
       * @param[in]  tile_width_pixels   The tile width pixels
       * @param[in]  tile_height_pixels  The tile height pixels
       */
      TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels);
      
      /**
       * @brief      Factory function for TileAnimator
       *
       * @param[in]  tileset_width   The tileset width
       * @param[in]  tileset_height  The tileset height
       * @param[in]  tile_width      The tile width
       * @param[in]  tile_height     The tile height
       *
       * @return     Newly created TileAnimator
       */
      static std::shared_ptr<TileAnimator> create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width = 32, const unsigned int tile_height = 32);

      /**
       * @brief      Sets the starting state.
       *
       * @param[in]  state  The state
       */
      void setStartingState(const std::string& state);
      
      /**
       * @brief      Adds a frame to the front.
       *
       * @param[in]  state        The state
       * @param[in]  frame_pos    The frame position
       * @param[in]  frame_time   The frame time
       * @param[in]  set_current  The set current
       */
      void addFrameFront(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false);

      /**
       * @brief      Adds a frame to the back.
       *
       * @param[in]  state        The state
       * @param[in]  frame_pos    The frame position
       * @param[in]  frame_time   The frame time
       * @param[in]  set_current  The set current
       */
      void addFrameBack(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false);
      
      /**
       * @brief      Pops a frame from the front
       *
       * @param[in]  state  The state
       */
      void popFrameFront(const std::string& state);

      /**
       * @brief      Pops a frame from the back
       *
       * @param[in]  state  The state
       */
      void popFrameBack(const std::string& state);

      /**
       * @brief      Triggers animation of state
       *
       * @param[in]  state  The state
       */
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
