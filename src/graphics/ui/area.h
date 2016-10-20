#ifndef AREA_H
#define AREA_H

#include <memory>
#include "graphics/ui/element.h"
#include "graphics/ui/skin.h"
#include "events/event.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Element

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI Area.
     */
    class Area : public Element, virtual public el::Loggable  {
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      Area constructor
         *
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        Area(VertexData vertex_data, std::shared_ptr<Skin> skin);
        /**
         * @brief      Area factory function.
         *
         * @param[in]  skin           The UI skin to use
         * @param[in]  color          The color
         * @param[in]  screen_width   The screen width
         * @param[in]  screen_height  The screen height
         * @param[in]  x_pos          The x position
         * @param[in]  y_pos          The y position
         * @param[in]  width          The width
         * @param[in]  height         The height
         *
         * @return     a newly constructed Area
         */
        static std::shared_ptr<Area> create(std::shared_ptr<Skin> skin, glm::vec4 color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

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
