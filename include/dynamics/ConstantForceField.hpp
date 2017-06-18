#ifndef CONSTANT_FORCE_FIELD_HPP
#define CONSTANT_FORCE_FIELD_HPP

#include <vector>
#include "ForceField.hpp"
#include "Particle.hpp"
#include "../texturing/TexturedMeshRenderablePhysic.hpp"
/**@brief Constant force field.
 *
 * Implementation of a force field that is constant, i.e. the same for
 * all managed meshes.
 */
class ConstantForceField : public ForceField
{
public:
    /**@brief Build a constant force field.
     *
     * Build a constant force field for a set of meshes.
     * @param meshes The set of meshes influenced by this force field.
     * @param force The constant force applied to all meshes.
     */
    ConstantForceField(const std::vector<TexturedMeshRenderablePhysicPtr>& meshes, const glm::vec3& force);
    virtual ~ConstantForceField();

    /**@brief Access to the set of managed meshes.
     *
     * Get the set of managed meshes of this constant force field.
     * @return The managed force field.
     */
    const std::vector<TexturedMeshRenderablePhysicPtr> getmeshes();

    /**@brief Define a nThe new set of meshes.
     */
    void setmeshes(const std::vector<TexturedMeshRenderablePhysicPtr>& meshes);

    /**@brief Access to the force applied to all influenced meshes.
     *
     * Get the constant force of this force field.
     * @return The force of this force field.
     */
    const glm::vec3& getForce();

    /**@brief Set the force applied to all influenced meshes.
     *
     * Set the force applied to all meshes influenced by this force field.
     * @param force The new force.
     */
    void setForce(const glm::vec3& force);

private:
    void do_addForce();
    std::vector<TexturedMeshRenderablePhysicPtr> m_meshes;
    glm::vec3 m_force;
};

typedef std::shared_ptr<ConstantForceField> ConstantForceFieldPtr;

#endif // SPRING_HPP
