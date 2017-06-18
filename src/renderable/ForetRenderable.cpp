#include "../../include/renderable/ForetRenderable.hpp"
#include "../../include/renderable/ArbreLegerRenderable.hpp"

ForetRenderable::ForetRenderable(ShaderProgramPtr phongShader, ShaderProgramPtr flatShader,
        Viewer& viewer, unsigned int taille_case, unsigned int taille_ss_case,
        unsigned int nb_sapins, Height h)
{
    unsigned int ecart = ceil((taille_case - taille_ss_case)/2);
    unsigned int marge = taille_case - 2*ecart;

    for (unsigned int i = 1; i <= nb_sapins; i ++) {
        for (unsigned int j = 1; j <= nb_sapins; j++) {
            float l = 5.0 + ((float)(rand()%3) / 10.0);
            float px = i*taille_case+ecart + rand()%marge;
            float py = j*taille_case + ecart + rand()%marge;
            glm::vec3 origine = glm::vec3(px, py, h(px, py));
            std::make_shared<ArbreLegerRenderable>(phongShader,
                    flatShader, viewer, l, origine);
        }
    }

}


ForetRenderable::~ForetRenderable() {

}
