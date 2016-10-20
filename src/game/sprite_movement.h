#ifndef SPRITE_MOVEMENT_H
#define SPRITE_MOVEMENT_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "events/event.h"
#include <list>
#include "component.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES Component

namespace Game {

  //= SCRIPTABLE ENUM
  /**
   * @brief      Sprite state enum
   */
  enum SpriteState : unsigned int {MOVE_UP, FACE_UP, MOVE_DOWN, FACE_DOWN, MOVE_LEFT, FACE_LEFT, MOVE_RIGHT, FACE_RIGHT};

  /**
   * @brief      Class for sprite movement.
   */
  class SpriteMovement : public Component {
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
      std::map<SpriteState, std::string> states;

    public:
      const bool onUpdate(const double delta) override;
      void onStart() override;
      void onDestroy() override {}

      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual const unsigned long long getValueForSorting() const noexcept override;

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Sprite Movement constructor
       */
      SpriteMovement();

      /**
       * @brief      Sets the animation string state.
       *
       * @param[in]  state  The state
       * @param[in]  str    The string
       */
      void setAnimationStringState(const SpriteState& state, const std::string& str);
      /**
       * @brief      Sets the moving speed.
       *
       * @param[in]  speed  The speed
       */
      void setMovingSpeed(const float speed);
      /**
       * @brief      Sets the move quantization.
       *
       * @param[in]  number_of_tiles  The number of tiles
       */
      void setMoveQuantization(const float number_of_tiles);
      /**
       * @brief      Stops the sprite moving left.
       */
      void stopMovingLeft();
      /**
       * @brief      Starts the sprite moving left.
       */
      void moveLeft();
      /**
       * @brief      Stops the sprite moving right.
       */
      void stopMovingRight();
      /**
       * @brief      Starts the sprite moving right.
       */
      void moveRight();
      /**
       * @brief      Stops the sprite moving up.
       */
      void stopMovingUp();
      /**
       * @brief      Starts the sprite moving up.
       */
      void moveUp();
      /**
       * @brief      Stops the sprite moving down.
       */
      void stopMovingDown();
      /**
       * @brief      Starts the sprite moving down
       */
      void moveDown();

      virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
      
      virtual void log(el::base::type::ostream_t& os) const override;
  };
}

#endif
