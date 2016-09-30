#include <easylogging++.h>
#define GLFW_INCLUDE_GLCOREARB
#include <glfw3.h>
#include "input/character_typed_event.h"
#include "text_field.h"
#include "events/event_type.h"
#include "graphics/ui/suspend_key_input_event.h"
#include "graphics/ui/resume_key_input_event.h"
#include "utility/debug_command_event.h"

namespace Graphics {
  namespace UI {

    TextField::TextField(std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, VertexData vertex_data, std::shared_ptr<Skin> skin) : TextArea(default_text, vertex_data, skin), typed_text(typed_text), default_text(default_text), caret(">"), blink_speed(0.5), in_focus(false), current_typed("") {
      setShader(skin->shader.lock());
      addTexture(0, "skin0", skin->texture.lock());
      typed_color = typed_text->getColor();
      setText(default_text);
    }

    std::shared_ptr<TextField> TextField::create(std::shared_ptr<Skin> skin, std::shared_ptr<WrappableText> default_text, std::shared_ptr<WrappableText> typed_text, glm::vec4 background_color, float padding, float screen_width, float screen_height, float x_pos, float y_pos, float width, float height) {
      auto vertex_data = VertexData();
      vertex_data.addVec(VertexData::DATA_TYPE::GEOMETRY, generateRect(screen_width, screen_height, 0, 0, width, height));
      vertex_data.addVec(VertexData::DATA_TYPE::TEX_COORDS, basisTexCoords());

      default_text->setSize(width - (2.0 * padding), height - (2.0 * padding));
      typed_text->setSize(width - (2.0 * padding), height - (2.0 * padding));
      typed_text->setText("");

      auto field = std::make_shared<TextField>(default_text, typed_text, vertex_data, skin);
      field->setColor(background_color);
      field->setAnchorPoint(glm::vec2(x_pos, y_pos));
      field->setWidth(width);
      field->setHeight(height);
      field->setTextPadding(padding); 

      return field;
    }

    void TextField::reset() {
      in_focus = false;
      typed_text->setText("");
      typed_text->setActive(false);
      default_text->setActive(false);
      setText(default_text);
      notifyNow(std::make_shared<ResumeKeyInputEvent>());
    }

    void TextField::onDestroy() {
      TextArea::onDestroy();
    }

    void TextField::onStart() {
      getTransform()->addChild(default_text->getTransform());
      getTransform()->addChild(typed_text->getTransform());
      
      reset();

      TextArea::onStart();
    }

    const bool TextField::onUpdate(const double delta) {
      return TextArea::onUpdate(delta);
    }
    
    void TextField::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      if(event->getEventType() == Events::EventType::CHARACTER_TYPED) {
        auto casted_event = std::static_pointer_cast<Input::CharacterTypedEvent>(event);
        onCharacterTyped(casted_event->getCharacter());
      }
      TextArea::handleQueuedEvent(event);
    }

    void TextField::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    const unsigned long long TextField::getValueForSorting() const noexcept {
      return -45;
    }

    void TextField::onLeftClick() {
      if(!in_focus) {
        in_focus = true;
        setText(typed_text);
        default_text->setActive(false);
        typed_text->setActive(true);
        typed_text->setColor(typed_color);
        notifyNow(std::make_shared<SuspendKeyInputEvent>());
      }
    }

    void TextField::onRightClick() {
    }

    void TextField::onLeftClickRelease() {

    }

    void TextField::onRightClickRelease() {

    }

    void TextField::onCursorEnter() {
      setColor(getColor() - mouse_over_dim);
    }

    void TextField::onCursorLeave() {
      setColor(getColor() + mouse_over_dim);
    }

    void TextField::onKeyDown(const int key) {

      if(key == GLFW_KEY_ESCAPE) {
        if(isActive()) {
          reset();
          setActive(false);
        }
        else if(!isActive()) {
          default_text->setActive(true);
          setActive(true);
        }
      }

      if(in_focus) {
        if(key == GLFW_KEY_BACKSPACE) {
          typed_text->setText(typed_text->getText().substr(0, typed_text->getText().size() - 1));
        }
        else if(key == GLFW_KEY_ENTER) {
          notifyNow(Utility::DebugCommandEvent::create(typed_text->getText()));
          reset();
          setActive(false);
        }
      }
    }

    void TextField::onKeyUp(const int key) {
    }

    void TextField::onKeyRepeat(const int key) {
      if(in_focus) {
        if(key == GLFW_KEY_BACKSPACE) {
          typed_text->setText(typed_text->getText().substr(0, typed_text->getText().size() - 1));
        }
      }
    }

    void TextField::onScroll(const glm::dvec2 position_change) {
    }

    void TextField::onCharacterTyped(const unsigned char character) {
      if(in_focus) {
        std::stringstream chars;
        chars<<typed_text->getText()<<character;
        typed_text->setText(chars.str());
      }
    }

    void TextField::log(el::base::type::ostream_t& os) const {
      TextArea::log(os);
    }
  }
}