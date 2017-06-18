# include "../../include/dynamics/Solver.hpp"

Solver::Solver()
{}

Solver::~Solver()
{}

void Solver::solve(const float& dt, std::vector<TexturedMeshRenderablePhysicPtr>& meshes)
{
    do_solve(dt, meshes);
}
