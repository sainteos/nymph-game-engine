#ifndef SPRITE_MOVEMENT_H
#define SPRITE_MOVEMENT_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <list>
#include "../events/event.h"
#include "../physics/collision_data.h"
#include "../component.h"
#include "../events/fsm/fsm.hpp"
#include "sprite_move_event.h"
#include "animation_trigger_event.hpp"


namespace Game {

  struct SpriteMovementMotor {
    /**
     * @brief      Sprite state enum
     */
    enum SpriteState : unsigned int {MOVE_UP, FACE_UP, MOVE_DOWN, FACE_DOWN, MOVE_LEFT, FACE_LEFT, MOVE_RIGHT, FACE_RIGHT};
    /**
     * @brief      Sprite input enum
     */
    enum SpriteInput : unsigned int {LEFT, RIGHT, UP, DOWN, NONE};
    /**
     * @brief      Sprite data to be exposed for FSM mutation.
     */
    struct SpriteData {
      float moving_speed;
      float move_quantization_in_tiles;
      glm::vec2 current_velocity;
      glm::vec2 next_position;
      glm::ivec2 tile_location;
      int current_level;
      std::shared_ptr<Transform> transform;
      std::shared_ptr<Physics::CollisionData> collision_data;
    };
    /**
     * @brief      Create a default SpriteData POD
     *
     * @return     Default SpriteData
     */
    static std::shared_ptr<SpriteData> defaultSpriteData() {
      return std::make_shared<SpriteData>(SpriteData {
        1.0,
        1.0,
        glm::vec2(),
        glm::vec2(),
        glm::ivec2(),
        0,
        nullptr,
        nullptr
      });
    }

    /**
     * @brief      General sprite update for each reactor
     *
     * @param[in]  data           Sprite data
     * @param[in]  current_state  The current sprite state
     * @param[in]  delta          The update delta
     *
     * @return     true if sprite is still moving, false otherwise
     */
    static bool updateSprite(const std::shared_ptr<SpriteData> data, const SpriteState current_state, const double delta) {
      if(current_state == MOVE_LEFT || current_state == MOVE_RIGHT ||
       current_state == MOVE_UP || current_state == MOVE_DOWN) {
        if(glm::distance(glm::vec2(data->transform->getAbsoluteTranslation()), data->next_position) > glm::length(data->current_velocity * 1.0f / 1000.0f * (float)delta)) {
          data->transform->getParent()->translate(data->current_velocity * 1.0f / 1000.0f * (float)delta);
          return true;
        }
        else {
          data->transform->getParent()->translate(data->next_position - glm::vec2(data->transform->getAbsoluteTranslation()));
          return false;
        }
      }
      else {
        return false;
      }
    }


    class MoveUpReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        MoveUpReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(MOVE_UP, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, data->moving_speed);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;
          data->tile_location += glm::ivec2(0, -1);

