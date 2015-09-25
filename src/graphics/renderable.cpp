#include <easylogging++.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <glew.h>
#endif
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "exceptions/invalid_vertex_array_exception.h"
#include "exceptions/renderable_not_initialized_exception.h"
#include "exceptions/invalid_shader_object_exception.h"
#include "renderable.h"

namespace Graphics {
  Renderable::Renderable() : active(false), shader(nullptr), light_reactive(false), ambient_light(1.0), ambient_intensity(1.0) {

  }

  Renderable::Renderable(const unsigned int vertex_array_object, const VertexData& vertex_data) : active(false), vertex_data(vertex_data), shader(nullptr), light_reactive(false), ambient_light(1.0), ambient_intensity(1.0) {
    if(!glIsVertexArray(vertex_array_object)) {
      throw Exceptions::InvalidVertexArrayException(vertex_array_object);
    }
    this->vertex_array_object = vertex_array_object;
  }

  Renderable::Renderable(Renderable&& renderable) : vertex_data(renderable.vertex_data) {
    active = std::move(renderable.active);
    vertex_array_object = std::move(renderable.vertex_array_object);
    shader = renderable.shader;
    transform = renderable.transform;
    renderable.transform.reset();
    light_reactive = std::move(renderable.light_reactive);
  }

  Renderable& Renderable::operator=(Renderable&& renderable) { 
    active = std::move(renderable.active);
    vertex_array_object = std::move(renderable.vertex_array_object);
    shader = renderable.shader;
    vertex_data = renderable.vertex_data;
    transform = renderable.transform;
    renderable.transform.reset();
    light_reactive = std::move(renderable.light_reactive);

    return *this;
  }

  Renderable::~Renderable() {
    onDestroy();
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

  const std::shared_ptr<BaseTexture> Renderable::getTextureByUniform(const std::string& uniform_name) {
    auto tex_iter = std::find_if(textures.begin(), textures.end(), [&](std::shared_ptr<BaseTexture> t) { return t->getTextureUniformName() == uniform_name; });
    if(tex_iter == textures.end()) {
      return nullptr;
    }
    else {
      return *tex_iter;
    }
  }

  void Renderable::setLightReactive(const bool reactive) noexcept {
    light_reactive = reactive;
  }

  const bool Renderable::isLightReactive() const noexcept {
    return light_reactive;
  }

  void Renderable::setAmbientLight(const glm::vec3 color) noexcept {
    ambient_light = color;
  }

  const glm::vec3 Renderable::getAmbientLight() const noexcept {
    return ambient_light;
  }

  void Renderable::setAmbientIntensity(const float intensity) noexcept {
    ambient_intensity = intensity;
  }

  const float Renderable::getAmbientIntensity() const noexcept {
    return ambient_intensity;
  }

  void Renderable::addInfluencingLight(std::shared_ptr<Light> light) noexcept {
    influencing_lights.push_back(light);
  }

  void Renderable::clearInfluencingLights() {
    influencing_lights.clear();
  }
  
  const unsigned int Renderable::getVertexArrayBinding() const noexcept {
    return vertex_array_object;
  }

  const VertexData Renderable::getVertexData() const noexcept {
    return vertex_data;
  }

  void Renderable::onDestroy() {
    active = false;
    //not this one's job to destroy shader
    shader = nullptr;
    textures.clear();
  }

  void Renderable::onStart() {
    if(light_reactive) {
      shader->setUniform("ambient_color", ambient_light);
      shader->setUniform("ambient_intensity", ambient_intensity);
    }
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
        if(light_reactive) {
          shader->setUniform("num_lights", (int)influencing_lights.size());
          int index = 0;
          std::stringstream light_str;
          for(auto& light : influencing_lights) {
            light_str << "lights[" << index << "].";
            shader->setUniform(light_str.str() + "position", light->getTransform()->getAbsoluteTranslation());
            shader->setUniform(light_str.str() + "color", light->getColor());
            shader->setUniform(light_str.str() + "intensity", light->getIntensity());
            shader->setUniform(light_str.str() + "linear_attenuation", light->getLinearAttenuation());
            if(light->castsQuantizedBands()) {
              shader->setUniform(light_str.str() + "number_quantized_bands", light->getNumberOfQuantizedBands());
            }
            if(light->getType() == Light::Type::POINT) {
              shader->setUniform(light_str.str() + "cone_angle", 0.0f);
            }
            else if(light->getType() == Light::Type::SPOT) {
              shader->setUniform(light_str.str() + "cone_angle", light->getConeAngle());
              shader->setUniform(light_str.str() + "cone_direction", light->getConeDirection());
            }
            light_str.str(std::string());
            index++;
          }
        }

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