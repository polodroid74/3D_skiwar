#ifndef COLLISION_SPHERE_HPP
#define COLLISION_SPHERE_HPP


#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <glm/glm.hpp>

class collisionSphere
{
public:
  collisionSphere(glm::vec3 position, float radius);
  glm::vec3 getPosition();
  void setPosition(glm::vec3 p);
  float getRadius();
  void setRadius(float r);
  ~collisionSphere();
protected:
  glm::vec3 position;
  float radius;
};
#endif
