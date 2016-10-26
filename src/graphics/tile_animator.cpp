#include "tile_animator.h"

namespace Graphics {
  
  TileAnimator::TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels)  : current_state(""), frame_time_accumulator(0.0), tile_width(tile_width_pixels), tile_height(tile_height_pixels), tileset_width(tileset_width), tileset_height(tileset_height) {
  } 

  std::shared_ptr<TileAnimator> TileAnimator::create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width, const unsigned int tile_height) {
    return std::make_shared<TileAnimator>(tileset_width, tileset_height, tile_width, tile_height);
  }

  void TileAnimator::setStartingState(const std::string& state) {
    current_state = state;
  }

  void TileAnimator::addFrameFront(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current) {
    if(set_current)
      current_state = state;
    triggerable_animations[state].push_front(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void TileAnimator::addFrameBack(const std::string& state, const glm::ivec2& frame_pos, const unsigned int frame_time, bool set_current) {
    if(set_current)
      current_state = state;
    triggerable_animations[state].push_back(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void TileAnimator::popFrameFront(const std::string& state) {
    if(triggerable_animations[state].size() > 0)
      triggerable_animations[state].pop_front();
  }

  void TileAnimator::popFrameBack(const std::string& state)  {
    if(triggerable_animations[state].size() > 0)
      triggerable_animations[state].pop_back();
  }

  void TileAnimator::triggerAnimation(const std::string& state) {
    current_state = state;
  }

  void TileAnimator::onStart() {
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

  const bool TileAnimator::onUpdate(const double delta) {
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

      return true;
    }
  }

  void TileAnimator::onDestroy() {
    triggerable_animations.clear();
  }

  void TileAnimator::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
    switch(event->getEventType()) {
      case Events::EventType::ANIMATION_TRIGGER: {
        auto casted_event = std::static_pointer_cast<Game::AnimationTriggerEvent>(event);
        triggerAnimation(casted_event->getState());
        break;
      }
      default:
        Component::handleQueuedEvent(event);
        break;
    }
  }

  void TileAnimator::onNotifyNow(std::shared_ptr<Events::Event> event) {
    handleQueuedEvent(event);
  }

  const unsigned long long TileAnimator::getValueForSorting() const noexcept {
    return getId();
  }

  const std::string TileAnimator::className() const noexcept {
    return "Graphics::TileAnimator";
  }

  void TileAnimator::log(el::base::type::ostream_t& os) const {
    os << "Current State: " << current_state << " Tile Width: "<<tile_width<<" Tile Height: "<<tile_height<<" Tileset Width: "<<tileset_width<<" Tileset Height: "<<tileset_height;
    Component::log(os);
  }
}