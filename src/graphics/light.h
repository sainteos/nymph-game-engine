#ifndef LIGHT_H
#define LIGHT_H
#include <easylogging++.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "../component.h"

namespace Graphics {
  /**
   * @brief      Class for light.
   */
  class [[scriptable]] Light : public Component {
    public:
      /**
       * @brief      Light type enum
       */
      enum [[scriptable]] Type : unsigned int { POINT, SPOT };
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
      virtual void onStart() override;
      virtual bool onUpdate(const double delta) override;
      virtual void onDestroy() override;

      /**
       * @brief      Light constructor
       *
       * @param[in]  type  The type
       */
      [[scriptable]] Light(const Light::Type& type = Type::POINT);
      /**
       * @brief      Destroys the object.
       */
      ~Light();
      /**
       * @brief      Sets the color.
       *
       * @param[in]  color  The color
       */
      [[scriptable]] void setColor(const glm::vec3 color) noexcept;
      /**
       * @brief      Gets the color.
       *
       * @return     The color.
       */
      [[scriptable]] glm::vec3 getColor() const noexcept;
      /**
       * @brief      Sets the intensity.
       *
       * @param[in]  intensity  The intensity
       */
      [[scriptable]] void setIntensity(const float intensity) noexcept;
      /**
       * @brief      Gets the intensity.
       *
       * @return     The intensity.
       */
      [[scriptable]] float getIntensity() const noexcept;
      /**
       * @brief      Sets the linear attenuation.
       *
       * @param[in]  linear  The linear
       */
      [[scriptable]] void setLinearAttenuation(const float linear) noexcept;
      /**
       * @brief      Gets the linear attenuation.
       *
       * @return     The linear attenuation.
       */
      [[scriptable]] float getLinearAttenuation() const noexcept;
      /**
       * @brief      Sets the quadratic attenuation.
       *
       * @param[in]  quadratic  The quadratic
       */
      [[scriptable]] void setQuadraticAttenuation(const float quadratic) noexcept;
      /**
       * @brief      Gets the quadratic attenuation.
       *
       * @return     The quadratic attenuation.
       */
      [[scriptable]] float getQuadraticAttenuation() const noexcept;

      /**
       * @brief      Function to see if lights cast quantized bands or are smooth
       *
       * @return     True if lights cast quantized bands
       */
      [[scriptable]] bool castsQuantizedBands() const noexcept;
      /**
       * @brief      Sets the number of quantized bands.
       *
       * @param[in]  bands  The bands
       */
      [[scriptable]] void setNumberOfQuantizedBands(const int bands) noexcept;
      /**
       * @brief      Gets the number of quantized bands.
       *
       * @return     The number of quantized bands.
       */
      [[scriptable]] int getNumberOfQuantizedBands() const noexcept;

      /**
       * @brief      Sets the cone angle.
       *
       * @param[in]  angle  The angle
       */
      [[scriptable]] void setConeAngle(const float angle) noexcept;
      /**
       * @brief      Gets the cone angle.
       *
       * @return     The cone angle.
       */
      [[scriptable]] float getConeAngle() const noexcept;
      /**
       * @brief      Sets the cone direction.
       *
       * @param[in]  direction  The direction
       */
      [[scriptable]] void setConeDirection(const glm::vec3 direction) noexcept;
      /**
       * @brief      Gets the cone direction.
       *
       * @return     The cone direction.
       */
      [[scriptable]] glm::vec3 getConeDirection() const noexcept;

      /**
       * @brief      Gets the type.
       *
       * @return     The type.
       */
      [[scriptable]] Light::Type getType() const noexcept;

      /**
       * @brief      Calculates a light's influence on a given component
       *
       * @param[in]  component  The component
       *
       * @return     Influence amount clamped 0.0, 1.0, on the given component
       */
      [[scriptable]] float influenceOnComponent(const Component& component) const;
      [[scriptable]] virtual std::string className() const noexcept override;

      virtual unsigned long long getValueForSorting() const noexcept override;

      virtual void log(el::base::type::ostream_t& os) const override;

      static inline const Light::Type stringToType(const std::string& str) {
        if(str == "Spot" || str == "spot" || str == "SPOT")
          return Light::Type::SPOT;
        else if(str == "Point" || str == "point" || str == "POINT")
          return Light::Type::POINT;
        else
          return Light::Type::POINT;
      }

      static inline const std::string typeToString(const Light::Type& type) {
        if(type == Light::Type::SPOT)
          return std::string("Spot");
        else if(type == Light::Type::POINT)
          return std::string("Point");
        else
          return std::string("N/A");
      }

  };
}

#endif
