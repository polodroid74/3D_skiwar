#include "../../include/renderable/ArbreLegerRenderable.hpp"
#include "../../include/keyframes/KeyframedEpineRenderable.hpp"
#include "../../include/keyframes/KeyframedConeRenderable.hpp"
#include "../../include/Utils.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include <ctime>
#include <cstdlib>

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


ArbreLegerRenderable::ArbreLegerRenderable(ShaderProgramPtr shader, ShaderProgramPtr flatShader,
        Viewer & viewer, float l, glm::vec3 origin) :
                HierarchicalRenderable(shader)
{
    this->translation=origin;
    this->m_radius=l/2.0;
    auto m_root = std::make_shared<KeyframedConeRenderable>(shader);
    m_root->setLocalTransform( GeometricTransformation( glm::vec3{0.0,0.0,0.0}, glm::quat(), glm::vec3{l/15, l/15,l}).toMatrix() );
    m_root->addParentTransformKeyframe(0.0, GeometricTransformation(origin));

    MaterialPtr mat = Material::Bronze();
    m_root->setMaterial(mat);

    int nb_branches_tronc =  4 + rand() % 3; // attention il y aura en fait une de moins de dessinée
    float hauteur_br = l / nb_branches_tronc;

    for (int i = 1; i < nb_branches_tronc; i ++) {
        int nb = 4 + rand() % 2;
        int angle_moy = 360 / nb;

        for (int j = 0; j < nb; j ++) {
            int alpha_h = angle_moy*j + rand() % 30;
            int alpha_v = 95 + rand()%10;
            auto son = std::make_shared<KeyframedConeRenderable>(shader);
            son->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,hauteur_br*i},
                                        glm::quat()) );
            son->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{},
                                        glm::quat(glm::vec3{0,alpha_v*2*M_PI/360,alpha_h*2*M_PI/360}),
                                        glm::vec3{(nb_branches_tronc - i +1)*l/180, (nb_branches_tronc - i +1)*l/180, (nb_branches_tronc - i +1)*l/9}) );

            MaterialPtr mat = Material::Emerald();
            son->setMaterial(mat);

            HierarchicalRenderable::addChild(m_root, son);
        }
    }
    viewer.addRenderable(m_root);
}


void ArbreLegerRenderable::do_draw() {}

void ArbreLegerRenderable::do_animate(float time) {}

ArbreLegerRenderable::~ArbreLegerRenderable()
{
}
//-----------------------------------Collisions-------------------------

//----------------------collisions-----------------------------
float ArbreLegerRenderable::getRadius() const
{
  return m_radius;
}
void ArbreLegerRenderable::setRadius(const float& radius){
  this->m_radius=radius;
}
collisionSphere* ArbreLegerRenderable::computeCollisionSphere(){
  collisionSphere *cs = new collisionSphere(this->translation,this->getRadius());
  return cs;
}

const glm::vec3& ArbreLegerRenderable::getPosition() const
{
  return this->translation;
}
bool ArbreLegerRenderable::isFixed() const
{
  return true;
}
