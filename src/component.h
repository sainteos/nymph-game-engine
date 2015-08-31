#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class Transform;

class Component {
  protected:
    std::shared_ptr<Transform> transform;
  public:
    virtual void onStart() = 0;
    virtual const bool onUpdate(const double delta) = 0;
    virtual void onDestroy() = 0;
    void setTransform(std::shared_ptr<Transform> transform);
    std::shared_ptr<Transform> getTransform() const noexcept;
    virtual ~Component() {}
};
#endif