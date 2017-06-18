#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/SkyBox.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/renderable/ArbreLegerRenderable.hpp"
#include "../include/renderable/ForetRenderable.hpp"
#include "../include/renderable/TreeRenderable.hpp"
#include "../include/ground/IndexedGroundRenderable.hpp"
#include "../include/ground/NoiseProvider.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/keyframes/KeyframedMeshRenderable.hpp"
#include "../include/renderable/SnowRenderable.hpp"
#include "../include/texturing/TexturedGroundRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/texturing/TexturedCubeRenderable.hpp"
#include "../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "../include/texturing/MipMapCubeRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderable.hpp"
#include "../include/texturing/TexturedMeshRenderablePhysic.hpp"
#include "../include/texturing/TexturedIglooRenderable.hpp"

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

#include <iostream>
#include <cmath>

void ajoute_pingouin(ShaderProgramPtr texShader, Height heightMap, Viewer & viewer, float x, float y, int angle, int tmax);

void global(Viewer& viewer)
{
	// Création des shaders
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

	FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
	viewer.addRenderable(frame);
	TexturedGroundRenderablePtr tgnd = std::make_shared<TexturedGroundRenderable>(heightMap, texShader, "../textures/snow.jpg");
	tgnd->setMaterial(Material::Snow());
	viewer.addRenderable(tgnd);
	tgnd->setModelMatrix( glm::mat4(1.0));
	//Temporary variables
	MaterialPtr pearl = Material::Pearl();
	std::vector<HierarchicalRenderablePtr> meshes;

	//FORET
	unsigned int taille_case=30;
	unsigned int taille_ss_case=17;
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
	// SKYBOX
	auto filename = "../textures/sky.jpg";
	SkyBoxPtr skybox= std::make_shared<SkyBox>(texShader, filename);
	auto parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(100.0, 100.0, -50.0));
	parentTransformation = glm::scale(parentTransformation, glm::vec3(70.0,70.0,100 ));

	skybox->setParentTransform(parentTransformation);
	skybox->setMaterial(Material::Snow());
	viewer.addRenderable(skybox);

	// LAC
	filename = "../textures/lake.png";
	TexturedPlaneRenderablePtr texCube= std::make_shared<TexturedPlaneRenderable>(texShader, filename);
	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(100.0, 100.0,  -4.0));
	parentTransformation = glm::scale(parentTransformation, glm::vec3(1000.0,1000.0, 0.0));

	texCube->setParentTransform(parentTransformation);
	texCube->setMaterial(Material::Snow());
	viewer.addRenderable(texCube);


	// IGLOO
	filename = "../textures/briques.jpg";
	auto igloo = std::make_shared<TexturedIglooRenderable>(texShader, filename);
	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( 140.0, 260.0, heightMap(140,260)));
	parentTransformation = glm::scale(parentTransformation, glm::vec3(2, 2, 2));
	igloo->setParentTransform(parentTransformation);
	igloo->setMaterial(Material::Snow());
	viewer.addRenderable(igloo);

	//SNOWMAN
	glm::vec3 px=glm::vec3(130.0, 260.0, heightMap(130,260));
	TexturedMeshRenderablePtr snowMan =
		std::make_shared<TexturedMeshRenderable>(
				texShader, "../meshes/SnowMan.obj", "../textures/SnowMan.png",
				4.0,px, true);
	snowMan->setMaterial(Material::Snow());
	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( 140.0, 270.0, heightMap(140,270)));
	snowMan->setParentTransform( parentTransformation );
	viewer.addRenderable(snowMan);

	//SNOWMANHAT
	TexturedMeshRenderablePtr snowManHat =
		std::make_shared<TexturedMeshRenderable>(
				texShader, "../meshes/hat.obj", "../textures/hat.jpg",
				4.0,px, true);
	snowManHat->setMaterial(Material::Snow());
	parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3( 140.0, 270.0, heightMap(140,270)+5));
	parentTransformation = glm::rotate(parentTransformation,2.0f,glm::vec3(0.0,1.0,0.0));
	parentTransformation = glm::scale(parentTransformation, glm::vec3( 0.8, 0.8, 1.0));
	snowManHat->setParentTransform( parentTransformation );
	viewer.addRenderable(snowManHat);
	meshes.push_back(snowManHat);

	// TOUR EN BOIS
	px=glm::vec3(185.0, 200.0, heightMap(185,200)-0.5);
	parentTransformation=glm::mat4(1.0);
	localTransformation = glm::translate( glm::mat4(1.0), px);
	localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
	localTransformation = glm::scale( localTransformation, glm::vec3(1,1,1.0));
	TexturedMeshRenderablePtr tour =
	std::make_shared<TexturedMeshRenderable>(
		texShader, "../meshes/castle.obj", "../textures/bois.png",
		4.0,px, false);
		tour->setMaterial(pearl);
		tour->setLocalTransform( localTransformation );
		tour->setParentTransform( parentTransformation );
		viewer.addRenderable(tour);
		meshes.push_back(tour);

		// TOUR MEDIEVALE
		px=glm::vec3(150.0, 250.0, heightMap(150,250));
		parentTransformation=glm::mat4(1.0);
		localTransformation = glm::translate( glm::mat4(1.0), px);
		localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
		localTransformation = glm::scale( localTransformation, glm::vec3(0.4,0.4,0.4));
		TexturedMeshRenderablePtr medieval =
		std::make_shared<TexturedMeshRenderable>(
			texShader, "../meshes/Medieval.obj", "../textures/bois.png",
			2.0,px, false);
			medieval->setMaterial(pearl);
			medieval->setLocalTransform( localTransformation );
			medieval->setParentTransform( parentTransformation );
			viewer.addRenderable(medieval);
			meshes.push_back(medieval);

			//NEIGE
			SnowRenderable(phongShader, flatShader, viewer,10);

	//SPEEDER 1
	px = glm::vec3(50, 50, 3.0);
	glm::vec3 pv = glm::vec3(0, 0, 0);
	float pr = 3.5;
	float pm = 12.0;
	TexturedMeshRenderablePhysicPtr speeder =
		std::make_shared<TexturedMeshRenderablePhysic>(
				texShader, "../meshes/SnowSpeeder.obj", "../textures/speeder.png",
				px,pv,pm,pr,true,
				&viewer, heightMap, skybox);
	speeder->setMaterial(pearl);

	//SPEEDER 2
	px=glm::vec3(190.0, 205.0, 2.0);
	TexturedMeshRenderablePhysicPtr speeder2 =
		std::make_shared<TexturedMeshRenderablePhysic>(
				texShader, "../meshes/SnowSpeeder.obj", "../textures/speeder.png",
				px,pv,pm,pr,false,
				&viewer, heightMap,skybox);
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
	system->addTexturedMeshRenderablePhysic(speeder);
	system->addTexturedMeshRenderablePhysic(speeder2);
	HierarchicalRenderable::addChild(systemRenderable, speeder);
	DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vSpeeder, 0.7);
	DampingForceFieldPtr dampingForceField2 = std::make_shared<DampingForceField>(vSpeeder2, 0.7);
	system->addForceField(dampingForceField2);
	system->addForceField(dampingForceField);
