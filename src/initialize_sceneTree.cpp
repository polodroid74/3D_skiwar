#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/texturing/TexturedLakeRenderable.hpp"
#include "../include/texturing/TexturedIglooRenderable.hpp"

#include "../include/renderable/TreeRenderable.hpp"
#include "../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../include/keyframes/GeometricTransformation.hpp"

#include <iostream>
#include <iomanip>
#include <ctime>

void initialize_sceneTree(Viewer& viewer)
{
    srand(time(NULL));
    // Shaders
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../shaders/flatVertex.glsl", "../shaders/flatFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(phongShader);
    
    ShaderProgramPtr texShader
    = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                      "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    // Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(phongShader);
    viewer.addRenderable(frame);

    // Define a directional light for the whole scene
    glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0, -1.0, -1.0));
    glm::vec3 ghostWhite(248.0/255, 248.0/255, 1.0);
    DirectionalLightPtr directionalLight =
        std::make_shared<DirectionalLight>(lightDirection, ghostWhite, ghostWhite, ghostWhite);
    viewer.setDirectionalLight(directionalLight);
    // Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0, 5.0, 8.0);
    DirectionalLightRenderablePtr directionalLightRenderable
        = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    glm::mat4 localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.addRenderable(directionalLightRenderable);

    float l = 0.9;
    auto arbre = std::make_shared<TreeRenderable>(phongShader, 
                    flatShader, viewer, l, l/10, 12, glm::vec3(10.0, 2.0, 0.0));

    l = 0.8;
    auto arbre2 = std::make_shared<TreeRenderable>(phongShader, 
                    flatShader, viewer, l, l/10, 15, glm::vec3(7.0, 3.0, 0.0));
    
    std::string filename = "../textures/briques.jpg";
    auto lake = std::make_shared<TexturedLakeRenderable>(texShader, filename);
    auto parentTransformation = glm::scale(glm::mat4(0.80), glm::vec3(15.0,10.0,10.0));
    parentTransformation = glm::translate(parentTransformation, glm::vec3( - 1.0, - 1.0, 0.0));
    lake->setParentTransform(parentTransformation);    
    lake->setMaterial(Material::Water()); 
    viewer.addRenderable(lake);
    
        
    auto igloo = std::make_shared<TexturedIglooRenderable>(texShader, filename);
    //parentTransformation = glm::scale(glm::mat4(0.80), glm::vec3(5.0,4.0,10.0));
    parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( - 5.0, - 4.0, 0.0));
    
    igloo->setParentTransform(parentTransformation);    
    igloo->setMaterial(Material::Snow()); 
    viewer.addRenderable(igloo);
    

    // Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -8, 7), glm::vec3(0, 0, 4), glm::vec3(0, 0, 1)) );

    viewer.setAnimationLoop(true, 6.0);
    viewer.startAnimation();
}
