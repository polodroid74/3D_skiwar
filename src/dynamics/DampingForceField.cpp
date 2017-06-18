#include "./../../include/dynamics/DampingForceField.hpp"

DampingForceField::DampingForceField(const std::vector<TexturedMeshRenderablePhysicPtr> meshes, const float damping)
{
    m_meshes = meshes;
    m_damping = damping;
}

DampingForceField::~DampingForceField()
{
}

void DampingForceField::do_addForce()
{
    for (TexturedMeshRenderablePhysicPtr p : m_meshes) {
        p->incrForce(-m_damping*p->getVelocity());
    }
}

const std::vector<TexturedMeshRenderablePhysicPtr> DampingForceField::getmeshes()
{
    return m_meshes;
}

void DampingForceField::setmeshes(const std::vector<TexturedMeshRenderablePhysicPtr>& meshes)
{
    m_meshes = meshes;
}

const float& DampingForceField::getDamping()
{
    return m_damping;
}

void DampingForceField::setDamping(const float& damping)
{
    m_damping = damping;
}
