#ifndef BUTTON_H
#define BUTTON_H

#include "graphics/ui/text_area.h"

//= SCRIPTABLE
//= SCRIPTABLE BASES TextArea
namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI button.
     */
    class Button : public TextArea {
      private: 
        bool cursor_over;
      protected:
        const glm::vec4 mouse_over_dim = glm::vec4(0.1, 0.1, 0.1, 0.0);
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      Button constructor
         *
         * @param[in]  text         The text
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        Button(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin);
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
        static std::shared_ptr<Button> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        virtual void onLeftClick() override;
        virtual void onRightClick() override;
        virtual void onLeftClickRelease() override;
        virtual void onRightClickRelease() override;
        virtual void onCursorEnter() override;
        virtual void onCursorLeave() override;
        virtual void onKeyDown(const int key) override;
        virtual void onKeyUp(const int key) override;
        virtual void onKeyRepeat(const int key) override;
        virtual void onScroll(const glm::dvec2 position_change) override;
        virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;

        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}

#endif
