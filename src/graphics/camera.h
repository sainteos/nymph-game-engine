#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
#include <memory>
#include "component.h"
#include "transform.h"
#include "graphics/shader_manager.h"
#include "graphics/renderable.h"
#include "events/observer.h"
#include "events/event_type.h"
#undef near
#undef far

namespace Graphics {
  class Camera : public Component {
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
      Camera(const std::shared_ptr<ShaderManager> shader_manager);
      Camera(const std::shared_ptr<ShaderManager> shader_manager, const float viewport_width, const float viewport_height, const float near = 0.1, const float far = 1.0);
      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      void setScreenPaddingInTiles(const int padding) noexcept;
      const int getScreenPaddingInTiles() const noexcept;
      void setWidth(const float width) noexcept;
      const float getWidth() const noexcept;
      void setHeight(const float height) noexcept;
      const float getHeight() const noexcept;
      void setNear(const float near) noexcept;
      const float getNear() const noexcept;
      void setFar(const float far) noexcept;
      const float getFar() const noexcept;
      void setFreeCameraSpeed(const float speed) noexcept;
      const float getFreeCameraSpeed() const noexcept;

      const glm::mat4 getProjectionMatrix() const noexcept;

      const bool isComponentWithin(const Component& component) const;

      const unsigned long long getValueForSorting() const noexcept override;

      virtual void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;

      virtual void log(el::base::type::ostream_t& os) const;
  };
}
#endif
