#include "./../../include/dynamics/HierarchicalHierarchicalCollision.hpp"
#include <glm/gtx/norm.hpp>
#include<iostream>
#include "./../../include/dynamics/collisionSphere.hpp"
HierarchicalHierarchicalCollision::HierarchicalHierarchicalCollision(HierarchicalRenderablePtr h1,
   HierarchicalRenderablePtr h2, float restitution) :
    Collision(restitution)
{
    m_h1 = h1;
    m_h2 = h2;
}

HierarchicalHierarchicalCollision::~HierarchicalHierarchicalCollision()
{
}

void HierarchicalHierarchicalCollision::do_solveCollision()
{
    //Don't process fixed particles (Let's assume that the ground plane is fixed)
    if (m_h1->isFixed() && m_h2->isFixed())
        return;

    //CompuisFixed()rpenetration distance
    float particleParticleDist = glm::distance(m_h1->getPosition(), m_h2->getPosition());
    float interpenetrationDist = m_h1->getRadius() + m_h2->getRadius() - particleParticleDist;

    //Compute particle-particle vector
    glm::vec3 k = glm::normalize(m_h1->getPosition() - m_h2->getPosition());

    //Project each particle along the particle-particle vector with half of the interpenetration distance
    //To be more precise, we ponderate the distance with the mass of the particle
    glm::vec3 prev_x1 = m_h1->getPosition();
    glm::vec3 prev_x2 = m_h2->getPosition();
    if (m_h1->isFixed()) {
        m_h2->setPosition(prev_x2 - interpenetrationDist*k);
    } else if(m_h2->isFixed()) {
        m_h1->setPosition(prev_x1 + interpenetrationDist*k);
    } else {
        float c1 = m_h1->getMass() / (m_h1->getMass() + m_h2->getMass());
        float c2 = m_h2->getMass() / (m_h1->getMass() + m_h2->getMass());
        m_h1->setPosition(prev_x1 + c2*interpenetrationDist*k);
        m_h2->setPosition(prev_x2 - c1*interpenetrationDist*k);
    }

    //Compute post-collision velocity
    glm::vec3 prev_v1 = m_h1->getVelocity();
    glm::vec3 prev_v2 = m_h2->getVelocity();
    float proj_v = (1.0f + m_restitution)
        * glm::dot(k, prev_v1 - prev_v2)
        / (1.0 / m_h1->getMass() + 1.0 / m_h2->getMass());
    glm::vec3 new_v1 = prev_v1 - proj_v/m_h1->getMass()*k;
    glm::vec3 new_v2 = prev_v2 + proj_v/m_h2->getMass()*k;
    m_h1->setVelocity(new_v1);
    m_h2->setVelocity(new_v2);
}


bool testHierarchicalHierarchical(const HierarchicalRenderablePtr & h1,
   const HierarchicalRenderablePtr & h2)
{
    /* p1( c1, r1 ) and p2( c2, r2 ) particles
   * intersection if || c2 - c1 || <= r1 + r2.
   * We avoid as much as possible to compute of a square root, so we check if
   * dot( c2 - c1, c2 - c1 ) <= (r1 + r2)²
   */

   //Vector between sphere centers

   if (h1 == h2){
     return false;
   }else{
     //Sum of sphere radii
     collisionSphere *cs1=h1->computeCollisionSphere();
     collisionSphere *cs2=h2->computeCollisionSphere();
     float r = cs1->getRadius() + cs2->getRadius();
     float c = glm::distance2(cs1->getPosition(), cs2->getPosition()) - r*r;
     if(c < 0.0f){
       return true;
     }
     return false;
   }
 }
