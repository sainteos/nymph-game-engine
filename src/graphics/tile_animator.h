#ifndef TILE_ANIMATOR_H
#define TILE_ANIMATOR_H
#include <glm/glm.hpp>
#include "component.h"
#include "graphics/vertex_data.h"
#include "events/subject.h"
#include "graphics/set_uniform_event.h"
#include "set_active_event.h"
#include "animation_trigger_event.h"

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
      TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels)  : current_state(""), frame_time_accumulator(0.0), tile_width(tile_width_pixels), tile_height(tile_height_pixels), tileset_width(tileset_width), tileset_height(tileset_height) {
      } 

      static std::shared_ptr<TileAnimator> create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width = 32, const unsigned int tile_height = 32) {
        return std::make_shared<TileAnimator>(tileset_width, tileset_height, tile_width, tile_height);
      }

      void setStartingState(const std::string& state) {
        current_state = state;
      }

      void addFrameFront(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false) {
        if(set_current)
          current_state = state;
        triggerable_animations[state].push_front(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
      }

      void addFrameBack(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current = false) {
        if(set_current)
          current_state = state;
        triggerable_animations[state].push_back(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
      }

      void popFrameFront(const std::string& state) {
        if(triggerable_animations[state].size() > 0)
          triggerable_animations[state].pop_front();
      }

      void popFrameBack(const std::string& state)  {
        if(triggerable_animations[state].size() > 0)
          triggerable_animations[state].pop_back();
      }

      void triggerAnimation(const std::string& state) {
        current_state = state;
      }

      virtual void onStart() override {
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

      virtual const bool onUpdate(const double delta) override {
        if(!active)
          return false;
 
        while(eventsWaiting()) {
          handleQueuedEvent(getEvent());
        }

        frame_time_accumulator += delta;
        if(triggerable_animations[current_state].size() > 1) {
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

          return true;
        }
      }

      virtual void onDestroy() override {
        triggerable_animations.clear();
      }

      void handleQueuedEvent(std::shared_ptr<Events::Event> event) override {
        switch(event->getEventType()) {
          case Events::EventType::ANIMATION_TRIGGER: {
            auto casted_event = std::static_pointer_cast<AnimationTriggerEvent>(event);
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

      virtual const unsigned long long getValueForSorting() const noexcept override {
        return getId();
      }
  };
}

#endif
