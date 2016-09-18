#ifndef BUTTON_H
#define BUTTON_H

#include "graphics/ui/text_area.h"

namespace Graphics {
  namespace UI {
    class Button : public TextArea {
      private: 
        bool cursor_over;
      public:
        Button(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin);
        static std::shared_ptr<Button> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;

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
    };
  }
}

#endif
