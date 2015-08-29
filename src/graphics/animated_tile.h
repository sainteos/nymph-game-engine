#ifndef ANIMATED_TILE_H
#define ANIMATED_TILE_H
#include <glm/glm.hpp>
#include "graphics/tile.h"
#include "graphics/vertex_data.h"

namespace Graphics {
  class AnimatedTile : public Tile {
    private:
      std::list<std::pair<glm::ivec2, unsigned int>> tile_to_time;
      glm::vec2 multiplier;
      //in ms
      float frame_time_accumulator;
    public:
      AnimatedTile(const unsigned int vertex_array_object, const VertexData& data);
      virtual ~AnimatedTile();

      void addFrameFront(const glm::ivec2& frame_pos, const unsigned int frame_time);
      void addFrameBack(const glm::ivec2& frame_pos, const unsigned int frame_time);
      void popFrameFront();
      void popFrameBack();

      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual const bool onRender() override;
      virtual void onDestroy() override;
  };
}

#endif
