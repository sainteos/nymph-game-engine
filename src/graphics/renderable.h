#ifndef RENDERABLE_H
#define RENDERABLE_H
#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include "component.h"
#include "vertex_data.h"
#include "shader.h"
#include "base_texture.h"
#include "renderable_attribute_trait.h"
#include "tile_attribute_trait.h"

namespace Graphics {
  class Renderable : Component {
    private:
      bool active;
      bool initialized;

      unsigned int vertex_array_object;
      unsigned int num_of_vertex_buffers;
      unsigned int* vertex_buffer_objects;
      unsigned int index_buffer_object;
      std::shared_ptr<Shader> shader;
      std::vector<std::shared_ptr<BaseTexture>> textures;

      VertexData vertex_data;
      std::unique_ptr<RenderableAttributeTrait> trait;

      //binds the texture
      //sets the texture sampler on the shader (don't forget to do this)
      void bindTextures();
    public:

      Renderable() = delete;
      Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data, 
                 RenderableAttributeTrait* trait = new TileAttributeTrait());
      //Remove copy constructor and assignment
      Renderable(const Renderable&) = delete;
      Renderable operator=(Renderable&) = delete;

      Renderable(Renderable&& renderable);
      Renderable& operator=(Renderable&& renderable);

      ~Renderable();
      
      void initialize();
      const bool isInitialized() const noexcept;

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;

      void setShader(std::shared_ptr<Shader> shader_object) noexcept;
      const std::shared_ptr<Shader> getShader() const noexcept;

      void addTexture(std::shared_ptr<BaseTexture> texture_object) noexcept;
      const std::vector<std::shared_ptr<BaseTexture>> getTextures() const noexcept;

      const unsigned int getVertexArrayBinding() const noexcept;
      const unsigned int getVertexBufferBinding(const VertexData::DATA_TYPE& data_type);
      const unsigned int getIndexBufferBinding() const noexcept;
      
      void destroy();
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
