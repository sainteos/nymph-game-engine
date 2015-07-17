#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "component.h"

namespace Graphics {
  class Renderable : Component {
    private:
      bool active;
    public:
      Renderable();

      void setActive() noexcept;
      void setInactive() noexcept;
      const bool isActive() const noexcept;
      
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
