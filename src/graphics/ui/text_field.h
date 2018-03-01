#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "text_area.h"
namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for text field.
     */
    class [[scriptable]] TextField : public TextArea {
      private:
        bool cursor_over;
        std::shared_ptr<WrappableText> default_text;
        std::shared_ptr<WrappableText> typed_text;
        std::string caret;
        float blink_speed;
        bool in_focus;
        std::string current_typed;
        glm::vec4 typed_color;

        void reset();
      protected:
        const glm::vec4 mouse_over_dim = glm::vec4(0.1, 0.1, 0.1, 0.0);
      public:

        /**
         * @brief      TextField constructor
         *
         * @param[in]  default_text  The default text
         * @param[in]  typed_text    The typed text
         * @param[in]  vertex_data   The vertex data
         * @param[in]  skin          The skin
         */
        [[scriptable]] TextField(std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
        /**
         * @brief      TextField factory function
         *
         * @param[in]  skin              The skin
         * @param[in]  default_text      The default text
         * @param[in]  typed_text        The typed text
         * @param[in]  background_color  The background color
         * @param[in]  padding           The padding
         * @param[in]  screen_width      The screen width
         * @param[in]  screen_height     The screen height
         * @param[in]  x_pos             The x position
         * @param[in]  y_pos             The y position
         * @param[in]  width             The width
         * @param[in]  height            The height
         *
         * @return     { description_of_the_return_value }
         */
        [[scriptable]] static std::shared_ptr<TextField> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, glm::vec4 background_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer);

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
        [[scriptable]] virtual void onCharacterTyped(const unsigned char character);
        /**
         * @brief      Returns a string representation of the object.
         *
         * @return     String representation of the object.
         */
        [[scriptable]] virtual std::string to_string() const noexcept override;
        [[scriptable]] virtual std::string className() const noexcept override;

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual bool onUpdate(const double delta) override;

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;

        virtual void log(el::base::type::ostream_t& os) const override;
    };
  }
}

#endif
