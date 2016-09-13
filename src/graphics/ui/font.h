#ifndef FONT_H
#define FONT_H
#include <map>
#include <glm/glm.hpp>
#include "graphics/vertex_data.h"

namespace Graphics {
  namespace UI {
    struct Character {
      unsigned int texture_handle;
      glm::vec2 position;
      glm::vec2 size;
      float advance;
      VertexData vertex_data;
      unsigned int vertex_array_object;
    };

    class Font {
      private:
        std::map<char, Character> characters;
        unsigned int size;
        unsigned int pixels_per_unit;
      public:
        Font(const unsigned int size = 12, const unsigned int pixels_per_unit = 32);
        void addCharacter(const char text_char, const Character& character);
        Character getCharacter(const char text_char);
        std::map<char, Character> getCharacters() const;
        const unsigned int getSize() const noexcept;
        const float getOpenGLSize() const noexcept;
    };
  }
}

#endif
