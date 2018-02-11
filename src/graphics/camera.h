#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <memory>
#include "../component.h"
#include "../transform.h"
#include "shader_manager.h"
#include "renderable.h"
#include "../events/observer.h"
#include "../events/event_type.h"
#undef near
#undef far

namespace Graphics {
  /**
   * @brief      Class for camera.
   */
  class [[scriptable]] Camera : public Component {
    private:
      glm::mat4 projection_matrix;
      //THESE NEED TO BE MOVED TO SOMEWHERE ELSE, PERHAPS AN INHERITED CLASS
      Transform last_transform;
      glm::mat4 last_projection_matrix;

      std::shared_ptr<ShaderManager> shader_manager;
      float viewport_width;
      float viewport_height;
      float near;
      float far;

      glm::vec2 velocity;
      glm::vec2 target_position;

      int screen_padding_in_tiles;
      bool free_camera;
      float free_camera_speed;

      Transform negateTransformForScreen(std::shared_ptr<Transform> trans);
    public:
      Camera() = delete;
      /**
       * @brief      Constructor for Camera
       *
       * @param[in]  shader_manager  The shader manager
       */
      Camera(const std::shared_ptr<ShaderManager> shader_manager);
      /**
       * @brief      Constructor for Camera
       *
       * @param[in]  shader_manager   The shader manager
       * @param[in]  viewport_width   The viewport width
       * @param[in]  viewport_height  The viewport height
       * @param[in]  near             The near plane z value
       * @param[in]  far              The far plane z value
       */
      Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near = 0.1, const float far = 1.0);
      virtual void onStart() override;
      virtual bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      /**
       * @brief      Sets the screen padding in tiles.
       *
       * @param[in]  padding  The padding
       */
      [[scriptable]] void setScreenPaddingInTiles(const int padding) noexcept;
      /**
       * @brief      Gets the screen padding in tiles.
       *
       * @return     The screen padding in tiles.
       */
      [[scriptable]] int getScreenPaddingInTiles() const noexcept;
      /**
       * @brief      Sets the width.
       *
       * @param[in]  width  The width
       */
      [[scriptable]] void setWidth(const float width) noexcept;
      /**
       * @brief      Gets the width.
       *
       * @return     The width.
       */
      [[scriptable]] float getWidth() const noexcept;
      /**
       * @brief      Sets the height.
       *
       * @param[in]  height  The height
       */
      [[scriptable]] void setHeight(const float height) noexcept;
      /**
       * @brief      Gets the height.
       *
       * @return     The height.
       */
      [[scriptable]] float getHeight() const noexcept;
      /**
       * @brief      Sets the near z plane
       *
       * @param[in]  near  The near
       */
      [[scriptable]] void setNear(const float near) noexcept;
      /**
       * @brief      Gets the near z plane.
       *
       * @return     The near.
       */
      [[scriptable]] float getNear() const noexcept;
      /**
       * @brief      Sets the far z plane.
       *
       * @param[in]  far   The far
       */
      [[scriptable]] void setFar(const float far) noexcept;
      /**
       * @brief      Gets the far z plane.
       *
       * @return     The far.
       */
      [[scriptable]] float getFar() const noexcept;
      /**
       * @brief      Sets the free camera speed when in free camera mode
       *
       * @param[in]  speed  The speed
       */
      [[scriptable]] void setFreeCameraSpeed(const float speed) noexcept;
      /**
       * @brief      Gets the free camera speed when in free camera mode
       *
       * @return     The free camera speed.
       */
      [[scriptable]] float getFreeCameraSpeed() const noexcept;

      /**
       * @brief      Gets the projection matrix.
       *
       * @return     The projection matrix.
       */
      [[scriptable]] glm::mat4 getProjectionMatrix() const noexcept;

      /**
       * @brief      Determines if component within.
       *
       * @param[in]  component  The component
       *
       * @return     True if component within, False otherwise.
       */
      [[scriptable]] bool isComponentWithin(const Component& component) const;
      [[scriptable]] virtual std::string className() const noexcept override;

      virtual unsigned long long getValueForSorting() const noexcept override;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      virtual void log(el::base::type::ostream_t& os) const override;
  };
}
#endif
