#include <easylogging++.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/renderable_not_initialized_exception.h"
#include "exceptions/invalid_shader_object_exception.h"
#include "renderable.h"

namespace Graphics {
  Renderable::Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data, BaseAttributeTrait* ra_trait) : vertex_data(vertex_data), trait(std::unique_ptr<BaseAttributeTrait>(std::move(ra_trait))), shader(nullptr), transform(std::make_shared<Transform>()) {
    if(!glIsVertexArray(vertex_array_object)) {
      throw Exceptions::InvalidVertexArrayException(vertex_array_object);
    }
    if(trait == nullptr) {
      throw std::invalid_argument("Constructor: Can't pass a nullptr as a trait, this argument is required");
    }
    this->vertex_array_object = vertex_array_object;
  }

  Renderable::Renderable(Renderable&& renderable) : vertex_data(renderable.vertex_data) {
    active = std::move(renderable.active);
    vertex_array_object = std::move(renderable.vertex_array_object);
    shader = renderable.shader;
    trait.reset(std::move(renderable.trait.release()));
    transform = renderable.transform;
    renderable.transform.reset();
    if(trait == nullptr) {
      throw std::invalid_argument("Move constructor: Can't pass a nullptr as a trait, this argument is required");
    }
  }

  Renderable& Renderable::operator=(Renderable&& renderable) { 
    active = std::move(renderable.active);
    vertex_array_object = std::move(renderable.vertex_array_object);
    shader = renderable.shader;
    vertex_data = renderable.vertex_data;
    trait.reset(std::move(renderable.trait.release()));
    transform = renderable.transform;
    renderable.transform.reset();
    if(trait == nullptr) {
      throw std::invalid_argument("Move assignment: Can't pass a nullptr as a trait, this argument is required");
    }

    return *this;
  }

  Renderable::~Renderable() {
    destroy();
  }
  
  void Renderable::setActive() noexcept {
    active = true;
  }

  void Renderable::setInactive() noexcept {
    active = false;
  }

  const bool Renderable::isActive() const noexcept {
    return active;
  }

  void Renderable::setShader(const std::shared_ptr<Shader> shader_object) noexcept {
    this->shader = shader_object;
  }

  const std::shared_ptr<Shader> Renderable::getShader() const noexcept {
    return shader;
  }

  void Renderable::addTexture(std::shared_ptr<BaseTexture> texture_object) noexcept {
    textures.push_back(texture_object);
  }

  const std::vector<std::shared_ptr<BaseTexture>> Renderable::getTextures() const noexcept {
    return textures;
  }
  
  const unsigned int Renderable::getVertexArrayBinding() const noexcept {
    return vertex_array_object;
  }

  void Renderable::setTransform(std::shared_ptr<Transform> transformation_matrix) noexcept {
    transform = transformation_matrix;
  }

  const std::shared_ptr<Transform> Renderable::getTransform() const noexcept {
    return transform;
  }

  const VertexData Renderable::getVertexData() const noexcept {
    return vertex_data;
  }

  void Renderable::destroy() {
    active = false;
    //not this one's job to destroy shader
    shader = nullptr;
    textures.clear();
    trait.reset();
  }

  void Renderable::onStart() {
    
  }

  const bool Renderable::onUpdate(const double delta) {
    return true;
  }

  const bool Renderable::onRender() {
    if(active) {
      if(!glIsVertexArray(vertex_array_object)) {
        throw Exceptions::InvalidVertexArrayException(vertex_array_object);
      }

      glBindVertexArray(vertex_array_object);

      if(shader != nullptr) {
        shader->useProgram();
        shader->setUniform<glm::mat4>("transform", transform->getAbsoluteTransformationMatrix());
        for(auto& texture : textures) {
          shader->setUniform<int>(texture->getTextureUniformName(), texture->getTextureUnit());
          texture->bind();
        }
      }
      else {
        LOG(WARNING)<<"Trying to render renderable with nullptr shader";
      }

      if(vertex_data.getIndexCount() > 0) {
        glDrawElements(GL_TRIANGLES, vertex_data.getIndexCount(), GL_UNSIGNED_INT, 0);
      }
      else {
        glDrawArrays(GL_TRIANGLES, 0, vertex_data.getVertexCount());
      }

      return true;
    }
    else {
      return false;
    }
  }
}