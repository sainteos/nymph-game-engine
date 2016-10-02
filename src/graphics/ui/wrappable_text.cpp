#include <easylogging++.h>
#include <glm/ext.hpp>
#include "graphics/ui/wrappable_text.h"

namespace Graphics {
  namespace UI {
    WrappableText::WrappableText() : line_spacing(0.175), horizontal_alignment(HorizontalAlignment::LEFT), vertical_alignment(VerticalAlignment::TOP), horizontal_alignment_transform(Transform()), vertical_alignment_transform(Transform()) {
      
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

    void WrappableText::setLineSpacing(float spacing) {
      this->line_spacing = spacing;
    }

    std::vector<std::pair<float, std::vector<Character>>> WrappableText::splitTextIntoLines() {
      std::vector<std::pair<float, std::vector<Character>>> lines;
      std::vector<Character> line;
      float current_line_width = 0.0;
      for(auto character : text) {
        if(character != '\n' && current_line_width + font->getCharacter(character).advance + kerning > width || character == '\n') {
          lines.push_back(std::pair<float, std::vector<Character>>(current_line_width, line));
          line.clear();
          if(character != '\n') {
            line.push_back(font->getCharacter(character));
            current_line_width = font->getCharacter(character).advance + kerning;
          }
          else {
            current_line_width = 0.0;
          }
        }
        else {
          line.push_back(font->getCharacter(character));
          current_line_width += font->getCharacter(character).advance + kerning;
        }
      }
      lines.push_back(std::pair<float, std::vector<Character>>(current_line_width, line));
      return lines;
    }


    void WrappableText::setText(const std::string& text) {
      this->text = text;
      character_transforms.clear();

      unsigned int number_of_lines = 0;

      std::string text_without_newlines = text;
      while(text_without_newlines.find_first_of('\n') != std::string::npos) {
        text_without_newlines.erase(text_without_newlines.find_first_of('\n'), 1);
      }

      if(horizontal_alignment == HorizontalAlignment::LEFT) {
        Transform t;
        float current_width = 0.0;
        number_of_lines = 1;
        t.translate(glm::vec2(-width / 2.0, 0.0));
        for(auto character : this->text) {
          if(current_width + font->getCharacter(character).size.x > width || character == '\n') {
            current_width = 0.0;
            t.translate(glm::vec2(-t.getAbsoluteTranslation().x - width / 2.0, -font->getOpenGLSize() - line_spacing));
            number_of_lines++;
          }
          character_transforms.insert(std::pair<unsigned char, Transform>(character, t));
          t.translate(glm::vec2(font->getCharacter(character).advance + kerning, 0.0));
          current_width += font->getCharacter(character).advance;
        }
      }
      else if(horizontal_alignment == HorizontalAlignment::CENTER) {
        auto lines = splitTextIntoLines();
        
        unsigned int character_index = 0;
        unsigned int line_number = 0;
        for(auto line : lines) {
          Transform character_transform;
          character_transform.translate(glm::vec2(-line.first / 2.0, -(font->getOpenGLSize() + line_spacing) * line_number));
          for(auto character : line.second) {
            character_transforms.insert(std::pair<unsigned char, Transform>(text_without_newlines[character_index], character_transform));
            character_index++;
            character_transform.translate(glm::vec2(character.advance + kerning, 0.0));
          }
          line_number++;
        }
        number_of_lines = line_number;
      }
      else {
        auto lines = splitTextIntoLines();
        unsigned int line_number = 0;
        std::string line_text= text.substr(0, lines[0].second.size()); 
        unsigned int last_position = 0;

        for(auto line : lines) {
          Transform character_transform;

          character_transform.translate(glm::vec2(width / 2.0, -font->getOpenGLSize() * line_number - line_spacing));

          if(line_number > 0) {
            line_text = text.substr(last_position, line.second.size());
          }
 
          for(struct {std::vector<Character>::reverse_iterator character; std::string::reverse_iterator text_char;} iter = {line.second.rbegin(), line_text.rbegin()};
                      iter.character != line.second.rend() && iter.text_char != line_text.rend(); iter.character++, iter.text_char++) {
            character_transform.translate(glm::vec2(-iter.character->advance - kerning, 0.0));
            character_transforms.insert(std::pair<unsigned char, Transform>(*iter.text_char, character_transform));
          }
          last_position += line_text.size();
          line_number++;
        }
        number_of_lines = line_number;
      }

      float text_body_height = number_of_lines * (font->getOpenGLSize() + line_spacing) ;
      float vertical_alignment_y = 0.0;
      vertical_alignment_transform = Transform();

      if(vertical_alignment == VerticalAlignment::TOP) {
        vertical_alignment_y = (height - font->getOpenGLSize()) / 2.0;
      }
      else if(vertical_alignment == VerticalAlignment::CENTER) {
        vertical_alignment_y = -font->getOpenGLSize() / 2.0; 
      }
      else {
        vertical_alignment_y = -height / 2.0;
      }
      
      vertical_alignment_transform.translate(glm::vec2(0.0, vertical_alignment_y));
    }


    void WrappableText::onDestroy() {

    }

    void WrappableText::onStart() {
    }

    const bool WrappableText::onUpdate(const double delta) {
      Uniform color_uniform;
      color_uniform.setData<glm::vec4>("color", color);
      shader->setUniform(color_uniform);

      if(isActive()) {
        if(shader != nullptr) {

          shader->useProgram();
          shader->setUniform<glm::vec4>("color", color);
          for(auto transform : character_transforms) {
            renderCharacter(transform.first, vertical_alignment_transform * transform.second);
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
      return -1;
    }
  }
}