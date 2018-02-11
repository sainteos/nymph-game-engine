#ifndef WRAPPABLE_TEXT_H
#define WRAPPABLE_TEXT_H

#include <map>
#include "text.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for wrappable text.
     */
    class [[scriptable]] WrappableText : public Text {
      public:
        /**
         * @brief      Horizontal Alignment Enum
         */
        enum [[scriptable]] HorizontalAlignment : unsigned int {LEFT, HCENTER, RIGHT};
        /**
         * @brief      Vertical Alignment Enum
         */
        enum [[scriptable]] VerticalAlignment : unsigned int { TOP, VCENTER, BOTTOM};
      private:
        float width;
        float height;
        VerticalAlignment vertical_alignment;
        HorizontalAlignment horizontal_alignment;
        Transform horizontal_alignment_transform;
        Transform vertical_alignment_transform;
        std::multimap<unsigned char, Transform> character_transforms;
        float line_spacing;
      protected:
        std::vector<std::pair<float, std::vector<Character>>> splitTextIntoLines();
      public:

        /**
         * @brief      Wrappable Text constructor.
         */
        [[scriptable]] WrappableText();
        /**
         * @brief      Sets the size.
         *
         * @param[in]  width   The width
         * @param[in]  height  The height
         */
        [[scriptable]] void setSize(float width, float height);
        /**
         * @brief      Sets the line spacing.
         *
         * @param[in]  spacing  The spacing
         */
        [[scriptable]] void setLineSpacing(float spacing);
        /**
         * @brief      Sets the horizontal alignment.
         *
         * @param[in]  alignment  The alignment
         */
        [[scriptable]] void setHorizontalAlignment(const HorizontalAlignment& alignment);
        /**
         * @brief      Sets the vertical alignment.
         *
         * @param[in]  alignment  The alignment
         */
        [[scriptable]] void setVerticalAlignment(const VerticalAlignment& alignment);

        /**
         * @brief      Sets the text string.
         *
         * @param[in]  text  The text string
         */
        [[scriptable]] virtual void setText(const std::string& text) override;
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
