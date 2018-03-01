#ifndef TEXT_H
#define TEXT_H
#include <glm/glm.hpp>
#include <memory>
#include "../../component.h"
#include "../../transform.h"
#include "font.h"
#include "../vertex_data.h"
#include "../shader.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for text.
     */
    class [[scriptable]] Text : public Component {
      protected:
        std::shared_ptr<Font> font;
        std::string text;
        glm::vec4 color;
        std::shared_ptr<Shader> shader;
        float kerning;

        void renderCharacter(const unsigned char character, Transform transform);

      public:
        /**
         * @brief      Constructor for Text
         */
        [[scriptable]] Text();
        /**
         * @brief      Sets the font.
         *
         * @param[in]  font  The font
         */
        [[scriptable]] void setFont(const std::shared_ptr<Font> font);
        /**
         * @brief      Sets the text string.
         *
         * @param[in]  text  The text string
         */
        [[scriptable]] virtual void setText(const std::string& text);
        /**
         * @brief      Gets the text string
         *
         * @return     The text string.
         */
        [[scriptable]] std::string getText() const noexcept;
        /**
         * @brief      Sets the color.
         *
         * @param[in]  color  The color
         */
        [[scriptable]] void setColor(const glm::vec4& color);
        /**
         * @brief      Gets the color.
         *
         * @return     The color.
         */
        [[scriptable]] glm::vec4 getColor() const noexcept;
        /**
         * @brief      Sets the shader.
         *
         * @param[in]  shader  The shader
         */
        [[scriptable]] void setShader(std::shared_ptr<Shader> shader);
        /**
         * @brief      Sets the kerning.
         *
         * @param[in]  amount  The amount
         */
        [[scriptable]] void setKerning(const float amount) noexcept;
        /**
         * @brief      Gets the kerning.
         *
         * @return     The kerning.
         */
        [[scriptable]] float getKerning() const noexcept;
        /**
         * @brief      Returns a string representation of the object.
         *
         * @return     String representation of the object.
         */
        [[scriptable]] std::string to_string() const noexcept override;
        [[scriptable]] virtual std::string className() const noexcept override;

        virtual void onDestroy() override;
        virtual void onStart() override;
        virtual bool onUpdate(const double delta) override;
        void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
        void onNotifyNow(std::shared_ptr<Events::Event> event) override;
        virtual unsigned long long getValueForSorting() const noexcept override;

        virtual void log(el::base::type::ostream_t& os) const override;
    };
  }
}

#endif
