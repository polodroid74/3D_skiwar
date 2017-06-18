#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/renderable/ForetRenderable.hpp"
#include "../include/renderable/ArbreLegerRenderable.hpp"
#include <iostream>
#include <cstdlib>
//Don't forget the include
#include "./../include/ground/IndexedGroundRenderable.hpp"
#include "./../include/SkyBox.hpp"
#include "./../include/texturing/TexturedGroundRenderable.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/ground/NoiseProvider.hpp"
#include "../include/renderable/SnowRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderablePhysic.hpp"
#include "../include/texturing/TexturedIglooRenderable.hpp"
#include "../include/keyframes/KeyframedMeshRenderable.hpp"
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


void speeder(Viewer& viewer)
{

                int tmax = 15; // durée de la boucle d'animation


	//Add the renderable to the Viewer
	std::string vShader = "./../shaders/flatVertex.glsl";
	std::string fShader = "./../shaders/flatFragment.glsl";
	ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader,fShader);
	viewer.addShaderProgram(flatShader);

	ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl", "../shaders/textureFragment.glsl");
	viewer.addShaderProgram(texShader);


	ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
			"../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
	viewer.addShaderProgram(phongShader);

	glm::mat4 localTransformation(1.0);
	// LIGHT
	glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0, -1.0, -1.0));
	glm::vec3 ghostWhite(248.0/255, 248.0/255, 1.0);
	DirectionalLightPtr directionalLight =
		std::make_shared<DirectionalLight>(lightDirection, ghostWhite, ghostWhite, ghostWhite);
	viewer.setDirectionalLight(directionalLight);

	Height heightMap = Height(300,300,300.0,300.0,0); //nb carreau x,y ; taille x,y
	NoiseProvider::randomizeHMap(&heightMap);
	//SOL
	TexturedGroundRenderablePtr tgnd = std::make_shared<TexturedGroundRenderable>(heightMap, texShader, "../textures/snow.jpg");
	tgnd->setMaterial(Material::Snow());
	viewer.addRenderable(tgnd);

	//FORET
	tgnd->setModelMatrix( glm::mat4(1.0));//model );
	std::vector<HierarchicalRenderablePtr> meshes; //physique
	unsigned int taille_case=20;
	unsigned int taille_ss_case=15;
	unsigned int ecart = ceil((taille_case - taille_ss_case)/2);
	unsigned int marge = taille_case - 2*ecart;
	unsigned int nb_sapins = 7;
	glm::vec3 origine;
	float l;
	float Px;
	float Py;
	for (unsigned int i = 1; i <= nb_sapins; i ++) {
			for (unsigned int j = 1; j <= nb_sapins; j++) {
					l = 10.0 + ((float)(rand()%3) / 10.0);
					Px = i*taille_case+ecart + rand()%marge;
					Py = j*taille_case + ecart + rand()%marge;
					origine = glm::vec3(Px, Py, heightMap(Px, Py));
					meshes.push_back(std::make_shared<ArbreLegerRenderable>(phongShader,
									flatShader, viewer, l, origine));
			}
	}
	//Temporary variables
	MaterialPtr pearl = Material::Pearl();

	//NEIGE
	SnowRenderable(phongShader, flatShader, viewer,10);
	//SKYBOX
	auto filename = "../textures/sky.jpg";
	SkyBoxPtr skybox= std::make_shared<SkyBox>(texShader, filename);
	auto parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(170.0, 147.0, -30.0));
	parentTransformation = glm::scale(parentTransformation, glm::vec3(200.0,200.0,100 ));
	skybox->setParentTransform(parentTransformation);
	skybox->setMaterial(Material::Snow());
	viewer.addRenderable(skybox);
	meshes.push_back(skybox);
  // Lac
  filename = "../textures/lake.png";
  TexturedPlaneRenderablePtr texCube= std::make_shared<TexturedPlaneRenderable>(texShader, filename);
  parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(100.0, 100.0,  -4.0));
  parentTransformation = glm::scale(parentTransformation, glm::vec3(500.0,500.0, 0.0));
  texCube->setParentTransform(parentTransformation);
  texCube->setMaterial(Material::Snow());
  viewer.addRenderable(texCube);


	// IGLOO
	filename = "../textures/briques.jpg";
	auto igloo = std::make_shared<TexturedIglooRenderable>(texShader, filename);
	parentTransformation = glm::scale(glm::mat4(10.0), glm::vec3(1000.0,1000.0,1000.0));
	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( 150.0, 120.0, heightMap(150,120)));
	igloo->setParentTransform(parentTransformation);
	igloo->setMaterial(Material::Snow());
	viewer.addRenderable(igloo);
	meshes.push_back(igloo);

	// tour en bois
auto px=glm::vec3(200.0, 200.0, 0.0);
parentTransformation=glm::mat4(1.0);
localTransformation = glm::translate( glm::mat4(1.0), px);
localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
localTransformation = glm::scale( localTransformation, glm::vec3(1,1,1.0));
TexturedMeshRenderablePtr tour =
std::make_shared<TexturedMeshRenderable>(
	texShader, "../meshes/castle.obj", "../textures/bois.png",
	2.0,px, false);
	tour->setMaterial(pearl);
