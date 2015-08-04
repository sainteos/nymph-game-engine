#ifndef BASE_TEXTURE_H
#define BASE_TEXTURE_H
#include <memory>
#include <OpenGL/gl3.h>
#include "graphics/base_sampler.h"

namespace Graphics {
  class BaseTexture {
    private:
      unsigned int texture_unit;
      unsigned int texture_object;
      GLenum texture_type;
      std::shared_ptr<BaseSampler> sampler;
      bool loaded;
      std::string name;
    public:
      BaseTexture() = delete;
      BaseTexture(const std::string& name, const GLenum texture_type, const unsigned int unit);
      ~BaseTexture();
      virtual const bool load(const std::string& filename);
      virtual void setSampler(const std::shared_ptr<BaseSampler> sampler);
      virtual void bind();

      virtual const unsigned int getTextureObject() const noexcept;
      virtual const unsigned int getTextureUnit() const noexcept;
      virtual const std::shared_ptr<BaseSampler> getSampler() const noexcept;
      virtual const bool isLoaded() const noexcept;

      virtual const std::string getName() const noexcept;
  };
}

#endif
