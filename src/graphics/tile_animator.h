#ifndef TILE_ANIMATOR_H
#define TILE_ANIMATOR_H
#include <glm/glm.hpp>
#include "component.h"
#include "graphics/vertex_data.h"
#include "events/subject.h"

namespace Graphics {
  class TileAnimator : public Component {
    private:
      std::list<std::pair<glm::ivec2, unsigned int>> tile_to_time;
      glm::vec2 multiplier;
      //in ms
      float frame_time_accumulator;
      unsigned int tile_width;
      unsigned int tile_height;
      unsigned int tileset_width;
      unsigned int tileset_height;

    public:
      TileAnimator(const unsigned int tileset_width, const unsigned int tileset_height, const unsigned int tile_width_pixels, const unsigned int tile_height_pixels);
      static std::shared_ptr<TileAnimator> create(const unsigned int tileset_width, const unsigned int tilesest_height, const unsigned int tile_width = 32, const unsigned int tile_height = 32);

      TileAnimator() = delete;
      virtual ~TileAnimator();

      void addFrameFront(const glm::ivec2& frame_pos, const unsigned int frame_time);
      void addFrameBack(const glm::ivec2& frame_pos, const unsigned int frame_time);
      void popFrameFront();
      void popFrameBack();

      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void onDestroy() override;
  };
}

#endif
