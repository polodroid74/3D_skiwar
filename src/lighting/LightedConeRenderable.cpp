#include "../../include/lighting/LightedConeRenderable.hpp"
#include "../../include/gl_helper.hpp"

#include <glm/gtc/type_ptr.hpp>


LightedConeRenderable::LightedConeRenderable(ShaderProgramPtr program, const MaterialPtr& material)
    : ConeRenderable(program), Materiable(material)
{
}

LightedConeRenderable::~LightedConeRenderable()
{
}

void LightedConeRenderable::do_draw()
{
    //Send material to GPU as uniform
    Material::sendToGPU(m_shaderProgram, getMaterial());

    //NIT matrix
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    // Call the super method to bind other uniforms/attributes & draw
    ConeRenderable::do_draw();

    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(nitLocation));
    }
}

void LightedConeRenderable::do_animate( float time )
{
}
