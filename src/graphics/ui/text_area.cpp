#include <easylogging++.h>
#include "text_area.h"
#include "change_text_event.h"

namespace Graphics {
  namespace UI {

    TextArea::TextArea(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin, const unsigned int layer) : Area(vertex_data, skin, layer), text(text) {
      getTransform()->addChild(text->getTransform());
    }

    std::shared_ptr<TextArea> TextArea::create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height, const unsigned int layer) {
      auto vertex_data = VertexData();
      vertex_data.addVec(VertexData::DATA_TYPE::GEOMETRY, generateRect(screen_width, screen_height, 0, 0, width, height));
      vertex_data.addVec(VertexData::DATA_TYPE::TEX_COORDS, basisTexCoords());

      text->setColor(text_color);
      text->setSize(width - (2.0 * padding), height - (2.0 * padding));

      auto area = std::make_shared<TextArea>(text, vertex_data, skin, layer);
      area->setColor(color);
      area->setAnchorPoint(glm::vec2(x_pos, y_pos));
      area->setWidth(width);
      area->setHeight(height);
      area->setTextPadding(padding);

      return area;
    }

    void TextArea::setText(std::shared_ptr<WrappableText> text) {
      this->text = text;
    }

    std::shared_ptr<WrappableText> TextArea::getText() const noexcept {
      return this->text;
    }

    void TextArea::onDestroy() {
      Area::onDestroy();
    }

    void TextArea::onStart() {
      getText()->getTransform()->translate(-getAnchorPoint());
      Area::onStart();
    }

    bool TextArea::onUpdate(const double delta) {
      return Area::onUpdate(delta);
    }

    void TextArea::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      switch(event->getEventType()) {
        case Events::EventType::CHANGE_TEXT: {
          auto casted_event = std::static_pointer_cast<ChangeTextEvent>(event);
          getText()->setText(casted_event->getText());
          break;
        }
        default:
          Area::handleQueuedEvent(event);
      }
    }

    void TextArea::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    void TextArea::onLeftClick() {
    }

    void TextArea::onRightClick() {
    }

    void TextArea::onLeftClickRelease() {

    }

    void TextArea::onRightClickRelease() {

    }

    void TextArea::onCursorEnter() {
    }

    void TextArea::onCursorLeave() {
    }

    void TextArea::onKeyDown(const int key) {
    }

    void TextArea::onKeyUp(const int key) {
    }

    void TextArea::onKeyRepeat(const int key) {
    }

    void TextArea::onScroll(const glm::dvec2 position_change) {
    }

    std::string TextArea::className() const noexcept {
      return "Graphics::UI::TextArea";
    }

    std::string TextArea::to_string() const noexcept {
      std::stringstream str;
      str << Area::to_string() << " Text Area text: "<<this->text->to_string();
      return str.str();
    }

    void TextArea::log(el::base::type::ostream_t& os) const {
      os << to_string();
    }
  }
}
