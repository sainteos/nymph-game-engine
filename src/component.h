#ifndef COMPONENT_H
#define COMPONENT_H

class Component {
  public:
    virtual const bool onUpdate(const double delta) = 0;
    virtual void destroy() = 0;
    virtual ~Component() {}
};
#endif