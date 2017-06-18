#include "./../../include/dynamics/ConstantForceField.hpp"

ConstantForceField::ConstantForceField(const std::vector<TexturedMeshRenderablePhysicPtr>& meshes, const glm::vec3& force)
{
    m_meshes = meshes;
    m_force = force;
}

ConstantForceField::~ConstantForceField()
{
}

void ConstantForceField::do_addForce()
{
    for (TexturedMeshRenderablePhysicPtr p : m_meshes) {
        p->incrForce(m_force*p->getMass());
    }
}

const std::vector<TexturedMeshRenderablePhysicPtr> ConstantForceField::getmeshes()
{
    return m_meshes;
}

void ConstantForceField::setmeshes(const std::vector<TexturedMeshRenderablePhysicPtr>& meshes)
{
    m_meshes = meshes;
}

const glm::vec3& ConstantForceField::getForce()
{
    return m_force;
}

void ConstantForceField::setForce(const glm::vec3& force)
{
    m_force = force;
}
