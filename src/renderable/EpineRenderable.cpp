#include "./../../include/renderable/EpineRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

EpineRenderable::EpineRenderable(ShaderProgramPtr shaderProgram):
    HierarchicalRenderable(shaderProgram), 
    m_pBuffer(0),
    m_cBuffer(0),
    m_iBuffer(0)
{
    m_positions.push_back(glm::vec3(-0.5,0,0));
    m_positions.push_back(glm::vec3(0,0,1));
    m_positions.push_back(glm::vec3(0.5,0,0));
    
    m_model = glm::mat4(1.0);
    glGenBuffers(1, &m_pBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

    // Gestion des couleurs aléatoires 
    glm::vec4 color =  glm::vec4(0.10f,0.31f,0.03f,1.0f);

    for (int i = 0; i < 3; i ++)
	    m_colors.push_back(color);
            
    glGenBuffers(1, &m_cBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);
}

EpineRenderable::~EpineRenderable()
{
    glDeleteBuffers(1, &m_pBuffer);
    glDeleteBuffers(1, &m_cBuffer);
    glDeleteBuffers(1, &m_iBuffer);
}

void EpineRenderable::do_animate(float time) {}

void EpineRenderable::EpineRenderable::do_draw()
{
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}
