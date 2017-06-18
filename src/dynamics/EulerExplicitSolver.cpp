#include "./../../include/dynamics/EulerExplicitSolver.hpp"
#include <iostream>
EulerExplicitSolver::EulerExplicitSolver()
{

}

EulerExplicitSolver::~EulerExplicitSolver()
{

}

void EulerExplicitSolver::do_solve(const float& dt, std::vector<TexturedMeshRenderablePhysicPtr>& meshes)
{
    for (TexturedMeshRenderablePhysicPtr p : meshes) {
			p->setVelocity(p->getVelocity()+p->getForce()/p->getMass());
			p->setPosition(p->getPosition()+dt*p->getVelocity());
      //std::cout << p <<"\n";
    }
}
