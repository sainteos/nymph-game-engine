#include <easylogging++.h>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include "graphics/animated_tile.h"

namespace Graphics {
  AnimatedTile::AnimatedTile(const unsigned int vertex_array_object, const VertexData& data) : Tile(vertex_array_object, data), frame_time_accumulator(0.0) {
  } 

  AnimatedTile::~AnimatedTile() {

  }
  
  void AnimatedTile::addFrameFront(const glm::ivec2& frame_pos, const unsigned int frame_time) {
    tile_to_time.push_front(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void AnimatedTile::addFrameBack(const glm::ivec2& frame_pos, const unsigned int frame_time) {
    tile_to_time.push_back(std::pair<glm::ivec2, unsigned int>(frame_pos, frame_time));
  }

  void AnimatedTile::popFrameFront() {
    if(tile_to_time.size() == 0)
      throw std::underflow_error("tile_to_time");
    tile_to_time.pop_front();
  }

  void AnimatedTile::popFrameBack() {
    if(tile_to_time.size() == 0)
      throw std::underflow_error("tile_to_time");
    tile_to_time.pop_back();
  }

  void AnimatedTile::onStart() {
    frame_time_accumulator = 0.0;

    if(tile_to_time.size() > 0 && getShader() != nullptr) {
      getShader()->setUniform("tile_coord", tile_to_time.front().first);

      float normalized_width = float(sizeInPixels()) / float(getTexture()->getWidth());
      float normalized_height = float(sizeInPixels()) / float(getTexture()->getHeight());
      multiplier = glm::vec2(normalized_width, normalized_height);

      getShader()->setUniform("tile_coord_multiplier", multiplier);
    }
    else {
      LOG(WARNING)<<"Trying to start an AnimatedTile without shader!";
    }
  }
  
  const bool AnimatedTile::onUpdate(const double delta) {
    frame_time_accumulator += delta;
    if(tile_to_time.size() > 0) {
      if(frame_time_accumulator > tile_to_time.front().second) {
        tile_to_time.push_back(tile_to_time.front());
        tile_to_time.pop_front();
        frame_time_accumulator = 0.0;
      }
      return true;
    }
    else {
      LOG(WARNING)<<"Trying to update animated tile without frames!";
      return false;
    }
  }

  const bool AnimatedTile::onRender() {        
    if(getShader() != nullptr) {
      getShader()->setUniform("tile_coord_multiplier", multiplier);
      getShader()->setUniform("tile_coord", tile_to_time.front().first);
    }
    else {
      LOG(WARNING)<<"Trying to update animated tile without shader!";
      return false;
    }
    Tile::onRender();
    return true;
  }

  void AnimatedTile::onDestroy() {
    Tile::onDestroy();
    tile_to_time.clear();
  }
}