          auto absolute_next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(MOVE_UP));
          subject->notify(SpriteMoveEvent::create(data->current_velocity, absolute_next_position));
        }

        virtual bool updateState(const double delta, SpriteInput& t) override {
          if(!updateSprite(data, getStateType(), delta)) {
            t = NONE;
            return true;
          }
          return false;
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case NONE: {
              return FACE_UP;
            }
            default: {
              return MOVE_UP;
            }
          };
        }
    };

    class FaceUpReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        FaceUpReactor() = delete;
        FaceUpReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(FACE_UP, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation());

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(FACE_UP));
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case LEFT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x - 1, data->tile_location.y) < data->current_level) {
                return MOVE_LEFT;
              }
              else {
                return FACE_LEFT;
              }
            }
            case RIGHT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x + 1, data->tile_location.y) < data->current_level) {
                return MOVE_RIGHT;
              }
              else {
                return FACE_RIGHT;
              }
            }
            case UP: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y - 1) < data->current_level) {
                return MOVE_UP;
              }
              else {
                return FACE_UP;
              }
            }
            case DOWN: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y + 1) < data->current_level) {
                return MOVE_DOWN;
              }
              else {
                return FACE_DOWN;
              }
            }
            case NONE: {
              return FACE_UP;
            }
          };
        }
    };

    class MoveDownReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        MoveDownReactor() = delete;
        MoveDownReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(MOVE_DOWN, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, -data->moving_speed);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;
          data->tile_location += glm::ivec2(0, 1);

          auto absolute_next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(MOVE_DOWN));
          subject->notify(SpriteMoveEvent::create(data->current_velocity, absolute_next_position));
        }

        virtual bool updateState(const double delta, SpriteInput& t) override {
          if(!updateSprite(data, getStateType(), delta)) {
            t = NONE;
            return true;
          }
          return false;
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case NONE: {
              return FACE_DOWN;
            }
            default: {
              return MOVE_DOWN;
            }
          };
        }
    };

    class FaceDownReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        FaceDownReactor() = delete;
        FaceDownReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(FACE_DOWN, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation());

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(FACE_DOWN));
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case LEFT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x - 1, data->tile_location.y) < data->current_level) {
                return MOVE_LEFT;
              }
              else {
                return FACE_LEFT;
              }
            }
            case RIGHT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x + 1, data->tile_location.y) < data->current_level) {
                return MOVE_RIGHT;
              }
              else {
                return FACE_RIGHT;
              }
            }
            case UP: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y - 1) < data->current_level) {
                return MOVE_UP;
              }
              else {
                return FACE_UP;
              }
            }
            case DOWN: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y + 1) < data->current_level) {
                return MOVE_DOWN;
              }
              else {
                return FACE_DOWN;
              }
            }
            case NONE: {
              return FACE_DOWN;
            }
          };
        }
    };

    class MoveLeftReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        MoveLeftReactor() = delete;
        MoveLeftReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(MOVE_LEFT, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(-data->moving_speed, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;
          data->tile_location += glm::ivec2(-1, 0);

          auto absolute_next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(MOVE_LEFT));
          subject->notify(SpriteMoveEvent::create(data->current_velocity, absolute_next_position));
        }

        virtual bool updateState(const double delta, SpriteInput& t) override {
          if(!updateSprite(data, getStateType(), delta)) {
            t = NONE;
            return true;
          }
          return false;
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case NONE: {
              return FACE_LEFT;
            }
            default: {
              return MOVE_LEFT;
            }
          };
        }
    };

    class FaceLeftReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        FaceLeftReactor() = delete;
        FaceLeftReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(FACE_LEFT, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation());

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(FACE_LEFT));
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case LEFT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x - 1, data->tile_location.y) < data->current_level) {
                return MOVE_LEFT;
              }
              else {
                return FACE_LEFT;
              }
            }
            case RIGHT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x + 1, data->tile_location.y) < data->current_level) {
                return MOVE_RIGHT;
              }
              else {
                return FACE_RIGHT;
              }
            }
            case UP: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y - 1) < data->current_level) {
                return MOVE_UP;
              }
              else {
                return FACE_UP;
              }
            }
            case DOWN: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y + 1) < data->current_level) {
                return MOVE_DOWN;
              }
              else {
                return FACE_DOWN;
              }
            }
            case NONE: {
              return FACE_LEFT;
            }
          };
        }
    };

    class MoveRightReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        MoveRightReactor() = delete;
        MoveRightReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(MOVE_RIGHT, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(data->moving_speed, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;
          data->tile_location += glm::ivec2(1, 0);

          auto absolute_next_position = glm::vec2(data->transform->getAbsoluteTranslation()) + glm::normalize(data->current_velocity) * data->move_quantization_in_tiles;

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(MOVE_RIGHT));
          subject->notify(SpriteMoveEvent::create(data->current_velocity, absolute_next_position));
        }

        virtual bool updateState(const double delta, SpriteInput& t) override {
          if(!updateSprite(data, getStateType(), delta)) {
            t = NONE;
            return true;
          }
          return false;
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case NONE: {
              return FACE_RIGHT;
            }
            default: {
              return MOVE_RIGHT;
            }
          };
        }
    };

    class FaceRightReactor : public Events::FSM::StateReactor<SpriteData, SpriteState, SpriteInput> {
      public:
        FaceRightReactor() = delete;
        FaceRightReactor(const std::shared_ptr<SpriteData>& data, Events::Subject* subject) : StateReactor(FACE_RIGHT, data, subject) {}
        virtual void enterState() override {
          data->current_velocity = glm::vec2(0.0, 0.0);
          data->next_position = glm::vec2(data->transform->getAbsoluteTranslation());

          subject->notifyNow(AnimationTriggerEvent<SpriteState>::create(FACE_RIGHT));
        }

        virtual SpriteState react(const SpriteInput transition) override {
          switch(transition) {
            case LEFT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x - 1, data->tile_location.y) < data->current_level) {
                return MOVE_LEFT;
              }
              else {
                return FACE_LEFT;
              }
            }
            case RIGHT: {
              if(data->collision_data->getCollideLevel(data->tile_location.x + 1, data->tile_location.y) < data->current_level) {
                return MOVE_RIGHT;
              }
              else {
                return FACE_RIGHT;
              }
            }
            case UP: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y - 1) < data->current_level) {
                return MOVE_UP;
              }
              else {
                return FACE_UP;
              }
            }
            case DOWN: {
              if(data->collision_data->getCollideLevel(data->tile_location.x, data->tile_location.y + 1) < data->current_level) {
                return MOVE_DOWN;
              }
              else {
                return FACE_DOWN;
              }
            }
            case NONE: {
              return FACE_RIGHT;
            }
          };
        }
    };

    using SpriteFSM = Events::FSM::FSM<SpriteData, SpriteState, SpriteInput,
                        MoveUpReactor, FaceUpReactor, MoveDownReactor, FaceDownReactor,
                        MoveLeftReactor, FaceLeftReactor, MoveRightReactor, FaceRightReactor>;
  };

  /**
   * @brief      Class for sprite movement.
   */
  class [[scriptable]] SpriteMovement : public Component, public std::enable_shared_from_this<SpriteMovement> {
    private:
      std::unique_ptr<SpriteMovementMotor::SpriteFSM> state_machine;
      std::shared_ptr<SpriteMovementMotor::SpriteData> data;

    public:
      virtual bool onUpdate(const double delta) override;
      virtual void onStart() override;
      virtual void onDestroy() override {}

      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual unsigned long long getValueForSorting() const noexcept override;

      /**
       * @brief      Sprite Movement constructor
       */
      [[scriptable]] SpriteMovement();

      /**
       * @brief      Adds a collision data.
       *
       * @param[in]  collision_data  The collision data
       */
      [[scriptable]] void addCollisionData(std::shared_ptr<Physics::CollisionData> collision_data);
      /**
       * @brief      Sets the moving speed.
       *
       * @param[in]  speed  The speed
       */
      [[scriptable]] void setMovingSpeed(const float speed);
      /**
       * @brief      Sets the move quantization.
       *
       * @param[in]  number_of_tiles  The number of tiles
       */
      [[scriptable]] void setMoveQuantization(const float number_of_tiles);


      [[scriptable]] virtual std::string className() const noexcept override;
      [[scriptable]] virtual std::string to_string() const noexcept override;

      virtual void log(el::base::type::ostream_t& os) const override;
  };
}


#endif
