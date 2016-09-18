#include <easylogging++.h>
#include "text_area.h"

namespace Graphics {
  namespace UI {

    TextArea::TextArea(std::shared_ptr<WrappableText> text, VertexData vertex_data, std::shared_ptr<Skin> skin) : Area(vertex_data, skin), text(text) {
      setShader(skin->shader.lock());
      addTexture(0, "skin0", skin->texture.lock());
    }

    std::shared_ptr<TextArea> TextArea::create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> text, glm::vec4 color, glm::vec4 text_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) {
      auto vertex_data = VertexData();
      vertex_data.addVec(VertexData::DATA_TYPE::GEOMETRY, generateRect(screen_width, screen_height, 0, 0, width, height));
      vertex_data.addVec(VertexData::DATA_TYPE::TEX_COORDS, basisTexCoords());

      text->setColor(text_color);
      text->setSize(width - (2.0 * padding), height - (2.0 * padding));

      auto area = std::make_shared<TextArea>(text, vertex_data, skin);
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

    void TextArea::onDestroy() {
      Area::onDestroy();
    }

    void TextArea::onStart() {
      //text->getTransform()->translate(glm::vec2(getTextPadding(), getHeight() - getTextPadding()));
      Area::onStart();
    }

    const bool TextArea::onUpdate(const double delta) {
      return Area::onUpdate(delta);
    }
    
    void TextArea::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      Area::handleQueuedEvent(event);
    }

    void TextArea::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    const unsigned long long TextArea::getValueForSorting() const noexcept {
      return -45;
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

    void TextArea::log(el::base::type::ostream_t& os) const {
      Element::log(os);
    }
  }
}