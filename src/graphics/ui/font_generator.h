#ifndef FONT_GENERATOR_H
#define FONT_GENERATOR_H
#include <memory>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H 
#include "graphics/ui/text.h"
#include "graphics/ui/font.h"
//= SCRIPTABLE

namespace Graphics {
  namespace UI {
    class FontGenerator {
      private:
        FT_Library freetype;
        std::string font_path;
        unsigned int pixels_per_unit;
        std::map<std::string, std::shared_ptr<Font>> fonts;
      public:
        FontGenerator(const std::string& font_path = "", const unsigned int pixels_per_unit = 32);
        ~FontGenerator();

      //= BEGIN SCRIPTABLE
        void setFontPath(const std::string& font_path) noexcept;
        const std::string getFontPath() const noexcept;

        void loadFont(const std::string& ttf_file, const unsigned int size, const std::string& name="");
        std::shared_ptr<Font> getFont(const std::string& name) const noexcept;
      //= END SCRIPTABLE
    };
  }
}

#endif
