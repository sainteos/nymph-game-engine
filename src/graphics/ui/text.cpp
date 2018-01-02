#include <easylogging++.h>
#include <glm/ext.hpp>
#include "text.h"
#include "graphics/uniform.h"

namespace Graphics {
  namespace UI {

    Text::Text() : text("text"), font(nullptr), color(1.0), kerning(0.0) {

    }

    void Text::setFont(const std::shared_ptr<Font> font) {
      this->font = font;
    }

    void Text::setText(const std::string& text) {
      this->text = text;
    }

    std::string Text::getText() const noexcept {
      return text;
    }

    void Text::setColor(const glm::vec4& color) {
      this->color = color;
    }

    glm::vec4 Text::getColor() const noexcept {
      return color;
    }

    void Text::setShader(std::shared_ptr<Shader> shader) {
      this->shader = shader;
    }

    void Text::setKerning(const float amount) noexcept {
      this->kerning = amount;
    }

    float Text::getKerning() const noexcept {
      return kerning;
    }

    std::string Text::to_string() const noexcept {
      std::stringstream str;
      str << Component::to_string() << "Text: "<<text<<" Color: "<<glm::to_string(color)<<" Kerning: "<<kerning;
      return str.str();
    }

    unsigned long long Text::getValueForSorting() const noexcept {
      return (unsigned long long)getTransform()->getAbsoluteTranslation().z;
    }

    void Text::renderCharacter(unsigned char character, Transform transform) {
      Uniform text_texture_uniform;
      text_texture_uniform.setData<int>("text_texture", 0);
      shader->setUniform(text_texture_uniform);

      Uniform transform_uniform;
      transform_uniform.setData<glm::mat4>("transform", getTransform()->getAbsoluteTransformationMatrix() * transform.getAbsoluteTransformationMatrix());
      shader->setUniform(transform_uniform);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, font->getCharacter(character).texture_handle);

      glBindVertexArray(font->getCharacter(character).vertex_array_object);

      if(font->getCharacter(character).vertex_data.getIndexCount() > 0) {
        glDrawElements(GL_TRIANGLES, font->getCharacter(character).vertex_data.getIndexCount(), GL_UNSIGNED_INT, 0);
      }
      else {
        glDrawArrays(GL_TRIANGLES, 0, font->getCharacter(character).vertex_data.getVertexCount());
      }
    }

    void Text::handleQueuedEvent(std::shared_ptr<Events::Event> event) {
      Component::handleQueuedEvent(event);
    }

    void Text::onNotifyNow(std::shared_ptr<Events::Event> event) {
      handleQueuedEvent(event);
    }

    void Text::onDestroy() {

    }

    void Text::onStart() {
    }

    bool Text::onUpdate(const double delta) {
      Uniform color_uniform;
      color_uniform.setData<glm::vec4>("color", color);
      shader->setUniform(color_uniform);

      if(isActive()) {
        if(shader != nullptr) {

          auto character_transform = *getTransform();

          shader->useProgram();

          for(auto character : text) {
            renderCharacter(character, character_transform);
            character_transform.translate(glm::vec2(font->getCharacter(character).advance + kerning, 0.0));
          }
        }
        else {
          LOG(WARNING)<<"Trying to render renderable with nullptr shader";
        }
        return true;
      }
      else {
        return false;
      }
    }

    std::string Text::className() const noexcept {
      return "Graphics::UI::Text";
    }

    void Text::log(el::base::type::ostream_t& os) const {
      os << to_string();
    }
  }
}
