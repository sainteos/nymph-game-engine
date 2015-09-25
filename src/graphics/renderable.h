#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "component.h"
#include "vertex_data.h"
#include "shader.h"
#include "base_texture.h"
#include "transform.h"
#include "light.h"

namespace Graphics {
  class Renderable : public Component {
    private:
      bool active;

      unsigned int vertex_array_object;
      std::shared_ptr<Shader> shader;
      std::vector<std::shared_ptr<BaseTexture>> textures;

      VertexData vertex_data;

      bool light_reactive;
      std::list<std::shared_ptr<Light>> influencing_lights;
      glm::vec3 ambient_light;
      float ambient_intensity;
    protected:
      Renderable();

    public:
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data);
      //Remove copy constructor and assignment
      Renderable(const Renderable&) = delete;
      Renderable operator=(Renderable&) = delete;

      Renderable(Renderable&& renderable);
      Renderable& operator=(Renderable&& renderable);

      virtual ~Renderable();

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;

      void setShader(std::shared_ptr<Shader> shader_object) noexcept;
      const std::shared_ptr<Shader> getShader() const noexcept;

      void addTexture(std::shared_ptr<BaseTexture> texture_object) noexcept;
      const std::vector<std::shared_ptr<BaseTexture>> getTextures() const noexcept;
      const std::shared_ptr<BaseTexture> getTextureByUniform(const std::string& uniform_name);

      void setLightReactive(const bool reactive) noexcept;
      const bool isLightReactive() const noexcept;

      void setAmbientLight(const glm::vec3 color) noexcept;
      const glm::vec3 getAmbientLight() const noexcept;
      void setAmbientIntensity(const float intensity) noexcept;
      const float getAmbientIntensity() const noexcept;

      void addInfluencingLight(std::shared_ptr<Light> light) noexcept;
      void clearInfluencingLights();

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
      virtual const bool onRender();
  };
}
#endif
