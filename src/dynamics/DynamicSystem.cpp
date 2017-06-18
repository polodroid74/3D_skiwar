#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include "./../../include/gl_helper.hpp"
#include "./../../include/dynamics/DynamicSystem.hpp"
#include "./../../include/dynamics/ParticlePlaneCollision.hpp"
#include "./../../include/dynamics/ParticleParticleCollision.hpp"
#include "./../../include/dynamics/HierarchicalHierarchicalCollision.hpp"

DynamicSystem::DynamicSystem() :
    m_dt(0.1), m_restitution(1.0), m_handleCollisions(true)
{
}

DynamicSystem::~DynamicSystem()
{
}

const std::vector<HierarchicalRenderablePtr>& DynamicSystem::getmeshes() const
{
    return m_meshes;
}

void DynamicSystem::setmeshes(const std::vector<HierarchicalRenderablePtr> &meshes)
{
    m_meshes = meshes;
}

const std::vector<ForceFieldPtr>& DynamicSystem::getForceFields() const
{
    return m_forceFields;
}

void DynamicSystem::setForceFields(const std::vector<ForceFieldPtr> &forceFields)
{
    m_forceFields = forceFields;
}


float DynamicSystem::getDt() const
{
    return m_dt;
}

void DynamicSystem::setDt(float dt)
{
    m_dt = dt;
}

void DynamicSystem::clear()
{
    m_meshes.clear();
    m_forceFields.clear();
    m_planeObstacles.clear();
}

bool DynamicSystem::getCollisionDetection()
{
    return m_handleCollisions;
}

void DynamicSystem::setCollisionsDetection(bool onOff)
{
    m_handleCollisions = onOff;
}

void DynamicSystem::addMesh(HierarchicalRenderablePtr m)
{
    m_meshes.push_back(m);
}
void DynamicSystem::addTexturedMeshRenderablePhysic(TexturedMeshRenderablePhysicPtr p)
{
  m_TexturedMeshRenderablePhysic.push_back(p);
}

void DynamicSystem::addForceField(ForceFieldPtr forceField)
{
    m_forceFields.push_back(forceField);
}

void DynamicSystem::addPlaneObstacle(PlanePtr planeObstacle)
{
    m_planeObstacles.push_back(planeObstacle);
}

SolverPtr DynamicSystem::getSolver()
{
    return m_solver;
}

void DynamicSystem::setSolver(SolverPtr solver)
{
    m_solver = solver;
}

void DynamicSystem::detectCollisions()
{
    //Detect particle plane collisions
    // for (ParticlePtr p : m_meshes) {
    //     for (PlanePtr o : m_planeObstacles) {
    //         if (testParticlePlane(p, o)) {
    //             ParticlePlaneCollisionPtr c =
    //                 std::make_shared<ParticlePlaneCollision>(p,o,m_restitution);
    //             m_collisions.push_back(c);
    //         }
    //     }
    // }

    //Detect particle particle collisions
    for (size_t i = 0; i < m_meshes.size(); ++i) {
      for (size_t j = i; j < m_meshes.size(); ++j) {
        HierarchicalRenderablePtr m1 = m_meshes[i];
        HierarchicalRenderablePtr m2 = m_meshes[j];
        if (testHierarchicalHierarchical(m1,m2)) {
          HierarchicalHierarchicalCollisionPtr c =
          std::make_shared<HierarchicalHierarchicalCollision>(m1,m2,m_restitution);
          m_collisions.push_back(c);
        }
      }
    }
  }

void DynamicSystem::solveCollisions()
{
    while (!m_collisions.empty()) {
        CollisionPtr collision = m_collisions.back();
        collision->solveCollision();
        m_collisions.pop_back();
    }
}

void DynamicSystem::computeSimulationStep()
{
    // //Reset the force for each particle
     for (TexturedMeshRenderablePhysicPtr p : m_TexturedMeshRenderablePhysic) {
         p->setForce(glm::vec3(0.0, 0.0, 0.0));
     }
    // //Compute forces
     for (ForceFieldPtr f : m_forceFields) {
         f->addForce();
     }

    //Integrate position and velocity of meshes
    m_solver->solve(m_dt, m_TexturedMeshRenderablePhysic);

    //Detect and resolve collisions
    if (m_handleCollisions) {
        detectCollisions();
        solveCollisions();
    }
}

const float DynamicSystem::getRestitution()
{
    return m_restitution;
}

void DynamicSystem::setRestitution(const float& restitution)
{
    m_restitution = std::max(0.0f,std::min(restitution,1.0f));
}

std::ostream& operator<<(std::ostream& os, const DynamicSystemPtr& system)
{
    std::vector<HierarchicalRenderablePtr> meshes = system->getmeshes();
    os << "Meshes number: " << meshes.size() << std::endl;
    for (HierarchicalRenderablePtr p : meshes)
        os << p << std::endl;
    return os;
}
