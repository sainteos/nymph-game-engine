#include <easylogging++.h>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include "graphics/tile_animator.h"
#include "graphics/set_uniform_event.h"
#include "graphics/uniform.h"

namespace Graphics {
  TileAnimator::TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels) : frame_time_accumulator(0.0), tile_width(tile_width_pixels), tile_height(tile_height_pixels), tileset_width(tileset_width), tileset_height(tileset_height) {
  } 

  TileAnimator::~TileAnimator() {

  }
  
  std::shared_ptr<TileAnimator> TileAnimator::create(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width, const unsigned int tile_height) {
    return std::make_shared<TileAnimator>(tileset_width, tileset_height, tile_width, tile_height);
  }

  void TileAnimator::addFrameFront(const glm::ivec2& frame_pos, const unsigned int frame_time) {
    tile_to_time.push_front(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void TileAnimator::addFrameBack(const glm::ivec2& frame_pos, const unsigned int frame_time) {
    tile_to_time.push_back(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void TileAnimator::popFrameFront() {
    if(tile_to_time.size() == 0)
      throw std::underflow_error("tile_to_time");
    tile_to_time.pop_front();
  }

  void TileAnimator::popFrameBack() {
    if(tile_to_time.size() == 0)
      throw std::underflow_error("tile_to_time");
    tile_to_time.pop_back();
  }

  void TileAnimator::onStart() {
    frame_time_accumulator = 0.0;

    if(tile_to_time.size() > 0) {
      Uniform tile_coord;
      tile_coord.setData("tile_coord", tile_to_time.front().first);
      notify(SetUniformEvent(tile_coord));

      float normalized_width = float(tile_width) / float(tileset_width);
      float normalized_height = float(tile_height) / float(tileset_height);
      multiplier = glm::vec2(normalized_width, normalized_height);

      Uniform tile_coord_multiplier;
      tile_coord_multiplier.setData("tile_coord_multiplier", multiplier);
      notify(SetUniformEvent(tile_coord_multiplier));
    }
  }
  
  const bool TileAnimator::onUpdate(const double delta) {
    frame_time_accumulator += delta;
    if(tile_to_time.size() > 0) {
      if(frame_time_accumulator > tile_to_time.front().second) {
        tile_to_time.push_back(tile_to_time.front());
        tile_to_time.pop_front();
        frame_time_accumulator = 0.0;
        Uniform tile_coord;
        tile_coord.setData("tile_coord", tile_to_time.front().first);
        notify(SetUniformEvent(tile_coord));
        Uniform tile_coord_multiplier;
        tile_coord_multiplier.setData("tile_coord_multiplier", multiplier);
        notify(SetUniformEvent(tile_coord_multiplier));
      }

      return true;
    }
  }

  void TileAnimator::onDestroy() {
    tile_to_time.clear();
  }
}
