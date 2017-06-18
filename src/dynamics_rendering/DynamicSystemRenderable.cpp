#include <cmath>
#include <iostream>
#include <map>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include "../../include/gl_helper.hpp"
#include "../../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../../include/Viewer.hpp"

DynamicSystemRenderable::DynamicSystemRenderable(DynamicSystemPtr system) :
    HierarchicalRenderable(nullptr), m_lastUpdateTime(0)
{
    m_system = system;
}

DynamicSystemRenderable::~DynamicSystemRenderable()
{}

void DynamicSystemRenderable::do_draw()
{}

  void DynamicSystemRenderable::do_animate(float time)
  {
    m_system->computeSimulationStep();
    m_lastUpdateTime = time;
  }

void DynamicSystemRenderable::setDynamicSystem(const DynamicSystemPtr &system)
{
    m_system = system;
}

void DynamicSystemRenderable::do_keyPressedEvent(sf::Event &e)
{
  //Propagate events to the children
  for (HierarchicalRenderablePtr c : getChildren()) {
    c->keyPressedEvent(e);
  }
}

void DynamicSystemRenderable::do_keyReleasedEvent(sf::Event& e)
{
    //Propagate events to the children
    for (HierarchicalRenderablePtr c : getChildren()) {
        c->keyReleasedEvent(e);
    }
}
