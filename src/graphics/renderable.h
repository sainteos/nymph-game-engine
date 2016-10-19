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
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data);
      static std::shared_ptr<Renderable> create(const VertexData& vertex_data);
 
      //Remove copy constructor and assignment
      Renderable(const Renderable&) = delete;
      Renderable operator=(Renderable&) = delete;

      Renderable(Renderable&& renderable);
      Renderable& operator=(Renderable&& renderable);

      virtual ~Renderable();

      //= BEGIN SCRIPTABLE
      void setShader(std::shared_ptr<Shader> shader_object) noexcept;
      const std::shared_ptr<Shader> getShader() const noexcept;

      void addTexture(const unsigned int unit, const std::string uniform_name, std::shared_ptr<BaseTexture> texture_object) noexcept;
      void removeTexture(const unsigned int unit);
      const std::map<unsigned int, std::shared_ptr<BaseTexture>> getTextures() const noexcept;

      void setLightReactive(const bool reactive) noexcept;
      const bool isLightReactive() const noexcept;

      void setAmbientLight(const glm::vec3 color) noexcept;
      const glm::vec3 getAmbientLight() const noexcept;
      void setAmbientIntensity(const float intensity) noexcept;
      const float getAmbientIntensity() const noexcept;

      void addInfluencingLight(std::shared_ptr<Light> light) noexcept;
      void clearInfluencingLights();

      const std::string to_string() const noexcept;
      
      virtual const std::string className() const noexcept override;
      //= END SCRIPTABLE

      const float highestZ() const noexcept;

      const unsigned int getVertexArrayBinding() const noexcept;

      const VertexData getVertexData() const noexcept;
      
      virtual void onDestroy() override;
      virtual void onStart() override;
      /**
       * @brief [brief description]
       * @details [long description]
       * @return true if anything was updated, false if nothing was updated
       */
      virtual const bool onUpdate(const double delta) override;

      void handleQueuedEvent(std::shared_ptr<Events::Event> event) override;
      void onNotifyNow(std::shared_ptr<Events::Event> event) override;
      virtual const unsigned long long getValueForSorting() const noexcept override;

      virtual void log(el::base::type::ostream_t& os) const;
  };
}
#endif