//--------------------------------------------------------------------------------
        int tmax = 14; // durée de la boucle d'animation

        // Cercle de pingouins :
        int ray = 5;
        int centerX = 150;
        int centerY = 120;
        int nbPing = 10;
        float angle = M_PI*2/nbPing;
        for (float p = 0; p < nbPing; p ++) {
            ajoute_pingouin(texShader, heightMap, viewer, centerX + ray*cos(p*angle), centerY + ray*sin(p*angle), M_PI + p*angle, tmax);
        }

        px=glm::vec3(centerX, centerY, heightMap(centerX, centerY)+1); // parce que j'ai pas mis le bas du corps à zero
        parentTransformation=glm::mat4(1.0);
        localTransformation = glm::translate( glm::mat4(1.0), px);
        localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
        localTransformation = glm::rotate( localTransformation, float(angle), glm::vec3(0,1,0));
        localTransformation = glm::scale( localTransformation, glm::vec3(0.4,0.4,0.4));

        // Grand gourou des pingouins :
        auto pong = std::make_shared<KeyframedMeshRenderable>(
                                texShader, "../meshes/pingouin_trop_choupinou2.obj", "../textures/pingouin_trop_choupi.png",
                                4.0, px, false);

        pong->setMaterial(Material::Pearl());
        pong->setParentTransform( parentTransformation );
        pong->setLocalTransform( localTransformation );
        viewer.addRenderable(pong);


        // Position the camera
        viewer.getCamera().setViewMatrix(
            glm::lookAt(glm::vec3(0, -8, 7), glm::vec3(0, 0, 4), glm::vec3(0, 0, 1)) );


        // Ajout du pingouin qui tourne en rond
        px=glm::vec3(0, 0, 0);
        parentTransformation=glm::mat4(1.0);
        localTransformation = glm::translate( glm::mat4(1.0), px);
        localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));
        auto ping = std::make_shared<KeyframedMeshRenderable>(
                                texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                                4.0, px, false);

        ping->setMaterial(Material::Pearl());
        ping->setParentTransform( parentTransformation );
        ping->setLocalTransform( localTransformation );

        int x = 100;
        int y = 100;
        int hauteur = 5;
        int duree_saut = tmax;
        float pas = 2*M_PI / duree_saut;
        for (int t = 0; t <= tmax; t++) {
            ping->addParentTransformKeyframe(t,
                    GeometricTransformation(glm::vec3(hauteur*cos(pas*t)+x, hauteur*sin(pas*t)+y, t%2),
                    glm::quat(glm::vec3(M_PI_2,0.0, 2*pas*t + M_PI_4))));
        }
        viewer.addRenderable(ping);

        // Groupe de pingouins qui regardent l'horizon
        int centreX = 295;
        int centreY = 295;
        for (int i = 0; i < 3; i ++) {
            for (int j = 0; j < 3; j ++) {
                x = i + centreX;
                y = j + centreY;
                px = glm::vec3(x, y, heightMap(x, y));
                parentTransformation=glm::mat4(1.0);
                localTransformation = glm::translate( glm::mat4(1.0), px);
                localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
                localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));
                auto pingouin = std::make_shared<KeyframedMeshRenderable>(
                        texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                        4.0, px, false);
                pingouin->setParentTransform( parentTransformation );
                pingouin->setLocalTransform( localTransformation );
                viewer.addRenderable(pingouin);
            }
        }


        // Ajout de pingouins au milieu :
        px = glm::vec3(110, 280, heightMap(110, 280));
        parentTransformation=glm::mat4(1.0);
        localTransformation = glm::translate( glm::mat4(1.0), px);
        localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
        localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));
        auto pingouin2 = std::make_shared<KeyframedMeshRenderable>(
                 texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                 4.0, px, false);
        pingouin2->setParentTransform( parentTransformation );
        pingouin2->setLocalTransform( localTransformation );
        viewer.addRenderable(pingouin2);


        viewer.setAnimationLoop(true, tmax);
        viewer.startAnimation();
}


