#ifndef BASE_TEXTURE_H
#define BASE_TEXTURE_H
#include <memory>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include <string>
#include "graphics/base_sampler.h"

namespace Graphics {
  class BaseTexture {
    private:
      unsigned int texture_object;
      GLenum texture_type;
      std::shared_ptr<BaseSampler> sampler;
      bool loaded;
      unsigned int width;
      unsigned int height;
    public:
      BaseTexture() = delete;
      BaseTexture(const GLenum texture_type);
      ~BaseTexture();

      const unsigned int getWidth() const noexcept;
      const unsigned int getHeight() const noexcept;
      
      virtual const bool load(const std::string& filename);
      virtual void setSampler(const std::shared_ptr<BaseSampler> sampler);
      virtual void bind(const unsigned int texture_unit);

      virtual const unsigned int getTextureObject() const noexcept;
      virtual const std::shared_ptr<BaseSampler> getSampler() const noexcept;
      virtual const bool isLoaded() const noexcept;
  };
}

#endif
