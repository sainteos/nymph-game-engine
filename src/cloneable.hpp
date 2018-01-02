#ifndef CLONEABLE_H
#define CLONEABLE_H
#include <memory>

template<class CloneType>
class Cloneable {
  public:
    virtual ~Cloneable() {}
    std::shared_ptr<CloneType> clone() {
      return std::shared_ptr<CloneType>(new CloneType(*dynamic_cast<CloneType*>(this)));
    }
};
#endif
