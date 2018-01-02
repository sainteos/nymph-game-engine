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
    /**
     * @brief      Class for text.
     */
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

        /**
         * @brief      Constructor for Text
         */
        Text();
        /**
         * @brief      Sets the font.
         *
         * @param[in]  font  The font
         */
        void setFont(const std::shared_ptr<Font> font);
        /**
         * @brief      Sets the text string.
         *
         * @param[in]  text  The text string
         */
        virtual void setText(const std::string& text);
        /**
         * @brief      Gets the text string
         *
         * @return     The text string.
         */
        std::string getText() const noexcept;
        /**
         * @brief      Sets the color.
         *
         * @param[in]  color  The color
         */
        void setColor(const glm::vec4& color);
        /**
         * @brief      Gets the color.
         *
         * @return     The color.
         */
        glm::vec4 getColor() const noexcept;
        /**
         * @brief      Sets the shader.
         *
         * @param[in]  shader  The shader
         */
        void setShader(std::shared_ptr<Shader> shader);
        /**
         * @brief      Sets the kerning.
         *
         * @param[in]  amount  The amount
         */
        void setKerning(const float amount) noexcept;
        /**
         * @brief      Gets the kerning.
         *
         * @return     The kerning.
         */
        float getKerning() const noexcept;
        /**
         * @brief      Returns a string representation of the object.
         *
         * @return     String representation of the object.
         */
        std::string to_string() const noexcept;
        virtual std::string className() const noexcept override;
      //= END SCRIPTABLE

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual bool onUpdate(const double delta) override;
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual unsigned long long getValueForSorting() const noexcept override;

        virtual void log(el::base::type::ostream_t& os) const;
    };
  }
}

#endif
