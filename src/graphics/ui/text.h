#ifndef TEXT_H
#define TEXT_H
#include <glm/glm.hpp>
#include <memory>
#include "component.h"
#include "graphics/ui/font.h"
#include "graphics/vertex_data.h"

namespace Graphics {
  namespace UI {
    class Text : public Component {
      public:
        enum class HorizontalAlignment {LEFT, CENTER, RIGHT};
        enum class VerticalAlignment { TOP, CENTER, BOTTOM};
      private:
        std::shared_ptr<Font> font;
        std::string text;
        glm::vec4 color;
        VerticalAlignment vertical_alignment;
        HorizontalAlignment horizontal_alignment;
        Transform horizontal_alignment_transform;
        Transform vertical_alignment_transform;

        struct TextData {
          VertexData vertex_data;
          unsigned int vertex_array_object;
        };

        std::map<char, TextData> character_vertex_data;
      public:

        Text();
        void setFont(const std::shared_ptr<Font> font);
        void setText(const std::string& text);
        void setColor(const glm::vec4& color);
        void setHorizontalAlignment(const HorizontalAlignment& alignment);
        void setVerticalAlignment(const VerticalAlignment& alignment);
        void addCharacterVertexData(const char character, const VertexData& vertex_data, const unsigned int vertex_array_object);

        virtual const unsigned long long getValueForSorting() const noexcept override;
        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual const bool onUpdate(const double delta) override;
    };
  }
}

#endif
