#include <easylogging++.h>
#include <SOIL.h>
#include <IL/il.h>
#include "graphics/base_texture.h"
#include "exceptions/texture_not_loaded_exception.h"

namespace Graphics {
  BaseTexture::BaseTexture(const std::string& name, const GLenum texture_type, const unsigned int unit) : name(name), loaded(false), texture_unit(unit), texture_object(0), texture_type(texture_type), sampler(nullptr) {

  }

  BaseTexture::~BaseTexture() {
    glDeleteTextures(1, &texture_object);
  }

  const bool BaseTexture::load(const std::string& filename) {
    unsigned int image_id;
    ilGenImages(1, &image_id);
    ilBindImage(image_id);
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    auto success = ilLoadImage((const ILstring)filename.c_str());
    if(success) {
      glGenTextures(1, &texture_object);
      glActiveTexture(GL_TEXTURE0 + texture_unit);
      glBindTexture(texture_type, texture_object);
        
      if(ilGetInteger(IL_IMAGE_FORMAT) == IL_RGBA)
        glTexImage2D(texture_type, 0, GL_RGBA8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
      else
        glTexImage2D(texture_type, 0, GL_RGB8, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());

      glGenerateMipmap(texture_type);
      glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glBindTexture(texture_type, 0);
      loaded = true;
      LOG(INFO)<<"Texture file: "<<filename<<" loaded!";
    }
    else {
      LOG(WARNING)<<"Texture file: "<<filename<<" failed to load!";
    }
    ilDeleteImages(1, &image_id);
    return success;
  }

  void BaseTexture::setSampler(const std::shared_ptr<BaseSampler> sampler) {
    this->sampler = sampler;
  }

  void BaseTexture::bind() {
    if(!loaded)
      throw Exceptions::TextureNotLoadedException();

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(texture_type, texture_object);
    if(sampler != nullptr)
      sampler->bind(texture_unit);
  }

  const unsigned int BaseTexture::getTextureObject() const noexcept {
    return texture_object;
  }

  const unsigned int BaseTexture::getTextureUnit() const noexcept {
    return texture_unit;
  }

  const std::shared_ptr<BaseSampler> BaseTexture::getSampler() const noexcept {
    return sampler;
  }

  const bool BaseTexture::isLoaded() const noexcept {
    return loaded;
  }

  const std::string BaseTexture::getName() const noexcept {
    return name;
  }
}