void ajoute_pingouin(ShaderProgramPtr texShader, Height heightMap, Viewer & viewer, float x, float y, int angle, int tmax) {

    auto px=glm::vec3(x, y, heightMap(x, y));
    auto parentTransformation=glm::mat4(1.0);
    auto localTransformation = glm::translate( glm::mat4(1.0), px);
    localTransformation = glm::rotate( localTransformation, float(M_PI_2), glm::vec3(1,0,0));
    localTransformation = glm::rotate( localTransformation, float(angle), glm::vec3(0,1,0));
    localTransformation = glm::scale( localTransformation, glm::vec3(0.051,0.051,0.051));

    auto ping = std::make_shared<KeyframedMeshRenderable>(
                            texShader, "../meshes/pingu.obj", "../textures/textures.bmp",
                            4.0, px, false);

    ping->setMaterial(Material::Pearl());
    ping->setParentTransform( parentTransformation );
    ping->setLocalTransform( localTransformation );

    int debut = rand()%(tmax-2);

    ping->addParentTransformKeyframe(debut,
            GeometricTransformation(glm::vec3(0, 0, 0),
            glm::quat(glm::vec3(0.0, 0.0, 0.0))));
    ping->addParentTransformKeyframe(debut+1,
            GeometricTransformation(glm::vec3(0, 0, 2),
            glm::quat(glm::vec3(0.0, 0.0, 0.0))));
    ping->addParentTransformKeyframe(debut+2,
            GeometricTransformation(glm::vec3(0, 0, 0),
            glm::quat(glm::vec3(0.0, 0.0, 0.0))));

    viewer.addRenderable(ping);
}
