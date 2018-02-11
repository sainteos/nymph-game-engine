#ifndef BUTTON_H
#define BUTTON_H

#include <glm/glm.hpp>
//#include <nanogui/button.h>
#include "wrappable_text.h"
#include "../vertex_data.h"
#include "text_area.h"
#include "skin.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI button.
     */
    class [[scriptable]] Button : public TextArea {
      private:
        bool cursor_over;
        //nanogui::Button nano_button;
      protected:
        const glm::vec4 mouse_over_dim = glm::vec4(0.1, 0.1, 0.1, 0.0);
      public:

        /**
         * @brief      Button constructor
         *
         * @param[in]  text         The text
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        [[scriptable]] Button(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
        /**
         * @brief      Factory function for Button
         *
         * @param[in]  skin           The UI skin
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
         * @return     a newly constructed Button
         */
        [[scriptable]] static std::shared_ptr<Button> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer);

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
        [[scriptable]] virtual std::string className() const noexcept override;
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;

        virtual void log(el::base::type::ostream_t& os) const override;
    };
  }
}

#endif
