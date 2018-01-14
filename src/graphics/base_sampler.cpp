#include "base_sampler.h"
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glad/glad.h>
#endif

namespace Graphics {
  BaseSampler::BaseSampler() {
    glGenSamplers(1, &sampler_object);
    glSamplerParameteri(sampler_object, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glSamplerParameteri(sampler_object, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

  BaseSampler::~BaseSampler() {
    glDeleteSamplers(1, &sampler_object);
  }

  void BaseSampler::bind(const unsigned int texture_unit) {
    glBindSampler(texture_unit, sampler_object);
  }

  unsigned int BaseSampler::getSamplerObject() const noexcept {
    return sampler_object;
  }
}