tour->setLocalTransform( localTransformation );
tour->setParentTransform( parentTransformation );
	viewer.addRenderable(tour);
meshes.push_back(tour);

	// tour médiévale
	px=glm::vec3(200.0, 100.0, 0.0);
parentTransformation=glm::mat4(1.0);
localTransformation = glm::translate( glm::mat4(1.0), px);
localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
localTransformation = glm::scale( localTransformation, glm::vec3(0.9,0.9,0.9));
TexturedMeshRenderablePtr medieval =
std::make_shared<TexturedMeshRenderable>(
	texShader, "../meshes/Medieval.obj", "../textures/bois.png",
	2.0,px, false);
	medieval->setMaterial(pearl);
medieval->setLocalTransform( localTransformation );
medieval->setParentTransform( parentTransformation );
	viewer.addRenderable(medieval);
	meshes.push_back(medieval);


        // Ajout d'un pingouin qui saute
        px=glm::vec3(170.0, 200.0, heightMap(170.0,200.0) + 10);
        parentTransformation = glm::translate( glm::mat4(1.0), px);
        parentTransformation = glm::rotate( parentTransformation, float(M_PI_2), glm::vec3(1,0,0));
        localTransformation = glm::translate( glm::mat4(1.0), px);
        localTransformation = glm::rotate( localTransformation, float(M_PI), glm::vec3(0.0,1.0,0.0));
        localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));
        auto ping = std::make_shared<KeyframedMeshRenderable>(
                                texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                                4.0, px, false);

        ping->setMaterial(Material::Pearl());
        ping->setParentTransform( parentTransformation );
        ping->setLocalTransform( localTransformation );

        int duree_saut = tmax -1;
        float ray = 5.0;
        float pas = 2*M_PI / duree_saut;
        for (int t = 1; t <= tmax; t++) {
            ping->addParentTransformKeyframe(t-1,
                    GeometricTransformation(glm::vec3(ray*cos(pas*t), ray*sin(pas*t), (t+1)%2),
                    glm::quat(glm::vec3(M_PI_2,0.0, pas*t - M_PI_4))));
        }
        ping->addParentTransformKeyframe(tmax,
            GeometricTransformation(glm::vec3(ray*cos(pas*tmax), ray*sin(pas*tmax), 0),
            glm::quat(glm::vec3(M_PI_2,0.0, pas*tmax - M_PI_4))));

        viewer.addRenderable(ping);
	//SPEEDER 1
	px = glm::vec3(5, 20, 2.0);
	glm::vec3 pv = glm::vec3(0, 0, 0);
	float pr = 3.5;
	float pm = 7.0;
	TexturedMeshRenderablePhysicPtr speeder =
		std::make_shared<TexturedMeshRenderablePhysic>(
				texShader, "../meshes/SnowSpeeder.obj", "../textures/speeder.png",
				px,pv,pm,pr,true,
				&viewer, heightMap, skybox);
	speeder->setMaterial(pearl);
	//SPEEDER 2
	px=glm::vec3(200.0, 150.0, 2.0);
	TexturedMeshRenderablePhysicPtr speeder2 =
		std::make_shared<TexturedMeshRenderablePhysic>(
				texShader, "../meshes/SnowSpeeder.obj", "../textures/speeder.png",
				px,pv,pm,pr,false,
				&viewer, heightMap, skybox);
	speeder2->setMaterial(pearl);
	viewer.addRenderable(speeder2);

	//-----------------------Physique-------------------------------

	meshes.push_back(speeder);
	meshes.push_back(speeder2);

	//Initialize a dynamic system (Solver, Time step, Restitution coefficient)
	DynamicSystemPtr system = std::make_shared<DynamicSystem>();
	system->setmeshes(meshes);
	EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
	system->setSolver(solver);
	system->setDt(0.01);
	DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
	viewer.addRenderable(systemRenderable);

	//speeder->setForce(glm::vec3(0,0,0.01));
	glm::vec3 Force(0.0, 0.0, 0.0);
	std::vector<TexturedMeshRenderablePhysicPtr> vSpeeder;
	std::vector<TexturedMeshRenderablePhysicPtr> vSpeeder2;
	vSpeeder.push_back(speeder);
	vSpeeder2.push_back(speeder2);
	ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vSpeeder, Force);
	system->addForceField(force);
	ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(flatShader, force);
	HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
	speeder->TexturedMeshRenderablePhysic::setAngle(forceRenderable-> ControlledForceFieldRenderable::getAddrAngle());
	float nullangle=0;
	speeder2->TexturedMeshRenderablePhysic::setAngle(&nullangle);
	//std::cout << speeder<<'\n';
	system->addTexturedMeshRenderablePhysic(speeder);
	system->addTexturedMeshRenderablePhysic(speeder2);
	HierarchicalRenderable::addChild(systemRenderable, speeder);
	DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vSpeeder, 0.7);
	DampingForceFieldPtr dampingForceField2 = std::make_shared<DampingForceField>(vSpeeder2, 0.7);
	system->addForceField(dampingForceField2);
	system->addForceField(dampingForceField);
	viewer.setAnimationLoop(true, tmax);
	viewer.startAnimation();
}
