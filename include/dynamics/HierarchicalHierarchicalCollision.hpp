#ifndef HIERARCHICAL_HIERARCHICAL_COLLISION_HPP
#define HIERARCHICAL_HIERARCHICAL_COLLISION_HPP

#include "Collision.hpp"
#include "../HierarchicalRenderable.hpp"

/**@brief Implement the resolution of a collision event between two particles.
 *
 * Implementation of the resolution of a collision between two particles.
 */
class HierarchicalHierarchicalCollision : public Collision
{
public:
    /**@brief Build a new collision event between two particles.
     *
     * Build a collision event between two particles.
     * @param particle1 The first colliding particle.
     * @param particle2 The second colliding particle.
     * @param restitution The restitution factor of the collision.
     */
    HierarchicalHierarchicalCollision( HierarchicalRenderablePtr h1,
      HierarchicalRenderablePtr h2, float restitution);

    virtual ~HierarchicalHierarchicalCollision();

private:
    void do_solveCollision();

    HierarchicalRenderablePtr m_h1;
    HierarchicalRenderablePtr m_h2;
};

typedef std::shared_ptr<HierarchicalHierarchicalCollision> HierarchicalHierarchicalCollisionPtr;

bool testHierarchicalHierarchical(const HierarchicalRenderablePtr& h1,
    const HierarchicalRenderablePtr& h2);

#endif //HIERARCHICAL_HIERARCHICAL_COLLISION_HPP
