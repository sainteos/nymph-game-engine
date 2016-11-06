#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <set>
#include "component.h"
#include "vertex_data.h"
#include "shader.h"
#include "base_texture.h"
#include "transform.h"
#include "light.h"
#include "events/observer.h"
#include "uniform.h"
//= SCRIPTABLE
//= SCRIPTABLE BASES Component

namespace Graphics {
  /**
   * @brief      Class for renderable.
   */
  class Renderable : public Component {
    private:
      unsigned int vertex_array_object;
      std::shared_ptr<Shader> shader;
      std::map<unsigned int, std::shared_ptr<BaseTexture>> textures;

      VertexData vertex_data;

      bool light_reactive;
      std::list<std::shared_ptr<Light>> influencing_lights;
      glm::vec3 ambient_light;
      float ambient_intensity;

      void setUniforms();
    protected:
      std::set<Uniform> uniforms;
      void setUniform(const Uniform& uniform) noexcept;
      Renderable() {}

    public:
      /**
       * @brief      Renderable constructor
       *
       * @param[in]  vertex_array_object  The vertex array object
       * @param[in]  vertex_data          The vertex data
       */
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data);
      /**
       * @brief      Renderable factory function
       *
       * @param[in]  vertex_data  The vertex data
       *
       * @return     newly created Renderable
       */
      static std::shared_ptr<Renderable> create(const VertexData& vertex_data);
 
      //Remove copy constructor and assignment
      Renderable(const Renderable&) = delete;
      Renderable operator=(Renderable&) = delete;

      Renderable(Renderable&& renderable);
      Renderable& operator=(Renderable&& renderable);

      virtual ~Renderable();

      //= BEGIN SCRIPTABLE
      
      /**
       * @brief      Sets the shader.
       *
       * @param[in]  shader_object  The shader object
       */
      void setShader(std::shared_ptr<Shader> shader_object) noexcept;
      /**
       * @brief      Gets the shader.
       *
       * @return     The shader.
       */
      const std::shared_ptr<Shader> getShader() const noexcept;
      
      /**
       * @brief      Adds a texture.
       *
       * @param[in]  unit            The unit
       * @param[in]  uniform_name    The uniform name
       * @param[in]  texture_object  The texture object
       */
      void addTexture(const unsigned int unit, const std::string uniform_name, std::shared_ptr<BaseTexture> texture_object) noexcept;
      /**
       * @brief      Removes a texture.
       *
       * @param[in]  unit  The unit
       */
      void removeTexture(const unsigned int unit);
      /**
       * @brief      Gets the textures
       * 
       * @return     A map of texture units to textures
       */
      const std::map<unsigned int, std::shared_ptr<BaseTexture>> getTextures() const noexcept;

      /**
       * @brief      Sets if the renderable should be light reactive
       *
       * @param[in]  reactive  True if reactive
       */
      void setLightReactive(const bool reactive) noexcept;
      /**
       * @brief      Determines if light reactive.
       *
       * @return     True if light reactive, False otherwise.
       */
      const bool isLightReactive() const noexcept;

      /**
       * @brief      Sets the ambient light.
       *
       * @param[in]  color  The color
       */
      void setAmbientLight(const glm::vec3 color) noexcept;
      /**
       * @brief      Gets the ambient light.
       *
       * @return     The ambient light.
       */
      const glm::vec3 getAmbientLight() const noexcept;
      /**
       * @brief      Sets the ambient intensity.
       *
       * @param[in]  intensity  The intensity
       */
      void setAmbientIntensity(const float intensity) noexcept;
      /**
       * @brief      Gets the ambient intensity.
       *
       * @return     The ambient intensity.
       */
      const float getAmbientIntensity() const noexcept;

      /**
       * @brief      Adds an influencing light.
       *
       * @param[in]  light  The light
       */
      void addInfluencingLight(std::shared_ptr<Light> light) noexcept;
      /**
       * @brief      Clears all influencing lights.
       */
      void clearInfluencingLights();

      /**
       * @brief      Returns a string representation of the object.
       *
       * @return     String representation of the object.
       */
      virtual const std::string to_string() const noexcept override;
      
      virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE
      
      /**
       * @brief      The highest possible z value in the renderable
       *
       * @return     A float representing the highest z value in the renderable
       */
      const float highestZ() const noexcept;

      /**
       * @brief      Gets the vertex array binding.
       *
       * @return     The vertex array binding.
       */
      const unsigned int getVertexArrayBinding() const noexcept;
      
      /**
       * @brief      Gets the vertex data.
       *
       * @return     The vertex data.
       */
      const VertexData getVertexData() const noexcept;
      
      virtual void onDestroy() override;
      virtual void onStart() override;
      virtual const bool onUpdate(const double delta) override;

      void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
      void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual const unsigned long long getValueForSorting() const noexcept override;

      virtual void log(el::base::type::ostream_t& os) const override;
  };
}
#endif
