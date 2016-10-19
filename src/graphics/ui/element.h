#ifndef ELEMENT_H
#define ELEMENT_H

#include <memory>
#include <glm/glm.hpp>
#include "graphics/ui/skin.h"
#include "events/event.h"
#include "graphics/renderable.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Renderable

namespace Graphics {  
  namespace UI {
    class Element : public Renderable, virtual public el::Loggable  {
      private:
        std::shared_ptr<Skin> skin;
        glm::vec4 color;
        glm::vec2 anchor_point;
        float width;
        float height;
        float text_padding;
        bool cursor_within;
        glm::dvec2 last_mouse_scroll_position;
      protected:
        static std::vector<glm::vec3> generateRect(float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) noexcept;
        static std::vector<glm::vec2> basisTexCoords() noexcept;

      public:
        Element(VertexData vertex_data, std::shared_ptr<Skin> skin);
        virtual ~Element();

      //= BEGIN SCRIPTABLE
        std::shared_ptr<Skin> getSkin() const noexcept;
        void setSkin(std::shared_ptr<Skin> skin) noexcept;

        const glm::vec2 getAnchorPoint() const noexcept;
        void setAnchorPoint(const glm::vec2 anchor_point);

        const float getWidth() const noexcept;
        const float getHeight() const noexcept;
        void setWidth(const float width) noexcept;
        void setHeight(const float height) noexcept;

        const float getTextPadding() const noexcept;
        void setTextPadding(const float text_padding) noexcept;

        const glm::vec4 getColor() const noexcept;
        void setColor(const glm::vec4 color) noexcept;

        const std::string to_string() const noexcept;

        bool isPointWithin(glm::vec2 point) noexcept;
        virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;

        virtual void onLeftClick() = 0;
        virtual void onRightClick() = 0;
        virtual void onLeftClickRelease() = 0;
        virtual void onRightClickRelease() = 0;
        virtual void onCursorEnter() = 0;
        virtual void onCursorLeave() = 0;
        virtual void onKeyDown(const int key) = 0;
        virtual void onKeyUp(const int key) = 0;
        virtual void onKeyRepeat(const int key) = 0;
        virtual void onScroll(const glm::dvec2 position_change) = 0;
        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}
#endif
