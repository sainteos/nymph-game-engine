#include <easylogging++.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/renderable_not_initialized_exception.h"
#include "renderable.h"

namespace Graphics {
  Renderable::Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data) : initialized(false), vertex_data(vertex_data) {
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

  void Renderable::initialize() {
    LOG(INFO)<<"Renderable initializing...";
    glBindVertexArray(vertex_array_object);

    auto float_data = vertex_data.getCollapsedVectors<float>();
    auto double_data = vertex_data.getCollapsedVectors<double>();
    auto int_data = vertex_data.getCollapsedVectors<int>();
    auto unsigned_int_data = vertex_data.getCollapsedVectors<unsigned int>();

    num_of_vertex_buffers = float_data.size() + double_data.size() + int_data.size() + unsigned_int_data.size();
    vertex_buffer_objects = new unsigned int[num_of_vertex_buffers];
    glGenBuffers(num_of_vertex_buffers, vertex_buffer_objects);

    int current_buffer = 0;
    for(auto i : float_data) {
      type_to_buffer_index[i.first] = current_buffer;
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(float), &(i.second)[0], GL_STATIC_DRAW);
      glVertexAttribPointer((int)i.first, VertexData::DataWidth.at(i.first), GL_FLOAT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray((int)i.first);
    }

    for(auto i : double_data) {
      type_to_buffer_index[i.first] = current_buffer;
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(double), &(i.second)[0], GL_STATIC_DRAW);
      glVertexAttribPointer((int)i.first, VertexData::DataWidth.at(i.first), GL_DOUBLE, GL_FALSE, 0, 0);
      glEnableVertexAttribArray((int)i.first);
    }

    for(auto i : int_data) {
      type_to_buffer_index[i.first] = current_buffer;
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(int), &(i.second)[0], GL_STATIC_DRAW);
      glVertexAttribPointer((int)i.first, VertexData::DataWidth.at(i.first), GL_INT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray((int)i.first);
    }

    for(auto i : unsigned_int_data) {
      type_to_buffer_index[i.first] = current_buffer;
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects[current_buffer++]);
      glBufferData(GL_ARRAY_BUFFER, i.second.size() * sizeof(unsigned int), &(i.second)[0], GL_STATIC_DRAW);
      glVertexAttribPointer((int)i.first, VertexData::DataWidth.at(i.first), GL_UNSIGNED_INT, GL_FALSE, 0, 0);
      glEnableVertexAttribArray((int)i.first);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Do this if we actually have indices
    if(vertex_data.getIndices().size() > 0) {
      glGenBuffers(1, &index_buffer_object);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_data.getIndices().size() * sizeof(unsigned int), &(vertex_data.getIndices())[0], GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

  const unsigned int Renderable::getVertexBufferBinding(const VertexData::DATA_TYPE& data_type) {
    if(type_to_buffer_index.find(data_type) == type_to_buffer_index.end())
      throw std::invalid_argument("Renderable does not have data for data type.");

    return vertex_buffer_objects[type_to_buffer_index[data_type]];
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
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertex_array_object);

    type_to_buffer_index.clear();
    initialized = false;
    active = false;
  }

  const bool Renderable::onUpdate(const double delta) {
    return true;
  }

  const bool Renderable::onRender() {
    if(active) {
      glBindVertexArray(vertex_array_object);
      LOG(INFO)<<"Error 1: "<<glGetError();
      if(vertex_data.getIndexCount() > 0) {
        LOG(INFO)<<"With indices";
        glDrawElements(GL_TRIANGLES, vertex_data.getIndexCount(), GL_UNSIGNED_INT, 0);
      }
      else {
        LOG(INFO)<<"Without indices";
        glDrawArrays(GL_TRIANGLES, 0, vertex_data.getVertexCount());
      }

      LOG(INFO)<<"Error 2: "<<glGetError();
    }
    return true;
  }
}