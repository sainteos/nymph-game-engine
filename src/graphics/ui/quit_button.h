#ifndef QUIT_BUTTON_H
#define QUIT_BUTTON_H

#include "graphics/ui/button.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Button

namespace Graphics {
  namespace UI {
    class QuitButton : public Button  {
      public:
      //= BEGIN SCRIPTABLE
        QuitButton(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin);
        static std::shared_ptr<QuitButton> create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height);

        virtual void onLeftClick() override;
        virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
    };
  }
}

#endif
