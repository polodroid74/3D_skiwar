#ifndef FORET_RENDERABLE_HPP
#define FORET_RENDERABLE_HPP

#include "../../include/Viewer.hpp"
#include "../../include/ShaderProgram.hpp"
#include "../../include/ground/Height.hpp"
#include "./../HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class ForetRenderable
{
    public:
        ForetRenderable(ShaderProgramPtr phongShader, ShaderProgramPtr flatShader,
            Viewer& viewer, unsigned int taille_case, unsigned int taille_ss_case,
            unsigned int nb_sapins, Height h);
        ~ForetRenderable();
};

#endif
