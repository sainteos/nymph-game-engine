#include "font_generator.h"
#include "exceptions/invalid_filename_exception.h"
#include "exceptions/freetype_initialization_exception.h"
#include <easylogging++.h>
#include <glm/glm.hpp>

namespace Graphics {
  namespace UI {
    FontGenerator::FontGenerator(const std::string& font_path, const unsigned int pixels_per_unit) : font_path(font_path), pixels_per_unit(pixels_per_unit) {
      if (FT_Init_FreeType(&freetype)) {
        throw Exceptions::FreeTypeInitializationException();
      }
    }

    FontGenerator::~FontGenerator() {
      FT_Done_FreeType(freetype);
      for(auto i : fonts) {
        i.second = nullptr;
      }
      fonts.clear();
    }

    void FontGenerator::setFontPath(const std::string& font_path) noexcept {
      this->font_path = font_path;
    }

    const std::string FontGenerator::getFontPath() const noexcept {
      return font_path;
    }

    void FontGenerator::loadFont(const std::string& ttf_file, const unsigned int size, const std::string& name) {
      FT_Face face;
      if (FT_New_Face(freetype, (font_path + ttf_file).c_str(), 0, &face)) {
        throw Exceptions::InvalidFilenameException(font_path + ttf_file);
      }
      FT_Set_Pixel_Sizes(face, 0, size);
      
      int unpack_alignment_before;
      glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment_before);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      std::shared_ptr<Font> font = std::make_shared<Font>(size, pixels_per_unit);

      for (unsigned char c = 0; c < 128; c++)
      {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
          LOG(WARNING)<<"Freetype failed to load Glyph: "<<c;
          continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RED,
          face->glyph->bitmap.width,
          face->glyph->bitmap.rows,
          0,
          GL_RED,
          GL_UNSIGNED_BYTE,
          face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        float advance = (face->glyph->advance.x >> 6) / (float)pixels_per_unit;

        auto char_pos = glm::vec2(face->glyph->bitmap_left / (float)pixels_per_unit, ((float)face->glyph->bitmap_top - (float)face->glyph->bitmap.rows) / (float)pixels_per_unit);
        auto char_size = glm::vec2(face->glyph->bitmap.width / (float)pixels_per_unit, face->glyph->bitmap.rows / (float)pixels_per_unit);

        //Generate geometry
        std::vector<glm::vec2> texs {
          glm::vec2(0.0, 0.0),
          glm::vec2(0.0, 1.0),
          glm::vec2(1.0, 1.0),
          glm::vec2(1.0, 0.0)
        };
        std::vector<unsigned int> indices {
          0, 1, 2, 0, 2, 3
        };
        std::vector<glm::vec3> verts {
          glm::vec3(char_pos.x, char_pos.y + char_size.y, -0.1),
          glm::vec3(char_pos.x, char_pos.y, -0.1),
          glm::vec3(char_pos.x + char_size.x, char_pos.y, -0.1),
          glm::vec3(char_pos.x + char_size.x, char_pos.y + char_size.y, -0.1)
        };

        float added_width = char_pos.x + char_size.x;
        float added_height = char_pos.y + char_size.y;

        VertexData vert_data(GL_TRIANGLES);
        vert_data.addIndices(indices);
        vert_data.addVec<glm::vec3>(VertexData::DATA_TYPE::GEOMETRY, verts);
        vert_data.addVec<glm::vec2>(VertexData::DATA_TYPE::TEX_COORDS, texs);

        // Now store character for later use
        Character character = {
          texture, 
          char_pos,
          char_size,
          advance,
          vert_data,
          vert_data.generateVertexArrayObject()
        };

        font->addCharacter(c, character);
      }
      FT_Done_Face(face);
      glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment_before);
      glBindTexture(GL_TEXTURE_2D, 0);

      std::string stored_name = "";
      if(name == "") {
        auto dot_pos = ttf_file.find_last_of('.');
        stored_name = ttf_file.substr(0, dot_pos);
      }
      else {
        stored_name = name;
      }

      fonts[stored_name] = font;
    }

    std::shared_ptr<Font> FontGenerator::getFont(const std::string& name) const noexcept {
      if(fonts.count(name) == 0)
        return nullptr;
      return fonts.at(name);
    }
  }
}
