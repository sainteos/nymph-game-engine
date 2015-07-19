#include <easylogging++.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/renderable_not_initialized_exception.h"
#include "renderable.h"

namespace Graphics {
  Renderable::Renderable(const unsigned int vertex_array_object) : initialized(false) {
    if(!glIsVertexArray(vertex_array_object)) {
      throw Exceptions::InvalidVertexArrayException(vertex_array_object);
    }
    this->vertex_array_object = vertex_array_object;
  }

  Renderable::~Renderable() {
    if(initialized) {
      destroy();
    }
  }

  void Renderable::initialize(const std::vector<std::vector<glm::vec3>>& vertex_data, const std::vector<unsigned int>& index_data) {
    LOG(INFO)<<"Renderable initializing...";
    glBindVertexArray(vertex_array_object);
    num_of_vertex_buffers = vertex_data.size();
    vertex_buffer_objects = new unsigned int[num_of_vertex_buffers];
    glGenBuffers(num_of_vertex_buffers, vertex_buffer_objects);
    //Lambda to collect glm vec3s into a single double vector
    auto collect = [](const std::vector<glm::vec3>& vec) {
      std::vector<double> out;
      for(auto vec3 : vec) {
        out.push_back(vec3.x);
        out.push_back(vec3.y);
        out.push_back(vec3.z);
      }
      return out;
    };

    for(int i = 0; i < num_of_vertex_buffers; i++) {
      std::vector<double> packed_vertex_data = collect(vertex_data[i]);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[i]);
      glBufferData(GL_ARRAY_BUFFER, vertex_data[i].size() * sizeof(double), &packed_vertex_data[0], GL_STATIC_DRAW);
    }

    //Do this if we actually have indices
    if(index_data.size() > 0) {
      glGenBuffers(1, &index_buffer_object);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(unsigned int), &index_data[0], GL_STATIC_DRAW);
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
  
  const unsigned int Renderable::getVertexArrayBinding() const noexcept {
    return vertex_array_object;
  }

  const unsigned int Renderable::getVertexBufferBinding(const unsigned int index) const {
    if(index >= num_of_vertex_buffers)
      throw std::out_of_range("Vertex Buffer index is out of range!");

    return vertex_buffer_objects[index];
  }

  const unsigned int Renderable::getIndexBufferBinding() const noexcept {
    return index_buffer_object;
  }

  void Renderable::destroy() {
    if(!initialized) {
      throw Exceptions::RenderableNotInitializedException();
    }

    glDeleteBuffers(num_of_vertex_buffers, vertex_buffer_objects);
    glDeleteBuffers(1, &index_buffer_object);
    glDeleteVertexArrays(1, &vertex_array_object);
    initialized = false;
    active = false;
  }

  const bool Renderable::onUpdate(const double delta) {
    return false;
  }

  const bool Renderable::onRender() {
    glBindVertexArray(vertex_array_object);
    return true;
  }
}