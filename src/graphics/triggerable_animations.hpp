#ifndef TRIGGERABLE_ANIMATIONS_H
#define TRIGGERABLE_ANIMATIONS_H

#include <easylogging++.h>
#include <memory>
#include <map>
#include "graphics/tile.h"

namespace Graphics {
  template <typename T>
  class TriggerableAnimations {
    private:
      std::map<T, std::shared_ptr<Graphics::Tile>> triggerable_tiles;
    protected:
      T current_state;

      void addTile(const T& state, std::shared_ptr<Graphics::Tile> tile) {
        triggerable_tiles[state] = tile;
      }
    public:
      TriggerableAnimations() = delete;
      TriggerableAnimations(const T& start_state) : current_state(start_state) {

      }

      void initializeStates() {
        for(auto i : triggerable_tiles) {
          if(i.first == current_state) {
            i.second->setActive();
          }
          else {
            i.second->setInactive();
          }
        }
      }

      void triggerTile(const T& state) {
        triggerable_tiles[current_state]->setInactive();
        current_state = state;
        triggerable_tiles[current_state]->setActive();
      }
  };
}

#endif
