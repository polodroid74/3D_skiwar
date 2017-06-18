#include "../../include/renderable/TreeRenderable.hpp"
#include "../../include/keyframes/KeyframedEpineRenderable.hpp"
#include "../../include/Utils.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../include/keyframes/KeyframedCylinderRenderable.hpp"
#include <ctime>
#include <cstdlib>

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

// la taille des feuilles est fixe
void TreeRenderable::branche(ShaderProgramPtr shader, ShaderProgramPtr flatShader, HierarchicalRenderablePtr father,
        float l, int alpha_h, int alpha_v, float rayon) {
    
    auto son = std::make_shared<KeyframedEpineRenderable>(flatShader);
    // translation // rotation // scale
    son->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,(5+rand()%5)/10.0f*l},
            glm::quat( glm::vec3(0, 0.0, 0.0)) ));

    son->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,0},
            glm::quat(glm::vec3{0, alpha_h*2*M_PI/360, alpha_v*2*M_PI/360}),
                    glm::vec3{rayon*1.5,rayon*1.5,rayon*10}) );

    HierarchicalRenderable::addChild(father, son);

}

void TreeRenderable::branche_principale(ShaderProgramPtr shader, ShaderProgramPtr flatShader, 
        HierarchicalRenderablePtr father,
        float longTronc, int alpha_h, int alpha_v, int compt, float rayon) {
    compt --;

    if(compt <= 0) {return;}
    auto son = std::make_shared<KeyframedCylinderRenderable>(shader);

    son->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,longTronc},
            glm::quat(glm::vec3{0, alpha_v*2*M_PI/360, alpha_h*2*M_PI/360}) ));

    son->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0, 0, 0},
            glm::quat( glm::vec3(0, 0.0, 0.0)),
            glm::vec3{rayon, rayon, longTronc}) );

    MaterialPtr mat = Material::Bronze();
    son->setMaterial(mat);

    HierarchicalRenderable::addChild(father, son);

    int epsilon = 10 - rand()%5;
    // on crée la branche principale qui suit une direction proche
    branche_principale(shader, flatShader, son, 0.981*longTronc,  epsilon, 5 - (rand()%4), compt, rayon * 0.981);
// alpha_h + epsilon
    int angle = rand()%50;
    
    for (int i = 0; i < longTronc*5; i ++) {
        branche(shader, flatShader, son, (rand()%10) / 10.0f *longTronc, 100, 360 -30 -(rand()%30), rayon);
        branche(shader, flatShader, son, (rand()%10) / 10.0f *longTronc, 100, 360 +30 +(rand()%30), rayon);    
    //branche(shader, son, (rand()%10) / 10.0f *longTronc, 100, 360 -30 -(rand()%30), rayon);
    //branche(shader, son, (rand()%10) / 10.0f *longTronc, 100, 360 +30 +(rand()%50), rayon);
    }

}

/*  fonction de création d'un bout de tronc (cylindre)
 *      a correspond à la taille atteinte par l'arbre actuellement
 *      l est sa taille adulte
 *      alpha h son angle dans le plan parallèle au sol
 *      alpha v son angle dans le plan perpendiculaire au sol
 */
void TreeRenderable::tronc(const ShaderProgramPtr& shader, ShaderProgramPtr flatShader, 
        HierarchicalRenderablePtr father, int a,
        float l, int alpha_h, int alpha_v, float r) {
    if(a <= 1) {return;}

    auto son = std::make_shared<KeyframedCylinderRenderable>(shader);
    son->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,l},
                                    glm::quat(glm::vec3{0,alpha_v*2*M_PI/360,alpha_h*2*M_PI/360})) );
    son->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{},
                                    glm::quat(),
                                    glm::vec3{r, r, l}) );

    MaterialPtr mat = Material::Bronze();
    son->setMaterial(mat);

    HierarchicalRenderable::addChild(father, son);

    int angle_tronc_h = 3 - rand() % 6;
    int angle_tronc_v = 2 - rand() % 5;
    tronc(shader, flatShader, son, a - 1, l*0.9, angle_tronc_h, angle_tronc_v, r*0.9);

    int nb_branches_tronc = 5 + rand() % 2;
    int angle_moy = 360 / (nb_branches_tronc);
    float rayon_branche = l/20;
    int longueur_branche_min = 3 + rand()%2;

    for (int i = 0; i < nb_branches_tronc; i ++) { //+ rand()%(angle_moy)/3
        branche_principale(shader, flatShader, son, (0.2*l) * 2, angle_moy*i + rand()%(angle_moy)/3,
                90 + rand()%25, (longueur_branche_min + a)/2, rayon_branche);
    }
}

TreeRenderable::TreeRenderable(ShaderProgramPtr shader, ShaderProgramPtr flatShader, 
        Viewer & viewer, float l, float rayon, int height, glm::vec3 origin) :
                HierarchicalRenderable(shader)
{
    srand(time(NULL));
     // Create the root - the first cylinder composing the tree
    m_root = std::make_shared<KeyframedCylinderRenderable>(shader);
    m_root->setLocalTransform( GeometricTransformation( glm::vec3{0.0,0.0,0.0}, glm::quat(), glm::vec3{rayon, rayon,l}).toMatrix() );
    m_root->addParentTransformKeyframe(0.0, GeometricTransformation(origin)); // glm::vec3{10.0,1.0,0}) );
    
    MaterialPtr mat = Material::Bronze();
    m_root->setMaterial(mat);
    
    std::cout << "\n Création d'un arbre  \n";
    // Create the rest of the branches
    tronc(shader, flatShader, m_root, height, l, 8 - (rand()%4), 8 - (rand()%4), rayon);
    
    
    
    //auto parentTrans = glm::translate( glm::mat4(1.0), glm::vec3(1, 1, 1));
    //m_root->setLocalTransform( parentTrans );
    
    viewer.addRenderable(m_root);
}



void TreeRenderable::do_draw() {}

void TreeRenderable::do_animate(float time) {}

TreeRenderable::~TreeRenderable() 
{
}