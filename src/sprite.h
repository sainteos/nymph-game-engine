#ifndef SPRITE_H
#define SPRITE_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "entity.h"
#include "events/subject.h"
#include "events/observer.h"
#include "events/event.h"
#include "graphics/tile.h"

class Sprite : public Entity, public Events::Observer {
  private: 
    std::map<std::string, std::shared_ptr<Graphics::Tile>> tiles;
    std::string current_tile;
    float moving_speed;
    glm::vec2 current_velocity;

  public:
    void onNotify(const Event& event);
    void onUpdate(const float delta);
    void addTile(const std::string& name, std::shared_ptr<Graphics::Tile> tile);
    void triggerTile(const std::string& tile_name);
    void setMovingSpeed(const float speed);
    void stopMovingLeft();
    void moveLeft();
    void stopMovingRight();
    void moveRight();
    void stopMovingUp();
    void moveUp();
    void stopMovingDown();
    void moveDown();
};

#endif
