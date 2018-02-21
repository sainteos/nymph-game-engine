#ifndef FONT_H
#define FONT_H
#include <map>
#include <glm/glm.hpp>
#include "../vertex_data.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Struct holding font character information.
     */
    struct [[scriptable]] Character {
      /**
       * The character's opengl texture handle.
       */
      unsigned int texture_handle;
      /**
       * The character's position as specified by FT2 and transformed into our system
       */
      glm::vec2 position;
      /**
       * The character's size as specified by FT2 and transformed into our system
       */
      glm::vec2 size;
      /**
       * The character's advance amount.
       */
      float advance;
      /**
       * The character's vertex data.
       */
      VertexData vertex_data;
      /**
       * The character's vao.
       */
      unsigned int vertex_array_object;
    };

    /**
     * @brief      Class for font.
     */
    class [[scriptable]] Font {
      private:
        std::map<char, Character> characters;
        unsigned int size;
        unsigned int pixels_per_unit;
      public:
        /**
         * @brief      Font constructor
         *
         * @param[in]  size             The size
         * @param[in]  pixels_per_unit  The pixels per unit
         */
        Font(const unsigned int size = 12, const unsigned int pixels_per_unit = 32);

        /**
         * @brief      Adds a character.
         *
         * @param[in]  text_char  The text character
         * @param[in]  character  The engine character
         */
        [[scriptable]] void addCharacter(const char text_char, const Character& character);
        /**
         * @brief      Gets the character.
         *
         * @param[in]  text_char  The text character
         *
         * @return     The character.
         */
        [[scriptable]] Character getCharacter(const char text_char);
        /**
         * @brief      Gets the characters.
         *
         * @return     The characters.
         */
        [[scriptable]] std::map<char, Character> getCharacters() const;
        /**
         * @brief      Gets the size.
         *
         * @return     The size.
         */
        [[scriptable]] unsigned int getSize() const noexcept;
        /**
         * @brief      Gets the open gl size.
         *
         * @return     The open gl size.
         */
        [[scriptable]] float getOpenGLSize() const noexcept;
    };
  }
}

#endif
