#ifndef FONT_H
#define FONT_H
#include <map>
#include <glm/glm.hpp>

namespace Graphics {
  namespace UI {
    struct Character {
      unsigned int texture_handle;
      glm::ivec2 size;
      glm::ivec2 bearing;
      long int advance;
    };

    class Font {
      private:
        std::map<char, Character> characters;
        unsigned int size;
      public:
        Font(const unsigned int size = 12);
        void addCharacter(const char text_char, const Character& character);
        Character getCharacter(const char text_char);
        std::map<char, Character> getCharacters() const;
        const unsigned int getSize() const noexcept;
    };
  }
}

#endif
