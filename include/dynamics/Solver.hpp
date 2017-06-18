#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <memory>
#include <vector>
#include "Particle.hpp"
#include "../texturing/TexturedMeshRenderablePhysic.hpp"
/**@brief Dynamic system solver interface.
 *
 * Define an interface for dynamic system solver.
 */
class Solver
{
public:
    Solver();
    virtual  ~Solver();

    /**@brief Solve the dynamic system of meshes.
     *
     * Solve the dynamic system of meshes for a specified time step.
     * @param dt The time step for the integration.
     * @param meshes The collection of meshes.
     */
    void solve(const float& dt, std::vector<TexturedMeshRenderablePhysicPtr>& meshes);

private:
    /**@brief Solve implementation.
     *
     * The actual implementation to solve the dynamic system. This should
     * be implemented in derived classes.
     * @param dt The time step for the integration.
     * @param meshes The collection of meshes.
     */
    virtual void do_solve(const float& dt, std::vector<TexturedMeshRenderablePhysicPtr>& meshes) = 0;
};

typedef std::shared_ptr<Solver> SolverPtr;

#endif //SOLVER_HPP
