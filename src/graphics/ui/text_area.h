#ifndef TEXT_AREA_H
#define TEXT_AREA_H

#include <memory>
#include "graphics/ui/element.h"
#include "graphics/ui/skin.h"
#include "graphics/ui/area.h"
#include "graphics/ui/wrappable_text.h"
#include "events/event.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Area

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for text area.
     */
    class TextArea : public Area, virtual public el::Loggable  {
      private:
        std::shared_ptr<WrappableText> text;
        const glm::vec4 mouse_over_dim = glm::vec4(0.2, 0.2, 0.2, 0.0);
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      TextArea constructor
         *
         * @param[in]  text         The text
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        TextArea(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin);
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
        static std::shared_ptr<TextArea> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        /**
         * @brief      Sets the text.
         *
         * @param[in]  text  The text
         */
        void setText(std::shared_ptr<WrappableText> text);
        /**
         * @brief      Gets the text.
         *
         * @return     The text.
         */
        std::shared_ptr<WrappableText> getText() const noexcept;
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
        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;
        /**
         * @brief      Returns a string representation of the object.
         *
         * @return     String representation of the object.
         */
        const std::string to_string() const noexcept;
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
