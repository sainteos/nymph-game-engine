#ifndef WRAPPABLE_TEXT_H
#define WRAPPABLE_TEXT_H

#include <map>
#include "graphics/ui/text.h"

namespace Graphics {
  namespace UI {
    class WrappableText : public Text {
      public:
        enum class HorizontalAlignment {LEFT, CENTER, RIGHT};
        enum class VerticalAlignment { TOP, CENTER, BOTTOM};
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
        WrappableText();
        void setSize(float width, float height);
        void setLineSpacing(float spacing);
        void setHorizontalAlignment(const HorizontalAlignment& alignment);
        void setVerticalAlignment(const VerticalAlignment& alignment);

        virtual void setText(const std::string& text) override;

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual const unsigned long long getValueForSorting() const noexcept override;
    };
  }
}
#endif
