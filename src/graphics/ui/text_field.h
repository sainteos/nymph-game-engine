#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "graphics/ui/text_area.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES TextArea

namespace Graphics {
  namespace UI {
    class TextField : public TextArea {
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
      //= BEGIN SCRIPTABLE
        TextField(std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, VertexData vertex_data, std::shared_ptr<Skin> skin);
        static std::shared_ptr<TextField> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, glm::vec4 background_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

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
        virtual void onCharacterTyped(const unsigned char character);
        const std::string to_string() const noexcept;
        virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
  
        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;

        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}

#endif
