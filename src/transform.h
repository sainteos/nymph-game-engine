#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <list>

//= SCRIPTABLE

/**
 * @brief      Class for transform.
 */
class Transform : public std::enable_shared_from_this<Transform> {
  private:
    std::weak_ptr<Transform> parent;
    std::list<std::shared_ptr<Transform>> children;

    glm::vec3 local_translation;
    glm::quat local_rotation;
    glm::vec3 local_scale;
  public:

    //= BEGIN SCRIPTABLE
    
    /**
     * @brief      Transform Constructor
     */
    Transform();
    /**
     * @brief      Destroys the object.
     */
    ~Transform();

    /**
     * @brief      operator ==
     *
     * @param[in]  other  The other
     * 
     * @detail Does not take tree properties into account
     *
     * @return     True if same
     */
    const bool operator==(const Transform& other);
    /**
     * @brief      operator !=
     *
     * @param[in]  other  The other
     * 
     * @detail Does not take tree properties into account
     *
     * @return     True if not same
     */
    const bool operator!=(const Transform& other);

    /**
     * @brief      Essentially a matrix multiplication, combines translations, rotations, and scales
     *
     * @param[in]  other  The other
     *
     * @return     New transform with the result
     */
    Transform operator*(const Transform& other) const;

    /**
     * @brief      Adds a child.
     *
     * @param[in]  transform  The transform
     */
    void addChild(std::shared_ptr<Transform> transform);
    /**
     * @brief      Removes a child.
     *
     * @param[in]  transform  The transform
     */
    void removeChild(std::shared_ptr<Transform> transform);

    /**
     * @brief      Gets the children
     * 
     * @return     list of Transform pointers
     */
    std::list<std::shared_ptr<Transform>> getChildren() const;
    /**
     * @brief      Gets the parent.
     *
     * @return     The parent.
     */
    std::shared_ptr<Transform> getParent() const;

    /**
     * @brief      Gets the tree size.
     *
     * @return     The tree size.
     */
    const unsigned int getTreeSize() const;

    /**
     * @brief      Adds 2d translation to Transform
     *
     * @param[in]  x     
     * @param[in]  y     
     */
    void translate(const float x, const float y) noexcept;
    /**
     * @brief      Adds 3d translation to Transform
     *
     * @param[in]  x     
     * @param[in]  y     
     * @param[in]  z     
     */
    void translate(const float x, const float y, const float z) noexcept;
    /**
     * @brief      Adds 3d translation
     *
     * @param[in]  vec   The vector
     */
    void translate(const glm::vec3& vec) noexcept;
    /**
     * @brief      Adds 2d translation
     *
     * @param[in]  vec   The vector
     */
    void translate(const glm::vec2& vec) noexcept;

    /**
     * @brief      Gets the absolute translation.
     *
     * @return     The absolute translation.
     */
    const glm::vec3 getAbsoluteTranslation() const noexcept;
    /**
     * @brief      Gets the local translation.
     *
     * @return     The local translation.
     */
    const glm::vec3 getLocalTranslation() const noexcept;

    /**
     * @brief      Rotates this transform using yaw roll pitch/euler angles
     *
     * @param[in]  angle_x  The angle x
     * @param[in]  angle_y  The angle y
     * @param[in]  angle_z  The angle z
     */
    void rotate(const float angle_x, const float angle_y, const float angle_z) noexcept;
    /**
     * @brief      Rotates using euler angles
     *
     * @param[in]  euler_angles  The euler angles
     */
    void rotate(const glm::vec3& euler_angles) noexcept;
    /**
     * @brief      Rotates using a quaternion
     *
     * @param[in]  quat  The quaternion
     */
    void rotate(const glm::quat& quat) noexcept;
    /**
     * @brief      Rotates using angle axis
     *
     * @param[in]  angle  The angle
     * @param[in]  axis   The axis
     */
    void rotate(const float angle, const glm::vec3& axis) noexcept;

    /**
     * @brief      Gets the absolute rotation.
     *
     * @return     The absolute rotation.
     */
    const glm::quat getAbsoluteRotation() const noexcept;
    /**
     * @brief      Gets the absolute rotation angle.
     *
     * @return     The absolute rotation angle.
     */
    const float getAbsoluteRotationAngle() const noexcept;
    /**
     * @brief      Gets the absolute rotation axis.
     *
     * @return     The absolute rotation axis.
     */
    const glm::vec3 getAbsoluteRotationAxis() const noexcept;
    /**
     * @brief      Gets the absolute euler angles.
     *
     * @return     The absolute euler angles.
     */
    const glm::vec3 getAbsoluteEulerAngles() const noexcept;

    /**
     * @brief      Gets the local rotation.
     *
     * @return     The local rotation.
     */
    const glm::quat getLocalRotation() const noexcept;
    /**
     * @brief      Gets the local rotation angle.
     *
     * @return     The local rotation angle.
     */
    const float getLocalRotationAngle() const noexcept;
    /**
     * @brief      Gets the local rotation axis.
     *
     * @return     The local rotation axis.
     */
    const glm::vec3 getLocalRotationAxis() const noexcept;
    /**
     * @brief      Gets the local euler angles.
     *
     * @return     The local euler angles.
     */
    const glm::vec3 getLocalEulerAngles() const noexcept;

    /**
     * @brief      Scales transform in all 3 directions by x
     *
     * @param[in]  x     
     */
    void scale(const float x) noexcept;
    /**
     * @brief      Scales transform in 2d
     *
     * @param[in]  x     
     * @param[in]  y     
     */
    void scale(const float x, const float y) noexcept;
    /**
     * @brief      Scales transform in 3d
     *
     * @param[in]  x     
     * @param[in]  y     
     * @param[in]  z     
     */
    void scale(const float x, const float y, const float z) noexcept;
    /**
     * @brief      Scales transform in 2d
     *
     * @param[in]  vec   The vector
     */
    void scale(const glm::vec2& vec) noexcept;
    /**
     * @brief      Scales transform in 3d
     *
     * @param[in]  vec   The vector
     */
    void scale(const glm::vec3& vec) noexcept;

    /**
     * @brief      Gets the absolute scale.
     *
     * @return     The absolute scale.
     */
    const glm::vec3 getAbsoluteScale() const noexcept;
    /**
     * @brief      Gets the local scale.
     *
     * @return     The local scale.
     */
    const glm::vec3 getLocalScale() const noexcept;

    /**
     * @brief      Gets the absolute transformation matrix.
     *
     * @return     The absolute transformation matrix.
     */
    const glm::mat4 getAbsoluteTransformationMatrix() const noexcept;
    /**
     * @brief      Gets the local transformation matrix.
     *
     * @return     The local transformation matrix.
     */
    const glm::mat4 getLocalTransformationMatrix() const noexcept;
    /**
     * @brief      Returns a string representation of the object.
     *
     * @return     String representation of the object.
     */
    const std::string to_string() const noexcept;
    //= END SCRIPTABLE
};

#endif
