#ifndef BASE_TEXTURE_H
#define BASE_TEXTURE_H
#include <memory>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include <string>
//= SCRIPTABLE

namespace Graphics {
  class BaseTexture {
    private:
      unsigned int texture_object;
      GLenum texture_type;
      bool loaded;
      unsigned int width;
      unsigned int height;
      std::string name;
    public:
      BaseTexture() = delete;
      //= BEGIN SCRIPTABLE
      BaseTexture(const GLenum texture_type);
      ~BaseTexture();

      const unsigned int getWidth() const noexcept;
      const unsigned int getHeight() const noexcept;
      
      virtual const bool load(const std::string& filename);
      virtual void bind(const unsigned int texture_unit);
      virtual const bool isLoaded() const noexcept;

      void setName(const std::string name) noexcept;
      const std::string getName() const noexcept;

      const std::string to_string() const noexcept;
      //= END SCRIPTABLE

      virtual const unsigned int getTextureObject() const noexcept;
  };
}

#endif
