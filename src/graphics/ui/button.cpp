#include "button.h"

namespace Graphics {
  namespace UI {

    Button::Button(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin) : TextArea(text, vertex_data, skin), cursor_over(false) {

    }

    std::shared_ptr<Button> Button::create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) {
      auto vertex_data = VertexData();
      vertex_data.addVec(VertexData::DATA_TYPE::GEOMETRY, generateRect(screen_width, screen_height, 0.0, 0.0, width, height));
      vertex_data.addVec(VertexData::DATA_TYPE::TEX_COORDS, basisTexCoords());

      text->setColor(text_color);
      text->setSize(width - (2.0 * padding), height - (2.0 * padding));

      auto button = std::make_shared<Button>(text, vertex_data, skin);
      button->setColor(color);
      button->setAnchorPoint(glm::vec2(x_pos, y_pos));
      button->setWidth(width);
      button->setHeight(height);
      button->setTextPadding(padding);

      return button;
    }

    void Button::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      TextArea::handleQueuedEvent(event);
    }

    void Button::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    const unsigned long long Button::getValueForSorting() const noexcept {
      return -30;
    }

    void Button::onLeftClick() {
      if(cursor_over)
        this->setColor(glm::vec4(0.0, 0.0, 0.0, 1.0));
    }

    void Button::onRightClick() {
      LOG(INFO)<<"right click";
    }

    void Button::onLeftClickRelease() {
      this->setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
    }

    void Button::onRightClickRelease() {

    }

    void Button::onCursorEnter() {
      cursor_over = true;
    }

    void Button::onCursorLeave() {
      cursor_over = false;
    }

    void Button::onKeyDown(const int key) {

    }

    void Button::onKeyUp(const int key) {

    }

    void Button::onScroll(const glm::dvec2 position_change) {

    }
  }
}