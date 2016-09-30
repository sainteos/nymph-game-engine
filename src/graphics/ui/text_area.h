#ifndef TEXT_AREA_H
#define TEXT_AREA_H

#include <memory>
#include "graphics/ui/element.h"
#include "graphics/ui/skin.h"
#include "graphics/ui/area.h"
#include "graphics/ui/wrappable_text.h"
#include "events/event.h"

namespace Graphics {
  namespace UI {
    class TextArea : public Area, virtual public el::Loggable  {
      private:
        std::shared_ptr<WrappableText> text;
        const glm::vec4 mouse_over_dim = glm::vec4(0.2, 0.2, 0.2, 0.0);
      public:
        TextArea(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin);
        static std::shared_ptr<TextArea> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        void setText(std::shared_ptr<WrappableText> text);
        std::shared_ptr<WrappableText> getText() const noexcept;
        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;
        
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

        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}

#endif
