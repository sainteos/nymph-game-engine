#include "font.h"

namespace Graphics {
  namespace UI {
    Font::Font(const unsigned int size) : size(size) {

    }

    void Font::addCharacter(const char text_char, const Character& character) {
      characters[text_char] = character;
    }

    Character Font::getCharacter(const char text_char) {
      return characters[text_char];
    }

    std::map<char, Character> Font::getCharacters() const {
      return characters;
    }

    const unsigned int Font::getSize() const noexcept {
      return size;
    }
  }
}