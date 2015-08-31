#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include "component.h"
#include "vertex_data.h"
#include "shader.h"
#include "base_texture.h"
#include "base_attribute_trait.h"
#include "renderable_attribute_trait.h"
#include "transform.h"

namespace Graphics {
  class Renderable : public Component {
    private:
      bool active;

      unsigned int vertex_array_object;
      std::shared_ptr<Shader> shader;
      std::vector<std::shared_ptr<BaseTexture>> textures;

      VertexData vertex_data;
      std::unique_ptr<BaseAttributeTrait> trait;

    public:

      Renderable() = delete;
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data, 
                 BaseAttributeTrait* trait = new RenderableAttributeTrait());
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
