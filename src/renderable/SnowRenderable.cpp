#include "../../include/renderable/SnowRenderable.hpp"

#include "../../include/keyframes/KeyframedCylinderRenderable.hpp"



SnowRenderable::SnowRenderable(ShaderProgramPtr texShader, ShaderProgramPtr flatShader,
        Viewer& viewer, int tmax)
{

    unsigned int n = 300;
    srand(time(NULL));
    float cycle[tmax];
    /* tableau qui contient les positions clés classiques d'un flocon
     * on va s'en servir pour avoir des flocons qui suivent le cylce sans commencer
     * en même temps à la position zero */

    for (int i = 0; i <tmax; i ++) {
        cycle[i] = i;
    }

    for (unsigned int i = 0; i < n; i+=8) {
        for (unsigned int j = 0; j < n; j+=8) {
            int debut = rand() % tmax;

            auto cylinder = std::make_shared<KeyframedCylinderRenderable>(texShader, Material::Snow());
            cylinder->setLocalTransform( GeometricTransformation( glm::vec3{i,j,debut}, glm::quat(), glm::vec3{0.12,0.12,0.12}).toMatrix() );

            for (int t = 1; t < debut; t ++) {
                cylinder->addParentTransformKeyframe(t-1, GeometricTransformation(glm::vec3(0.0, 0.0, -t) ));
            }
            for (int t = debut; t <= tmax + 1; t ++) {
                cylinder->addParentTransformKeyframe(t-1, GeometricTransformation(glm::vec3(0.0, 0.0, tmax-t) ));
            }
            viewer.addRenderable(cylinder);
        }
    }
}


SnowRenderable::~SnowRenderable() {

}
