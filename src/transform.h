#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <list>

class Transform : public std::enable_shared_from_this<Transform> {
  private:
    std::weak_ptr<Transform> parent;
    std::list<std::shared_ptr<Transform>> children;

    glm::vec3 local_translation;
    glm::quat local_rotation;
    glm::vec3 local_scale;
  public:
    Transform();
    ~Transform();

    //does not take tree properties into account
    const bool operator==(const Transform& other);
    //does not take tree properties into account
    const bool operator!=(const Transform& other);

    Transform operator*(const Transform& other) const;

    void addChild(std::shared_ptr<Transform> transform);
    void removeChild(std::shared_ptr<Transform> transform);

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
};

#endif
