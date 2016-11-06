#ifndef QUIT_BUTTON_H
#define QUIT_BUTTON_H

#include "graphics/ui/button.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Button

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for UI quit button.
     */
    class QuitButton : public Button  {
      public:
      //= BEGIN SCRIPTABLE
      
        /**
         * @brief      Quit Button constructor
         *
         * @param[in]  text         The text
         * @param[in]  vertex_data  The vertex data
         * @param[in]  skin         The skin
         */
        QuitButton(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer);
        /**
         * @brief      Quit Button factory function
         *
         * @param[in]  skin           The skin
         * @param[in]  text           The text
         * @param[in]  color          The color
         * @param[in]  text_color     The text color
         * @param[in]  screen_width   The screen width
         * @param[in]  screen_height  The screen height
         * @param[in]  x_pos          The x position
         * @param[in]  y_pos          The y position
         * @param[in]  width          The width
         * @param[in]  height         The height
         *
         * @return     A newly constructed QuitButton
         */
        static std::shared_ptr<QuitButton> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer);

        virtual void onLeftClick() override;
        virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
    };
  }
}

#endif
