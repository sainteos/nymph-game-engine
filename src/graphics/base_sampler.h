#ifndef BASE_SAMPLER_H
#define BASE_SAMPLER_H

namespace Graphics {
  class BaseSampler {
    private:
      unsigned int sampler_object;
    public:
      BaseSampler();
      virtual ~BaseSampler();
      virtual void bind(const unsigned int texture_unit);
      virtual const unsigned int getSamplerObject() const noexcept;
  };
}
#endif
