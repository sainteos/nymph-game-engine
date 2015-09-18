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
#include "graphics/triggerable_animations.hpp"
#include <list>
#include "component.h"

enum class SpriteState {MOVE_UP, FACE_UP, MOVE_DOWN, FACE_DOWN, MOVE_LEFT, FACE_LEFT, MOVE_RIGHT, FACE_RIGHT};
class Sprite : public Entity, public Events::Observer, public Events::Subject, public Graphics::TriggerableAnimations<SpriteState> {
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

  public:
    Sprite();
    void onNotify(const Events::Event& event) override;
    void onUpdate(const float delta) override;
    void onStart() override;
    void addTriggerableTile(const SpriteState& state, std::shared_ptr<Graphics::Tile> tile);
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
    std::list<std::shared_ptr<Component>> getComponents();
};

#endif
