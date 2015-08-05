#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <list>

#include "component.h"

class Transform : public Component, public std::enable_shared_from_this<Transform> {
  private:
    std::shared_ptr<Transform> parent;
    std::list<std::shared_ptr<Transform>> children;

    glm::mat4 absolute_matrix;

    glm::vec3 local_translation;
    glm::vec3 absolute_translation;

    glm::quat local_rotation;
    glm::quat absolute_rotation;

    glm::vec3 local_scale;
    glm::vec3 absolute_scale;

    
    void translate_absolute(const glm::vec3& translation);
    void rotate_absolute(const glm::quat& rotation);
    void scale_absolute(const glm::vec3& scale);
  public:
    Transform();
    ~Transform();

    //does not take tree properties into account
    const bool operator==(const Transform& other);
    //does not take tree properties into account
    const bool operator!=(const Transform& other);

    void addChild(const std::shared_ptr<Transform>& transform);

    std::list<std::shared_ptr<Transform>> getChildren() const;

    const unsigned int getTreeSize() const;

    template<class T>
    void translate(const T& translation);

    const glm::vec3 getAbsoluteTranslation() const noexcept;
    const glm::vec3 getLocalTranslation() const noexcept;

    void rotate(const float angle, const glm::vec3& axis);
    void rotate(const float angle_x, const float angle_y, const float angle_z);
    void rotate(const glm::vec3& euler_angles);
    void rotate(const glm::quat& quat);

    const glm::quat getAbsoluteRotation() const noexcept;
    const float getAbsoluteRotationAngle() const noexcept;
    const glm::vec3 getAbsoluteRotationAxis() const noexcept;
    const glm::vec3 getAbsoluteEulerAngles() const noexcept;

    const glm::quat getLocalRotation() const noexcept;
    const float getLocalRotationAngle() const noexcept;
    const glm::vec3 getLocalRotationAxis() const noexcept;
    const glm::vec3 getLocalEulerAngles() const noexcept;


    template<class T>
    void scale(const T& scale);

    const glm::vec3 getAbsoluteScale() const noexcept;
    const glm::vec3 getLocalScale() const noexcept;

    const glm::mat4 getAbsoluteTransformationMatrix() const noexcept;
    const glm::mat4 getLocalTransformationMatrix() const noexcept;

    virtual const bool onUpdate(const double delta) override;
    virtual void destroy() override;
};

#endif
