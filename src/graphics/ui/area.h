#ifndef AREA_H
#define AREA_H

#include <memory>
#include "element.h"
#include "skin.h"
#include "../../events/event.h"
namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI Area.
     */
    class [[scriptable]] Area : public Element, virtual public el::Loggable  {
      public:

        /**
         * @brief      Area constructor
         *
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        [[scriptable]] Area(VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
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
        [[scriptable]] static std::shared_ptr<Area> create(std::shared_ptr<Skin> skin, glm::vec4 color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer);

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
