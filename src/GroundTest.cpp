#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"


#include <iostream>
//Don't forget the include
#include "./../include/ground/IndexedGroundRenderable.hpp"
#include "./../include/texturing/TexturedGroundRenderable.hpp"
#include "../include/ground/NoiseProvider.hpp"
//...

void init_ground(Viewer& viewer)
{
	//...

	//Instanciate a CubeRenderable while specifying its shader program

	//Add the renderable to the Viewer
	std::string vShader = "./../shaders/flatVertex.glsl";
	std::string fShader = "./../shaders/flatFragment.glsl";
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader,fShader);
	viewer.addShaderProgram(flatShader);

	ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl", "../shaders/textureFragment.glsl");
	viewer.addShaderProgram(texShader);

	glm::mat4 localTransformation(1.0);
	//Define a directional light for the whole scene
	glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
	glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.8), d_specular(1.0,1.0,1.0);
	DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
	//Add a renderable to display the light and control it via mouse/key event
	glm::vec3 lightPosition(0.0,0.0,5.0);
	DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
	localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
	directionalLightRenderable->setLocalTransform(localTransformation);
	viewer.setDirectionalLight(directionalLight);
	viewer.addRenderable(directionalLightRenderable);

	Height heightMap = Height(1000,1000,1000.0,1000.0,0); //nb carreau x,y ; taille x,y
	NoiseProvider::randomizeHMap(&heightMap);

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);
	IndexedGroundRenderablePtr ignd = std::make_shared<IndexedGroundRenderable>(flatShader);
	TexturedGroundRenderablePtr tgnd = std::make_shared<TexturedGroundRenderable>(heightMap, texShader, "../textures/snow2.jpg");

	//tgnd->setMaterial(Material::Pearl());
	tgnd->setMaterial(Material::Snow());

	//	viewer.addRenderable(ignd);
	viewer.addRenderable(tgnd);

	glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(-1.0, 0.0, 0.0));
	model = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.0, 0.0));
	ignd->setModelMatrix( model );
	model = glm::translate(glm::mat4(1.0), glm::vec3(-100.0, -100.0, 0.0));
	tgnd->setModelMatrix( model );
}

