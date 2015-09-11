#ifndef LIGHT_H
#define LIGHT_H
#include <glm/glm.hpp>
#include "component.h"

namespace Graphics {
  class Light : public Component {
    public:
      enum class Type { POINT, SPOT };
    private:
      Type type;
      glm::vec3 color;
      float intensity;
      float linear_attenuation;
      float quadratic_attenuation;
      int quantized_bands;
      float cone_angle;
      glm::vec3 cone_direction;
    public:
      Light(const Type& type = Type::POINT);
      ~Light();

      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      void setColor(const glm::vec3 color) noexcept;
      const glm::vec3 getColor() const noexcept;
      void setIntensity(const float intensity) noexcept;
      const float getIntensity() const noexcept;
      void setLinearAttenuation(const float linear) noexcept;
      const float getLinearAttenuation() const noexcept;
      void setQuadraticAttenuation(const float quadratic) noexcept;
      const float getQuadraticAttenuation() const noexcept;

      const bool castsQuantizedBands() const noexcept;
      void setNumberOfQuantizedBands(const int bands) noexcept;
      const int getNumberOfQuantizedBands() const noexcept;

      void setConeAngle(const float angle) noexcept;
      const float getConeAngle() const noexcept;
      void setConeDirection(const glm::vec3 direction) noexcept;
      const glm::vec3 getConeDirection() const noexcept;

      const Light::Type getType() const noexcept;

      const float influenceOnComponent(const Component& component) const;

  };
}

#endif
