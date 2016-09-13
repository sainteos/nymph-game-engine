#include <easylogging++.h>
#include "graphics/ui/wrappable_text.h"

namespace Graphics {
  namespace UI {
    WrappableText::WrappableText() : horizontal_alignment(HorizontalAlignment::LEFT), vertical_alignment(VerticalAlignment::TOP), horizontal_alignment_transform(Transform()), vertical_alignment_transform(Transform()) {
      
    }

    void WrappableText::setHorizontalAlignment(const HorizontalAlignment& alignment) {
      this->horizontal_alignment = alignment;
    }

    void WrappableText::setVerticalAlignment(const VerticalAlignment& alignment) {
      this->vertical_alignment = alignment;
    }

    void WrappableText::setSize(float width, float height) {
      this->width = width;
      this->height = height;
    }

    std::vector<std::pair<float, std::vector<Character>>> WrappableText::splitTextIntoLines() {
      std::vector<std::pair<float, std::vector<Character>>> lines;
      std::vector<Character> line;
      float current_line_width = 0.0;
      for(auto character : text) {
        if(current_line_width + font->getCharacter(character).size.x > width) {
          lines.push_back(std::pair<float, std::vector<Character>>(current_line_width, line));
          line.clear();
          line.push_back(font->getCharacter(character));
          current_line_width = font->getCharacter(character).size.x;
        }
        else {
          line.push_back(font->getCharacter(character));
          current_line_width += font->getCharacter(character).size.x;
        }
      }
      lines.push_back(std::pair<float, std::vector<Character>>(current_line_width, line));
      return lines;
    }


    void WrappableText::setText(const std::string& text) {
      this->text = text;
      character_transforms.clear();

      if(horizontal_alignment == HorizontalAlignment::LEFT) {
        Transform t;
        float current_width = 0.0;
        for(auto character : this->text) {
          if(current_width + font->getCharacter(character).size.x > width) {
              current_width = 0.0;
              t.translate(glm::vec2(-t.getAbsoluteTranslation().x, -font->getOpenGLSize()));
          }
          character_transforms.insert(std::pair<unsigned char, Transform>(character, t));
          t.translate(glm::vec2(font->getCharacter(character).advance, 0.0));
          current_width += font->getCharacter(character).advance;
        }
      }
      else if(horizontal_alignment == HorizontalAlignment::CENTER) {
        auto lines = splitTextIntoLines();
        
        unsigned int character_index = 0;
        unsigned int line_number = 0;
        for(auto line : lines) {
          Transform character_transform;
          character_transform.translate(glm::vec2(-line.first / 2.0, -font->getOpenGLSize() * line_number));
          for(auto character : line.second) {
            character_transforms.insert(std::pair<unsigned char, Transform>(this->text[character_index], character_transform));
            character_index++;
            character_transform.translate(glm::vec2(character.advance, 0.0));
          }
          line_number++;
        }
      }
      else {
        auto lines = splitTextIntoLines();
        unsigned int line_number = 0;
        std::string line_text= text.substr(0, lines[0].second.size()); 
        unsigned int last_position = 0;

        for(auto line : lines) {
          Transform character_transform;
          character_transform.translate(glm::vec2(width, -font->getOpenGLSize() * line_number));

          if(line_number > 0) {
            line_text = text.substr(last_position, line.second.size());
          }
 
          for(struct {std::vector<Character>::reverse_iterator character; std::string::reverse_iterator text_char;} iter = {line.second.rbegin(), line_text.rbegin()};
                      iter.character != line.second.rend() && iter.text_char != line_text.rend(); iter.character++, iter.text_char++) {
            character_transform.translate(glm::vec2(-iter.character->advance, 0.0));
            character_transforms.insert(std::pair<unsigned char, Transform>(*iter.text_char, character_transform));
          }
          last_position += line_text.size();
          line_number++;
        }
      }        
    }


    void WrappableText::onDestroy() {

    }

    void WrappableText::onStart() {

    }

    const bool WrappableText::onUpdate(const double delta) {

      if(isActive()) {
        if(shader != nullptr) {

          shader->useProgram();
          shader->setUniform<glm::vec4>("color", color);

          for(auto transform : character_transforms) {
            renderCharacter(transform.first, transform.second.getAbsoluteTransformationMatrix());
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

    void WrappableText::handleQueuedEvent(std::shared_ptr<Events::Event> event) {

    }

    void WrappableText::onNotifyNow(std::shared_ptr<Events::Event> event) {

    }

    const unsigned long long WrappableText::getValueForSorting() const noexcept {
      return 0;
    }
  }
}