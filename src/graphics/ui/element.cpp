#include <easylogging++.h>
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include "element.h"
#include "input/key_down_event.h"
#include "input/key_up_event.h"
#include "input/mouse_button_down_event.h"
#include "input/mouse_button_up_event.h"
#include "input/mouse_cursor_event.h"
#include "input/mouse_scroll_event.h"

namespace Graphics {
  namespace UI {

    Element::Element(VertexData vertex_data, std::shared_ptr<Skin> skin) : Renderable(vertex_data.generateVertexArrayObject(), vertex_data), skin(skin), anchor_point(glm::vec2(0.0, 0.0)), width(0), height(0), text_padding(0), cursor_within(false), color(glm::vec4(1.0)) {

    }

    Element::~Element() {

    }

    std::vector<glm::vec3> Element::generateRect(float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) noexcept {
      std::vector<glm::vec3> rect_points;

      rect_points.push_back(glm::vec3(x_pos, y_pos, -0.1));
      rect_points.push_back(glm::vec3(x_pos, y_pos + height, -0.1));
      rect_points.push_back(glm::vec3(x_pos + width, y_pos + height, -0.1));
      rect_points.push_back(glm::vec3(x_pos, y_pos, -0.1));
      rect_points.push_back(glm::vec3(x_pos + width, y_pos + height, -0.1));
      rect_points.push_back(glm::vec3(x_pos + width, y_pos, -0.1));

      return rect_points;
    }

    std::vector<glm::vec2> Element::basisTexCoords() noexcept {
      std::vector<glm::vec2> tex_coords {
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 1.0),
        glm::vec2(0.0, 0.0),
        glm::vec2(1.0, 1.0),
        glm::vec2(1.0, 0.0)
      };

      return tex_coords;
    }

    std::shared_ptr<Skin> Element::getSkin() const noexcept {
      return skin;
    }

    void Element::setSkin(std::shared_ptr<Skin> skin) noexcept {
      this->skin = skin;
    }

    const glm::vec2 Element::getAnchorPoint() const noexcept {
      return anchor_point;
    }

    void Element::setAnchorPoint(const glm::vec2 anchor_point) {
      this->anchor_point = anchor_point;
    }

    const unsigned int Element::getWidth() const noexcept {
      return width;
    }

    const unsigned int Element::getHeight() const noexcept {
      return height;
    }

    void Element::setWidth(const unsigned int width) noexcept {
      this->width = width;
    }

    void Element::setHeight(const unsigned int height) noexcept {
      this->height = height;
    }

    const unsigned int Element::getTextPadding() const noexcept {
      return this->text_padding;
    }

    void Element::setTextPadding(const unsigned int text_padding) noexcept {
      this->text_padding = text_padding;
    }

    const glm::vec4 Element::getColor() const noexcept {
      return color;
    }

    void Element::setColor(const glm::vec4 color) noexcept {
      this->color = color;
      auto uniform = Uniform();
      uniform.setData("color", color);

      auto find_iter = uniforms.find(uniform);

      if(find_iter != uniforms.end()) {
        uniforms.erase(find_iter);
      }

      uniforms.insert(uniform);
    }

    bool Element::isPointWithin(glm::vec2 point) noexcept {
      auto translation = getTransform()->getAbsoluteTranslation();

      LOG(INFO)<<glm::to_string(point);

      glm::vec2 min_bounds(translation.x - width / 2.0, translation.y - height / 2.0);
      glm::vec2 max_bounds(translation.x + width / 2.0, translation.y + height / 2.0);

      if(point.x >= min_bounds.x && point.x <= max_bounds.x && point.y >= min_bounds.y && point.y <= max_bounds.y) {
        return true;
      }
      else {
        return false;
      }
    }

    void Element::onDestroy() {
    }

    void Element::onStart() {
      auto uniform = Uniform();
      uniform.setData("color", color);
      uniforms.insert(uniform);

      auto anchor_point_transform = Transform();
      auto anchor_point_uniform = Uniform();
      
      anchor_point_transform.translate(-anchor_point);
      anchor_point_uniform.setData("anchor_point", anchor_point_transform.getAbsoluteTransformationMatrix());
      uniforms.insert(anchor_point_uniform);
    }

    const bool Element::onUpdate(const double delta) {
      return Renderable::onUpdate(delta);
    }

    void Element::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      switch(event->getEventType()) {
        case Events::EventType::MOUSE_CURSOR: {
          auto casted_event = std::static_pointer_cast<Input::MouseCursorEvent>(event);
          if(cursor_within == false && isPointWithin(casted_event->getPosition())) {
            cursor_within = true;
            onCursorEnter();
          }
          else if(cursor_within == true && !isPointWithin(casted_event->getPosition())) {
            cursor_within = false;
            onCursorLeave();
          }
          break;
        }

        case Events::EventType::MOUSE_SCROLL: {
          auto casted_event = std::static_pointer_cast<Input::MouseScrollEvent>(event);
          auto change = last_mouse_scroll_position - casted_event->getPosition();
          last_mouse_scroll_position = casted_event->getPosition();
          onScroll(change);
          break;
        }

        case Events::EventType::MOUSE_BUTTON_DOWN: {
          auto casted_event = std::static_pointer_cast<Input::MouseButtonDownEvent>(event);
          if(casted_event->getButton() == GLFW_MOUSE_BUTTON_LEFT)
            onLeftClick();
          else if(casted_event->getButton() == GLFW_MOUSE_BUTTON_RIGHT)
            onRightClick();
          break;
        }

        case Events::EventType::MOUSE_BUTTON_UP: {
          auto casted_event = std::static_pointer_cast<Input::MouseButtonUpEvent>(event);
          if(casted_event->getButton() == GLFW_MOUSE_BUTTON_LEFT)
            onLeftClickRelease();
          else if(casted_event->getButton() == GLFW_MOUSE_BUTTON_RIGHT)
            onRightClickRelease();
          break;
        }

        case Events::EventType::KEY_DOWN: {
          auto casted_event = std::static_pointer_cast<Input::KeyDownEvent>(event);
          onKeyDown(casted_event->getKey());
          break;
        }

        case Events::EventType::KEY_UP: {
          auto casted_event = std::static_pointer_cast<Input::KeyUpEvent>(event);
          onKeyUp(casted_event->getKey());
          break;
        }

        default: {
          Component::handleQueuedEvent(event);
          break;
        }

      }
    }

    void Element::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    const unsigned long long Element::getValueForSorting() const noexcept {
      return 0;
    }

    void Element::log(el::base::type::ostream_t& os) const {
      os<<"\n"
        <<" Anchor Point: "<<glm::to_string(anchor_point)<<" Width: "<<width<<" Height: "<<height
        <<"Text Padding: "<<text_padding<<" Cursor Within: "<<cursor_within<<" Color: "<<glm::to_string(color);
      Renderable::log(os);
    }
  }
}