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
  Renderable::Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data, BaseAttributeTrait* ra_trait) : initialized(false), vertex_data(vertex_data), trait(std::unique_ptr<BaseAttributeTrait>(std::move(ra_trait))), shader(nullptr), transform(std::make_shared<Transform>()) {
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
    initialized = std::move(renderable.initialized);
    vertex_array_object = std::move(renderable.vertex_array_object);
    num_of_vertex_buffers = std::move(renderable.num_of_vertex_buffers);
    vertex_buffer_objects = std::move(renderable.vertex_buffer_objects);
    index_buffer_object = std::move(renderable.index_buffer_object);
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
    initialized = std::move(renderable.initialized);
    vertex_array_object = std::move(renderable.vertex_array_object);
    num_of_vertex_buffers = std::move(renderable.num_of_vertex_buffers);
    vertex_buffer_objects = std::move(renderable.vertex_buffer_objects);
    index_buffer_object = std::move(renderable.index_buffer_object);
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
    if(initialized) {
      destroy();
    }
  }

  void Renderable::initialize() {
    LOG(INFO)<<"Renderable initializing...";

    auto float_data = vertex_data.getCollapsedVectors<float>();
    auto double_data = vertex_data.getCollapsedVectors<double>();
    auto int_data = vertex_data.getCollapsedVectors<int>();
    auto unsigned_int_data = vertex_data.getCollapsedVectors<unsigned int>();
    
    num_of_vertex_buffers = (*trait)().size();
    vertex_buffer_objects = new unsigned int[num_of_vertex_buffers];
    glGenBuffers(num_of_vertex_buffers, vertex_buffer_objects);

    
    //Do this if we actually have indices
    if(vertex_data.getIndices().size() > 0) {
      glGenBuffers(1, &index_buffer_object);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_data.getIndices().size() * sizeof(unsigned int), &(vertex_data.getIndices())[0], GL_STATIC_DRAW);
    }

    for(auto i : float_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(float), &(i.second)[0], GL_STATIC_DRAW);
      }
      else {
        LOG(WARNING)<<"Float data trying to be added that doesn't conform to trait.";
      }
    }

    for(auto i : double_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(double), &(i.second)[0], GL_STATIC_DRAW);
      }
      else {
        LOG(WARNING)<<"Double data trying to be added that doesn't conform to trait.";
      }
    }

    for(auto i : int_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(int), &(i.second)[0], GL_STATIC_DRAW);
      }
      else {
        LOG(WARNING)<<"Int Data trying to be added that doesn't conform to trait.";
      }
    }

    for(auto i : unsigned_int_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(unsigned int), &(i.second)[0], GL_STATIC_DRAW);
      }
      else {
        LOG(WARNING)<<"Unsigned int Data trying to be added that doesn't conform to trait.";
      }
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vertex_array_object);
    for(auto i : float_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glVertexAttribPointer((*trait)().at(i.first), VertexData::DataWidth.at(i.first), GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray((*trait)().at(i.first));
      }
    }
    for(auto i : double_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glVertexAttribPointer((*trait)().at(i.first), VertexData::DataWidth.at(i.first), GL_DOUBLE, GL_FALSE, 0, 0);
        glEnableVertexAttribArray((*trait)().at(i.first));
      }
    }
    for(auto i : int_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glVertexAttribPointer((*trait)().at(i.first), VertexData::DataWidth.at(i.first), GL_INT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray((*trait)().at(i.first));
      }
    }
    for(auto i : unsigned_int_data) {
      if((*trait)().count(i.first) > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[(*trait)().at(i.first)]);
        glVertexAttribPointer((*trait)().at(i.first), VertexData::DataWidth.at(i.first), GL_UNSIGNED_INT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray((*trait)().at(i.first));
      }
    }
    if(vertex_data.getIndices().size() > 0) {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
    }
    glBindVertexArray(0);

    initialized = true;
    LOG(INFO)<<"Renderable initialized!";
  }

  const bool Renderable::isInitialized() const noexcept {
    return initialized;
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

  const unsigned int Renderable::getVertexBufferBinding(const VertexData::DATA_TYPE& data_type) {
    if((*trait)().count(data_type) <= 0)
      throw std::invalid_argument("Renderable does not have data for data type.");

    return vertex_buffer_objects[(*trait)().at(data_type)];
  }

  const unsigned int Renderable::getIndexBufferBinding() const noexcept {
    return index_buffer_object;
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
    if(!initialized) {
      throw Exceptions::RenderableNotInitializedException();
    }
    
    glDeleteBuffers(num_of_vertex_buffers, vertex_buffer_objects);
    glDeleteBuffers(1, &index_buffer_object);
    glBindVertexArray(0);
    initialized = false;
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