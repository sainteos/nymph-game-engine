#ifndef TEXT_AREA_H
#define TEXT_AREA_H

#include <memory>
#include "element.h"
#include "skin.h"
#include "area.h"
#include "wrappable_text.h"
#include "../../events/event.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for text area.
     */
    class [[scriptable]] TextArea : public Area, virtual public el::Loggable  {
      private:
        std::shared_ptr<WrappableText> text;
        const glm::vec4 mouse_over_dim = glm::vec4(0.2, 0.2, 0.2, 0.0);
      public:

        /**
         * @brief      TextArea constructor
         *
         * @param[in]  text         The text
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        [[scriptable]] TextArea(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
        /**
         * @brief      TextArea factory function
         *
         * @param[in]  skin           The skin
         * @param[in]  text           The text
         * @param[in]  color          The color
         * @param[in]  text_color     The text color
         * @param[in]  padding        The padding
         * @param[in]  screen_width   The screen width
         * @param[in]  screen_height  The screen height
         * @param[in]  x_pos          The x position
         * @param[in]  y_pos          The y position
         * @param[in]  width          The width
         * @param[in]  height         The height
         *
         * @return     A newly created TextArea
         */
        [[scriptable]] static std::shared_ptr<TextArea> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer);

        /**
         * @brief      Sets the text.
         *
         * @param[in]  text  The text
         */
        [[scriptable]] void setText(std::shared_ptr<WrappableText> text);
        /**
         * @brief      Gets the text.
         *
         * @return     The text.
         */
        [[scriptable]] std::shared_ptr<WrappableText> getText() const noexcept;
        [[scriptable]] virtual void onLeftClick() override;
        [[scriptable]] virtual void onRightClick() override;
        [[scriptable]] virtual void onLeftClickRelease() override;
        [[scriptable]] virtual void onRightClickRelease() override;
        [[scriptable]] virtual void onCursorEnter() override;
        [[scriptable]] virtual void onCursorLeave() override;
        [[scriptable]] virtual void onKeyDown(const int key) override;
        [[scriptable]] virtual void onKeyUp(const int key) override;
        [[scriptable]] virtual void onKeyRepeat(const int key) override;
        [[scriptable]] virtual void onScroll(const glm::dvec2 position_change) override;
        [[scriptable]] virtual void onDestroy() override;
        [[scriptable]] virtual void onStart() override;
        [[scriptable]] virtual bool onUpdate(const double delta) override;
        /**
         * @brief      Returns a string representation of the object.
         *
         * @return     String representation of the object.
         */
        [[scriptable]] virtual std::string to_string() const noexcept override;
        [[scriptable]] virtual std::string className() const noexcept override;

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;


        virtual void log(el::base::type::ostream_t& os) const override;
    };
  }
}

#endif
