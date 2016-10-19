#include <easylogging++.h>
#include "graphics/ui/quit_button.h"
#include "set_entity_active_event.h"

namespace Graphics {
  namespace UI {

    QuitButton::QuitButton(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin) : Button(text, vertex_data, skin) {
      setTextPadding(0.0);
    }

    std::shared_ptr<QuitButton> QuitButton::create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) {
      auto vertex_data = VertexData();
      vertex_data.addVec(VertexData::DATA_TYPE::GEOMETRY, generateRect(screen_width, screen_height, 0.0, 0.0, width, height));
      vertex_data.addVec(VertexData::DATA_TYPE::TEX_COORDS, basisTexCoords());

      text->setColor(text_color);
      text->setSize(width, height);
      text->setVerticalAlignment(WrappableText::VerticalAlignment::VCENTER);
      text->setHorizontalAlignment(WrappableText::HorizontalAlignment::HCENTER);

      auto button = std::make_shared<QuitButton>(text, vertex_data, skin);
      button->setColor(color);
      button->setAnchorPoint(glm::vec2(x_pos, y_pos));
      button->setWidth(width);
      button->setHeight(height);

      text->setText("X");

      return button;
    }

    void QuitButton::onLeftClick() {
      notifyNow(SetEntityActiveEvent::create(false));
    }

    const std::string QuitButton::className() const noexcept {
      return "Graphics::UI::QuitButton";
    }
  }
}