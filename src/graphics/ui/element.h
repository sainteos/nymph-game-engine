#ifndef ELEMENT_H
#define ELEMENT_H

#include <memory>
#include <glm/glm.hpp>
#include "skin.h"
#include "../../events/event.h"
#include "../renderable.h"

namespace Graphics {
  namespace UI {
    class [[scriptable]] Element : public Renderable, virtual public el::Loggable  {
      private:
        std::shared_ptr<Skin> skin;
        glm::vec4 color;
        glm::vec2 anchor_point;
        float width;
        float height;
        float text_padding;
        bool cursor_within;
        glm::dvec2 last_mouse_scroll_position;
        unsigned int layer;
      protected:
        static std::vector<glm::vec3> generateRect(float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) noexcept;
        static std::vector<glm::vec2> basisTexCoords() noexcept;

      public:
        /**
         * @brief      Element constructor
         *
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The UI skin
         */
        Element(VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
        /**
         * @brief      Destroys element.
         */
        virtual ~Element();

        /**
         * @brief      Gets the UI skin.
         *
         * @return     The skin.
         */
        [[scriptable]] std::shared_ptr<Skin> getSkin() const noexcept;
        /**
         * @brief      Sets the UI skin.
         *
         * @param[in]  skin  The skin
         */
        [[scriptable]] void setSkin(std::shared_ptr<Skin> skin) noexcept;
        /**
         * @brief      Gets the anchor point.
         *
         * @return     The anchor point.
         */
        [[scriptable]] glm::vec2 getAnchorPoint() const noexcept;
        /**
         * @brief      Sets the anchor point.
         *
         * @param[in]  anchor_point  The anchor point
         */
        [[scriptable]] void setAnchorPoint(const glm::vec2 anchor_point);
        /**
         * @brief      Gets the width.
         *
         * @return     The width.
         */
        [[scriptable]] float getWidth() const noexcept;
        /**
         * @brief      Gets the height.
         *
         * @return     The height.
         */
        [[scriptable]] float getHeight() const noexcept;
        /**
         * @brief      Sets the width.
         *
         * @param[in]  width  The width
         */
        [[scriptable]] void setWidth(const float width) noexcept;
        /**
         * @brief      Sets the height.
         *
         * @param[in]  height  The height
         */
        [[scriptable]] void setHeight(const float height) noexcept;
        /**
         * @brief      Gets the text padding.
         *
         * @return     The text padding.
         */
        [[scriptable]] float getTextPadding() const noexcept;
        /**
         * @brief      Sets the text padding.
         *
         * @param[in]  text_padding  The text padding
         */
        [[scriptable]] void setTextPadding(const float text_padding) noexcept;
        /**
         * @brief      Gets the color.
         *
         * @return     The color.
         */
        [[scriptable]] glm::vec4 getColor() const noexcept;
        /**
         * @brief      Sets the color.
         *
         * @param[in]  color  The color
         */
        [[scriptable]] void setColor(const glm::vec4 color) noexcept;

        [[scriptable]] virtual std::string to_string() const noexcept override;

        /**
         * @brief      Determines if point is within element.
         *
         * @param[in]  point  The point
         *
         * @return     True if point within, False otherwise.
         */
        [[scriptable]] bool isPointWithin(glm::vec2 point) noexcept;
        [[scriptable]] virtual std::string className() const noexcept override;

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual bool onUpdate(const double delta) override;

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual unsigned long long getValueForSorting() const noexcept override;

        /**
         * @brief      Called when element is left clicked.
         */
        virtual void onLeftClick() = 0;
        /**
         * @brief      Called when element is right clicked.
         */
        virtual void onRightClick() = 0;
        /**
         * @brief      Called when element has left click released over it.
         */
        virtual void onLeftClickRelease() = 0;
        /**
         * @brief      Called when element has right click released over it.
         */
        virtual void onRightClickRelease() = 0;
        /**
         * @brief      Called when the mosue cursor goes over the element.
         */
        virtual void onCursorEnter() = 0;
        /**
         * @brief      Called when the mouse cursor goes off the element.
         */
        virtual void onCursorLeave() = 0;
        /**
         * @brief      Called when a key is pressed while element is in focus.
         *
         * @param[in]  key   The key
         */
        virtual void onKeyDown(const int key) = 0;
        /**
         * @brief      Called when a key is released while element is in focus.
         *
         * @param[in]  key   The key
         */
        virtual void onKeyUp(const int key) = 0;
        /**
         * @brief      Called when a key is repeated while element is in focus.
         *
         * @param[in]  key   The key
         */
        virtual void onKeyRepeat(const int key) = 0;
        /**
         * @brief      Called when the mouse wheel is scrolled while element is in focus.
         *
         * @param[in]  position_change  The position change
         */
        virtual void onScroll(const glm::dvec2 position_change) = 0;
        virtual void log(el::base::type::ostream_t& os) const override;
    };
  }
}
#endif
