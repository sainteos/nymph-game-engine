#include "renderable.h"

namespace Graphics {
  Renderable::Renderable() {

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

  const bool Renderable::onUpdate(const double delta) {
    return false;
  }

  const bool Renderable::onRender() {
    return true;
  }
}