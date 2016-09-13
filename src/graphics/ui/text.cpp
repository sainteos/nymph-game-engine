#include <easylogging++.h>
#include "text.h"

namespace Graphics {
  namespace UI {
  
    Text::Text() : text("text"), font(nullptr), color(1.0) {

    }

    void Text::setFont(const std::shared_ptr<Font> font) {
      this->font = font;
    }

    void Text::setText(const std::string& text) {
      this->text = text;
    }

    void Text::setColor(const glm::vec4& color) {
      this->color = color;
    }

    void Text::setShader(std::shared_ptr<Shader> shader) {
      this->shader = shader;
    }

    const unsigned long long Text::getValueForSorting() const noexcept {
      return 0;
    }

    void Text::renderCharacter(unsigned char character, glm::mat4 transform_matrix) {
      shader->setUniform<glm::mat4>("transform", getTransform()->getAbsoluteTransformationMatrix() * transform_matrix);
      shader->setUniform<int>("text_texture", 0);

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

    const bool Text::onUpdate(const double delta) {

      if(isActive()) {
        if(shader != nullptr) {

          auto character_transform = *getTransform();

          shader->useProgram();
          shader->setUniform<glm::vec4>("color", color);

          for(auto character : text) {
            renderCharacter(character, character_transform.getAbsoluteTransformationMatrix());
            character_transform.translate(glm::vec2(font->getCharacter(character).advance, 0.0));
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
  }
}