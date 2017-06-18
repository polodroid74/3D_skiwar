#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/texturing/TexturedLakeRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderablePhysic.hpp"

#include "../include/renderable/ArbreLegerRenderable.hpp"
#include "../include/renderable/SnowRenderable.hpp"
#include "../include/keyframes/KeyframedConeRenderable.hpp"
#include "../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../include/keyframes/GeometricTransformation.hpp"
#include "../include/keyframes/KeyframedMeshRenderable.hpp"

#include "../include/texturing/TexturedLakeRenderable.hpp"
#include "../include/texturing/TexturedIglooRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"

#include "../include/lighting/SpotLightRenderable.hpp"
#include "../include/lighting/PointLightRenderable.hpp"

#include "./../include/ground/IndexedGroundRenderable.hpp"
#include "./../include/SkyBox.hpp"
#include "./../include/texturing/TexturedGroundRenderable.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/ground/NoiseProvider.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>

//-------Physique---------------------
#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"

#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/dynamics_rendering/ParticleRenderable.hpp"
#include "../include/dynamics_rendering/ParticleListRenderable.hpp"
#include "../include/dynamics_rendering/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringListRenderable.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/QuadRenderable.hpp"


// Génération d'une foret aléatoire, 
// on définit une grille et on place les arbres dedans
// de sorte qu'ils n'entrent pas en collision
void scene_foret(Viewer& viewer)
{
    std::cout << "\n Génération d'un forêt \n ";
    srand(time(NULL));
    // Shaders
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../shaders/flatVertex.glsl", "../shaders/flatFragment.glsl");
    
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
    
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(
        "../shaders/textureVertex.glsl", "../shaders/textureFragment.glsl");

    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(phongShader);
    viewer.addShaderProgram(texShader);
    
    // LIGHTS
    glm::vec3 lightDirection = glm::normalize(glm::vec3(-0.0, -1.0, -1.0));
    glm::vec3 ghostWhite(248.0/255, 248.0/255, 1.0);
    DirectionalLightPtr directionalLight =
        std::make_shared<DirectionalLight>(lightDirection, ghostWhite, ghostWhite, ghostWhite);
    viewer.setDirectionalLight(directionalLight);
    
    // FORET
    unsigned int taille_case = 4; 
    unsigned int lon = 1;
    unsigned int ecart = ceil((taille_case - lon)/2);
    unsigned int marge = taille_case - 2*ecart;
    
    unsigned int nb = 10; 
    for (unsigned int i = 1; i <= nb; i ++) {
        for (unsigned int j = 1; j <= nb; j++) {
            float l = 2.4 + ((float)(rand()%3) / 10.0);
            glm::vec3 origine = glm::vec3(i*taille_case+ecart + rand()%marge, j*taille_case + ecart + rand()%marge, 0.0);
            auto arbre = std::make_shared<ArbreLegerRenderable>(phongShader, 
                    flatShader, viewer, l, origine);
        }
    }
    
    // LAC
    std::string filename = "../textures/snow.jpg";    
    auto lake = std::make_shared<TexturedLakeRenderable>(texShader, filename);
    auto parentTransformation = glm::scale(glm::mat4(1.5), glm::vec3(3.0,5.0,1.0));
    parentTransformation = glm::translate(parentTransformation, glm::vec3( 1.0, - 1.0, 0.02));
    lake->setParentTransform(parentTransformation);    
    lake->setMaterial(Material::Water()); 
    viewer.addRenderable(lake);
    
    // IGLOO
    filename = "../textures/briques.jpg";    
    auto igloo = std::make_shared<TexturedIglooRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( -1.0, - 2.0, 0.0));
    igloo->setParentTransform(parentTransformation);    
    igloo->setMaterial(Material::Snow()); 
    viewer.addRenderable(igloo);
    
    //Textured plane
    filename = "../textures/snow.jpg";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(50.0,50.0,50.0));
    texPlane->setParentTransform(parentTransformation);
    texPlane->setMaterial(Material::Snow());
    viewer.addRenderable(texPlane);

    int tmax = 14;
    // Neige qui tombe
    SnowRenderable(phongShader, flatShader, viewer, tmax);

    
    // SKYBOX
    filename = "../textures/hoth5.jpg";
    float hauteur = 20.0;
    TexturedCubeRenderablePtr texCube= std::make_shared<TexturedCubeRenderable>(texShader, filename);
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, hauteur/2.0 - 0.01));
    parentTransformation = glm::scale(parentTransformation, glm::vec3(50.0,50.0, hauteur));
    
    texCube->setParentTransform(parentTransformation);
    texCube->setMaterial(Material::Snow());
    viewer.addRenderable(texCube);
        
    
    
    auto px=glm::vec3(10.0, 20.0, 0.0);
    parentTransformation=glm::mat4(1.0);
    auto localTransformation = glm::translate( glm::mat4(1.0), px);
    localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
    localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));
    auto ping = std::make_shared<KeyframedMeshRenderable>(
                            texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                            4.0, px, false);
    
    ping->setMaterial(Material::Pearl());
    ping->setParentTransform( parentTransformation );
    ping->setLocalTransform( localTransformation );
    
    int duree_saut = tmax/2 ;
    float pas = 2*M_PI / duree_saut;
  
        int x = 5;
        int y = 10;
    for (int t = 1; t <= tmax; t++) {
        ping->addParentTransformKeyframe(t-1, 
                GeometricTransformation(glm::vec3(hauteur*cos(pas*t)+x, hauteur*sin(pas*t)+y, (t+1)%2), 
                glm::quat(glm::vec3(M_PI_2,0.0, pas*t - M_PI_4)))); 
    }
    ping->addParentTransformKeyframe(tmax, 
        GeometricTransformation(glm::vec3(hauteur*cos(pas*tmax), hauteur*sin(pas*tmax), 0), 
        glm::quat(glm::vec3(M_PI_2,0.0, pas*tmax - M_PI_4)))); 
    

    viewer.addRenderable(ping);
    
    // Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -8, 7), glm::vec3(0, 0, 4), glm::vec3(0, 0, 1)) );
    
    
    viewer.setAnimationLoop(true, 7);
    viewer.startAnimation();
}
