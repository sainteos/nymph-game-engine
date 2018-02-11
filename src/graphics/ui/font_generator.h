#ifndef FONT_GENERATOR_H
#define FONT_GENERATOR_H
#include <memory>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "text.h"
#include "font.h"

namespace Graphics {
  namespace UI {
    /**
     * @brief      Class for font generator.
     */
    class [[scriptable]] FontGenerator {
      private:
        FT_Library freetype;
        std::string font_path;
        unsigned int pixels_per_unit;
        std::map<std::string, std::shared_ptr<Font>> fonts;
      public:
        /**
         * @brief      Font generator constructor
         *
         * @param[in]  font_path        The font path
         * @param[in]  pixels_per_unit  The pixels per unit
         */
        FontGenerator(const std::string& font_path = "", const unsigned int pixels_per_unit = 32);
        /**
         * @brief      Destroys the font generator.
         */
        ~FontGenerator();

        /**
         * @brief      Sets the font path.
         *
         * @param[in]  font_path  The font path
         */
        [[scriptable]] void setFontPath(const std::string& font_path) noexcept;
        /**
         * @brief      Gets the font path.
         *
         * @return     The font path.
         */
        [[scriptable]] std::string getFontPath() const noexcept;

        /**
         * @brief      Loads a font.
         *
         * @param[in]  ttf_file  The ttf file
         * @param[in]  size      The size
         * @param[in]  name      The name
         */
        [[scriptable]] void loadFont(const std::string& ttf_file, const unsigned int size, const std::string& name="");
        /**
         * @brief      Gets the font.
         *
         * @param[in]  name  The name
         *
         * @return     The font.
         */
        [[scriptable]] std::shared_ptr<Font> getFont(const std::string& name) const noexcept;
    };
  }
}

#endif
