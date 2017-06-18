#include "./../../include/dynamics/collisionSphere.hpp"
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <glm/glm.hpp>

  collisionSphere::collisionSphere(glm::vec3 position, float radius)
  {
    this->position=position;
    this->radius=radius;
    // std::cout << "pos (" << this->position[0] << ", " << this->position[1] << ", " << this->position[2] << ")\n";
    // std::cout << "rayon= " <<this->radius << '\n';
  }
  glm::vec3 collisionSphere::getPosition()
  {
    return position;
  }
  void collisionSphere::setPosition(glm::vec3 p)
  {
    position=p;
  }
  float collisionSphere::collisionSphere::getRadius()
  {
    return radius;
  }
  void collisionSphere::setRadius(float r)
  {
    radius=r;
  }

  collisionSphere::~collisionSphere()
  {}
