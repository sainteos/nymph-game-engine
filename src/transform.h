#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <list>

//= SCRIPTABLE

class Transform : public std::enable_shared_from_this<Transform> {
  private:
    std::weak_ptr<Transform> parent;
    std::list<std::shared_ptr<Transform>> children;

    glm::vec3 local_translation;
    glm::quat local_rotation;
    glm::vec3 local_scale;
  public:

    //= BEGIN SCRIPTABLE
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
    std::shared_ptr<Transform> getParent() const;

    const unsigned int getTreeSize() const;

    void translate(const float x, const float y) noexcept;
    void translate(const float x, const float y, const float z) noexcept;
    void translate(const glm::vec3& vec) noexcept;
    void translate(const glm::vec2& vec) noexcept;

    const glm::vec3 getAbsoluteTranslation() const noexcept;
    const glm::vec3 getLocalTranslation() const noexcept;

    void rotate(const float angle_x, const float angle_y, const float angle_z) noexcept;
    void rotate(const glm::vec3& euler_angles) noexcept;
    void rotate(const glm::quat& quat) noexcept;
    void rotate(const float angle, const glm::vec3& axis) noexcept;

    const glm::quat getAbsoluteRotation() const noexcept;
    const float getAbsoluteRotationAngle() const noexcept;
    const glm::vec3 getAbsoluteRotationAxis() const noexcept;
    const glm::vec3 getAbsoluteEulerAngles() const noexcept;

    const glm::quat getLocalRotation() const noexcept;
    const float getLocalRotationAngle() const noexcept;
    const glm::vec3 getLocalRotationAxis() const noexcept;
    const glm::vec3 getLocalEulerAngles() const noexcept;

    void scale(const float x) noexcept;
    void scale(const float x, const float y) noexcept;
    void scale(const float x, const float y, const float z) noexcept;
    void scale(const glm::vec2& vec) noexcept;
    void scale(const glm::vec3& vec) noexcept;

    const glm::vec3 getAbsoluteScale() const noexcept;
    const glm::vec3 getLocalScale() const noexcept;

    const glm::mat4 getAbsoluteTransformationMatrix() const noexcept;
    const glm::mat4 getLocalTransformationMatrix() const noexcept;

    const std::string to_string() const noexcept;
    //= END SCRIPTABLE

};

#endif
