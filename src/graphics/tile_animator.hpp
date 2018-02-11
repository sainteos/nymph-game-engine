#ifndef TILE_ANIMATOR_H
#define TILE_ANIMATOR_H
#include <glm/glm.hpp>
#include "component.h"
#include "graphics/vertex_data.h"
#include "events/subject.h"
#include "graphics/set_uniform_event.h"
#include "set_active_event.h"
#include "game/animation_trigger_event.hpp"
#include "cloneable.hpp"

namespace Graphics {
  /**
   * @brief      Class for tile animator.
   */
  template<typename StateType>
  class TileAnimator : public Component, public Cloneable<TileAnimator<StateType>> {
    private:
      glm::vec2 multiplier;
      //in ms
      float frame_time_accumulator;
      unsigned int tile_width;
      unsigned int tile_height;
      unsigned int tileset_width;
      unsigned int tileset_height;

      std::map<StateType, std::list<std::pair<glm::ivec2, unsigned int>>> triggerable_animations;
      StateType current_state;

    public:
      TileAnimator() = delete;
      /**
       * @brief      TileAnimator Constructor
       *
       * @param[in]  tileset_width       The tileset width
       * @param[in]  tileset_height      The tileset height
       * @param[in]  tile_width_pixels   The tile width pixels
       * @param[in]  tile_height_pixels  The tile height pixels
       */
      TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels)  : current_state((StateType)0), frame_time_accumulator(0.0), tile_width(tile_width_pixels), tile_height(tile_height_pixels), tileset_width(tileset_width), tileset_height(tileset_height) {
      }
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
      static std::shared_ptr<TileAnimator<StateType>> create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width, const unsigned int tile_height) {
        return std::make_shared<TileAnimator<StateType>>(tileset_width, tileset_height, tile_width, tile_height);
      }
      /**
       * @brief      Sets the starting state.
       *
       * @param[in]  state  The state
       */
      void setStartingState(const StateType& state) {
        current_state = state;
      }
      /**
       * @brief      Adds a frame to the front.
       *
       * @param[in]  state        The state
       * @param[in]  frame_pos    The frame position
       * @param[in]  frame_time   The frame time
       * @param[in]  set_current  The set current
       */
      void addFrameFront(const StateType& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false) {
        if(set_current)
          current_state = state;
        triggerable_animations[state].push_front(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
      }

      /**
       * @brief      Adds a frame to the back.
       *
       * @param[in]  state        The state
       * @param[in]  frame_pos    The frame position
       * @param[in]  frame_time   The frame time
       * @param[in]  set_current  The set current
       */
      void addFrameBack(const StateType& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false){
        if(set_current)
          current_state = state;
        triggerable_animations[state].push_back(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
      }

      /**
       * @brief      Pops a frame from the front
       *
       * @param[in]  state  The state
       */
      void popFrameFront(const StateType& state) {
        if(triggerable_animations[state].size() > 0)
          triggerable_animations[state].pop_front();
      }

      /**
       * @brief      Pops a frame from the back
       *
       * @param[in]  state  The state
       */
      void popFrameBack(const StateType& state) {
        if(triggerable_animations[state].size() > 0)
          triggerable_animations[state].pop_back();
      }

      /**
       * @brief      Triggers animation of state
       *
       * @param[in]  state  The state
       */
      void triggerAnimation(const StateType& state) {
        current_state = state;
      }

      virtual std::string className() const noexcept override  {
        return "Graphics::TileAnimator";
      }

      virtual void onStart() override  {
        frame_time_accumulator = 0.0;

        if(triggerable_animations[current_state].size() > 0) {
          Uniform tile_coord;
          tile_coord.setData("tile_coord", triggerable_animations[current_state].front().first);
          notify(SetUniformEvent::create(tile_coord));

          float normalized_width = float(tile_width) / float(tileset_width);
          float normalized_height = float(tile_height) / float(tileset_height);
          multiplier = glm::vec2(normalized_width, normalized_height);

          Uniform tile_coord_multiplier;
          tile_coord_multiplier.setData("tile_coord_multiplier", multiplier);
          notify(SetUniformEvent::create(tile_coord_multiplier));
        }
      }

      virtual bool onUpdate(const double delta) override  {
        if(!active)
          return false;

        while(eventsWaiting()) {
          handleQueuedEvent(getEvent());
        }

        frame_time_accumulator += delta;
        if(triggerable_animations[current_state].size() > 0) {
          if(frame_time_accumulator > triggerable_animations[current_state].front().second) {
            triggerable_animations[current_state].push_back(triggerable_animations[current_state].front());
            triggerable_animations[current_state].pop_front();
            frame_time_accumulator = 0.0;
            Uniform tile_coord;
            tile_coord.setData(std::string("tile_coord"), triggerable_animations[current_state].front().first);
            notifyNow(SetUniformEvent::create(tile_coord));
            Uniform tile_coord_multiplier;
            tile_coord_multiplier.setData(std::string("tile_coord_multiplier"), multiplier);
            notifyNow(SetUniformEvent::create(tile_coord_multiplier));
          }
        }
        return true;
      }

      virtual void onDestroy() override {
        triggerable_animations.clear();
      }

      void handleQueuedEvent(std::shared_ptr<Events::Event> event) override  {
        switch(event->getEventType()) {
          case Events::EventType::ANIMATION_TRIGGER: {
            auto casted_event = std::static_pointer_cast<Game::AnimationTriggerEvent<StateType>>(event);
            triggerAnimation(casted_event->getState());
            break;
          }
          default:
            Component::handleQueuedEvent(event);
            break;
        }
      }

      void onNotifyNow(std::shared_ptr<Events::Event> event) override {
        handleQueuedEvent(event);
      }

      virtual unsigned long long getValueForSorting() const noexcept override  {
        return getId();
      }

      virtual void log(el::base::type::ostream_t& os) const override {
        os << "Current State: " << current_state << " Tile Width: "<<tile_width<<" Tile Height: "<<tile_height<<" Tileset Width: "<<tileset_width<<" Tileset Height: "<<tileset_height;
        Component::log(os);
      }
  };
}

#endif
