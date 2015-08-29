#include "sprite.h"

void Sprite::onNotify(const Event& event) {

}

void Sprite::onUpdate(const float delta) {
  for(auto i : tiles) {
    if(i.first == current_tile)
      i.second->setActive();
    else
      i.second->setInactive();

    i.second->getTransform()->translate(current_velocity / 1000.0f * 16.6f);
  }
}

void Sprite::addTile(const std::string& name, std::shared_ptr<Graphics::Tile> tile) {
  addComponent(tile);
  tiles[name] = tile;
}

void Sprite::triggerTile(const std::string& name) {
  current_tile = name;
}

void Sprite::setMovingSpeed(const float speed) {
  moving_speed = speed;
}

void Sprite::stopMovingLeft() {
  current_velocity = glm::vec2(0.0, 0.0);
  triggerTile("stop left");
}

void Sprite::moveLeft() {
  current_velocity = glm::vec2(-moving_speed, 0.0);
  triggerTile("move left");
}

void Sprite::stopMovingRight() {
  current_velocity = glm::vec2(0.0, 0.0);
  triggerTile("stop right");
}

void Sprite::moveRight() {
  current_velocity = glm::vec2(moving_speed, 0.0);
  triggerTile("move right");
}

void Sprite::stopMovingUp() {
  current_velocity = glm::vec2(0.0, 0.0);
  triggerTile("stop up");
}

void Sprite::moveUp() {
  current_velocity = glm::vec2(0.0, moving_speed);
  triggerTile("move up");
}

void Sprite::stopMovingDown() {
  current_velocity = glm::vec2(0.0, 0.0);
  triggerTile("stop down");
}

void Sprite::moveDown() {
  current_velocity = glm::vec2(0.0, -moving_speed);
  triggerTile("move down");
}
