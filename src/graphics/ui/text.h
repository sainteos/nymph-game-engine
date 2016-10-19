#ifndef TEXT_H
#define TEXT_H
#include <glm/glm.hpp>
#include <memory>
#include "component.h"
#include "graphics/ui/font.h"
#include "graphics/vertex_data.h"
#include "graphics/shader.h"
#include "transform.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Component

namespace Graphics {
  namespace UI {
    class Text : public Component {
      protected:
        std::shared_ptr<Font> font;
        std::string text;
        glm::vec4 color;
        std::shared_ptr<Shader> shader;
        float kerning;

        void renderCharacter(const unsigned char character, Transform transform);

      public:
      //= BEGIN SCRIPTABLE
        Text();
        void setFont(const std::shared_ptr<Font> font);
        virtual void setText(const std::string& text);
        const std::string getText() const noexcept;
        void setColor(const glm::vec4& color);
        const glm::vec4 getColor() const noexcept;
        void setShader(std::shared_ptr<Shader> shader);
        void setKerning(const float amount) noexcept;
        const float getKerning() const noexcept;
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
