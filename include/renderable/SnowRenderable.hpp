#ifndef SNOW_RENDERABLE_HPP
#define SNOW_RENDERABLE_HPP

#include "../../include/Viewer.hpp"
#include "../../include/ShaderProgram.hpp"

#include <vector>
#include <glm/glm.hpp>

class SnowRenderable
{
    public:
        SnowRenderable(ShaderProgramPtr texShader, ShaderProgramPtr flatShader, 
                Viewer& viewer, int tmax);
        ~SnowRenderable();
};

#endif
