#include "../../include/renderable/ConeRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

 ConeRenderable::ConeRenderable(ShaderProgramPtr shaderProgram,
     unsigned int nbSlices)
  : HierarchicalRenderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_iBuffer(0)
{
     
     getUnitCone(m_positions, m_normals, 1, nbSlices); 
                //const unsigned int& strips, const unsigned int& slices)
    //Generate buffers
    glcheck(glGenBuffers(1, &m_pBuffer));
    glcheck(glGenBuffers(1, &m_cBuffer));
    glcheck(glGenBuffers(1, &m_nBuffer));

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

 ConeRenderable::~ConeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void  ConeRenderable::do_draw()
{
    //Location
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

    //Send data to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
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

void  ConeRenderable::do_animate(float time)
{
}


void  ConeRenderable::getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices)
{
    // basic cylinder: radius 1, along z axis, bases on z == 0 and z == 1
    // The normal on a given coord around z is the coord itself!
    //
    // NORMAL PER FACE: in this case, each slice of the cylinder has its
    // own normal (1 for the 2 triangles of a slice)

    double height = 1.0;
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);
    int numberOfVertices = slices*strips*6 + slices*3;
    positions.resize(numberOfVertices);
    normals.resize(numberOfVertices);

    //Lateral surface
    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_u = j*(height/(double)strips);
            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_u = (j+1)*(height/(double)strips);
            double factor1 = (height-curr_u)/height;
            double factor2 = (height-next_u)/height;

            m_positions[i*strips*6 + 6*j +0] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            m_positions[i*strips*6 + 6*j +1] = center + glm::vec3(factor1*radius*cos(next_theta),
                    factor1*radius*sin(next_theta),
                    curr_u);
            m_positions[i*strips*6 + 6*j +2] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);

            m_normals[i*strips*6 + 6*j +0] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            m_normals[i*strips*6 + 6*j +1] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        curr_u));
            m_normals[i*strips*6 + 6*j +2] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        next_u));

            m_positions[i*strips*6 + 6*j +3] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            m_positions[i*strips*6 + 6*j +4] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);
            m_positions[i*strips*6 + 6*j +5] = center + glm::vec3(factor2*radius*cos(curr_theta),
                    factor2*radius*sin(curr_theta),
                    next_u);

            m_normals[i*strips*6 + 6*j +3] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            m_normals[i*strips*6 + 6*j +4] = glm::normalize(glm::vec3(cos(next_theta),
                        radius*sin(next_theta),
                        next_u));
            m_normals[i*strips*6 + 6*j +5] = glm::normalize(glm::vec3(cos(curr_theta),
                        radius*sin(curr_theta),
                        next_u));

        }
    }

    //Bottom faces
    unsigned int counter = slices*strips*6;
    for(int i=0; i<slices; ++i)
    {
        double curr_theta = i*(2.0*M_PI/(double)slices);
        double next_theta = (i+1)*(2.0*M_PI/(double)slices);
        double u_base = 0.0;

        m_positions[counter+3*i+0] = center + glm::vec3(0, 0, u_base );
        m_positions[counter+3*i+1] = center + glm::vec3(radius*cos(curr_theta), radius*sin(curr_theta), u_base);
        m_positions[counter+3*i+2] = center + glm::vec3(radius*cos(next_theta), radius*sin(next_theta), u_base);

        m_normals[counter+3*i+0] = glm::vec3(0, 0, -1);
        m_normals[counter+3*i+1] = glm::vec3(0, 0, -1);
        m_normals[counter+3*i+2] = glm::vec3(0, 0, -1);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*
    glm::vec3 bottomCenter(0.0, 0.0, 0.0);
    glm::vec3 topCenter(0.0, 0.0, 1.0);

    double dTheta = 2.0 * M_PI / (double) nbSlices;
    double currentTheta = 0.0;
    double currentCos = 1.0;
    double currentSin = 0.0;

    for (unsigned int i = 0; i < nbSlices; ++i) {
        double nextCos = cos(currentTheta + dTheta);
        double nextSin = sin(currentTheta + dTheta);
        // normal: computed in the middle of the slice
        glm::vec3 normal(cos(currentTheta + dTheta / 2.0),
                         sin(currentTheta + dTheta / 2.0), 0.0);
        // let's color this cube with normal values
        // (since we do not have illumination yet, it will create a shaded effect)

        // first triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(glm::vec4(normal, 1.0));
            m_normals.push_back(normal);
        }

        // second triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(glm::vec4(normal, 1.0));
            m_normals.push_back(normal);
        }

        // bottom and top bases
        m_positions.push_back(bottomCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));

        // glm::vec4 color((i%2==0) ? 0.0 : 1.0, 0.0, (i%2==0) ? 1.0 : 0.0, 1.0);
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, -1.0));
            m_colors.push_back(glm::vec4(0.0, 0.0, -1.0, 1.0));
        }

        m_positions.push_back(topCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, +1.0));
            m_colors.push_back(glm::vec4(0.0, 0.0, +1.0, 1.0));
        }

        currentTheta = currentTheta + dTheta;
        currentCos = nextCos;
        currentSin = nextSin;
    }*/
}