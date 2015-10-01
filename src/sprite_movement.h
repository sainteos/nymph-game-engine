#ifndef SPRITE_MOVEMENT_H
#define SPRITE_MOVEMENT_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "events/event.h"
#include <list>
#include "component.h"

enum class SpriteState {MOVE_UP, FACE_UP, MOVE_DOWN, FACE_DOWN, MOVE_LEFT, FACE_LEFT, MOVE_RIGHT, FACE_RIGHT};
class SpriteMovement : public Component {
  public:
  private: 
    float moving_speed;
    float move_quantization_in_tiles;
    glm::vec2 current_velocity;
    glm::vec2 next_position;
    bool left_down;
    bool up_down;
    bool down_down;
    bool right_down;
    SpriteState current_state;

  public:
    SpriteMovement();
    void onNotify(const Events::Event& event) override;
    const bool onUpdate(const double delta) override;
    void onStart() override;
    void onDestroy() override {}
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
