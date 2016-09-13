#ifndef AREA_H
#define AREA_H

#include <memory>
#include "graphics/ui/element.h"
#include "graphics/ui/skin.h"
#include "events/event.h"

namespace Graphics {
  namespace UI {
    class Area : public Element, virtual public el::Loggable  {
      public:
        Area(VertexData vertex_data, std::shared_ptr<Skin> skin);
        static std::shared_ptr<Area> create(std::shared_ptr<Skin>, glm::vec4 color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;
        
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;

        virtual void onLeftClick() override;
        virtual void onRightClick() override;
        virtual void onCursorEnter() override;
        virtual void onCursorLeave() override;
        virtual void onKeyDown(const int key) override;
        virtual void onKeyUp(const int key) override;
        virtual void onScroll(const glm::dvec2 position_change) override;

        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}

#endif
