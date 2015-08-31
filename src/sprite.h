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
    float move_quantization_in_tiles;
    glm::vec2 current_velocity;
    glm::vec2 next_position;
    bool moving_left;
    bool moving_up;
    bool moving_down;
    bool moving_right;
    bool left_down;
    bool up_down;
    bool down_down;
    bool right_down;

  public:
    Sprite();
    void onNotify(const Events::Event& event) override;
    void onUpdate(const float delta) override;
    void onStart() override;
    void addTile(const std::string& name, std::shared_ptr<Graphics::Tile> tile);
    void triggerTile(const std::string& tile_name);
    void setMovingSpeed(const float speed);
    void setMoveQuantization(const float number_of_tiles);
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
