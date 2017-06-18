#include "./../include/CylinderRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

CylinderRenderable::CylinderRenderable(ShaderProgramPtr shaderProgram) :
    Renderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0)
{
    // Here you must:
    // 1. define the positions and colors of all your vertices
    //    (you might stored them in m_positions and m_colors)
    // 2. generate gl buffers,
    // 3. for each of them, bind then send data
    //
    // No code is provided, let's see if you're getting
    // more comfortable (or not) with these different steps
    //
    // For step 1, ask yourself how many vertices you want, are they
    // shared or not by multiple triangles, how to compute their position, ...
    // Indexing is more difficult, you may start with direct generation first
    
    glm::vec3 center, current, previous = glm::vec3(0,0,0);
    int triangleNb = 200;
    float rayon = 1.0;
    
    /*
     *      PREMIER CERCLE (dessous)
     */
    previous = glm::vec3(rayon*cos((triangleNb-1)*2*M_PI/triangleNb), rayon*sin((triangleNb-1)*2*M_PI/triangleNb), 0);
    for (int i = 0; i <= triangleNb; i++) {
        m_positions.push_back(center);
        m_positions.push_back(previous);
        current = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), 0);
        m_positions.push_back(current);  
        previous = current;
    }
        
    // Génération de la couleur du cercle : 
    glm::vec4 color =  randomColor();
    for (int i = 0; i <= triangleNb*3; i++) {
        m_colors.push_back(color);
    }
    
    /*
     *      DEUXIEME CERCLE (dessus)
     */
    int hauteur = 1;
    center = glm::vec3(0,0,hauteur);
    previous = glm::vec3(rayon*cos((triangleNb-1)*2*M_PI/triangleNb), rayon*sin((triangleNb-1)*2*M_PI/triangleNb), hauteur);
    // Generation d'un cercle 
    for (int i = 0; i <= triangleNb; i++) {
        m_positions.push_back(center);
        m_positions.push_back(previous);
        current = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), hauteur);
        m_positions.push_back(current);  
        previous = current;
    }
        
    // Génération de la couleur du second cercle : 
    color =  randomColor();
    for (int i = 0; i <= triangleNb*3 +1; i++) {
        m_colors.push_back(color);
    }
    
    /*
     *      CORPS du cylindre
     */
    
    /*
     *  On trace maintenant les triangles donc la base est sur le cercle du bas
     * donc deux points en bas et un en haut (center)
     */
    center = glm::vec3(rayon*cos((triangleNb-1)*2*M_PI/triangleNb), rayon*sin((triangleNb-1)*2*M_PI/triangleNb), hauteur);
    previous = glm::vec3(rayon*cos((triangleNb-1)*2*M_PI/triangleNb), rayon*sin((triangleNb-1)*2*M_PI/triangleNb), 0);

    for (int i = 0; i <= triangleNb; i++) {
        m_positions.push_back(center);
        m_positions.push_back(previous);
        current = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), 0);
        m_positions.push_back(current);  
        previous = current;
        
        glm::vec3 normal = glm::vec3(fabs((current[0]+previous[0])/2), 
                                fabs((current[1]+previous[1])/2), 
                                0.0);
                                //fabs((center[3]+previous[3]+current[3])/3));
        
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0)));  
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0))); 
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0))); 
        center = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), hauteur);
    }
    
    /*color =  randomColor();
    for (int i = 0; i <= triangleNb*3 +2; i++) {
        m_colors.push_back(color);
    }*/
    
    /*
     *  On trace le reste
     */
    previous = glm::vec3(rayon*cos((triangleNb-1)*2*M_PI/triangleNb), rayon*sin((triangleNb-1)*2*M_PI/triangleNb), hauteur);

    for (int i = 0; i <= triangleNb; i++) {
        center = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), 0);
        m_positions.push_back(center);
        m_positions.push_back(previous);
        current = glm::vec3(rayon*cos(i*2*M_PI/triangleNb), rayon*sin(i*2*M_PI/triangleNb), hauteur);
        m_positions.push_back(current);
        
        glm::vec3 normal = glm::vec3(fabs(center[0]), 
                                fabs(center[1]), 
                                0.0);
        
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0))); 
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0))); 
        m_colors.push_back(glm::normalize(glm::vec4(normal, 1.0)));  
        previous = current;        
    }
    
   /*for (int i = 0; i <= triangleNb*3 +2; i++) {
        m_colors.push_back(color);
    }*/
    
    
    /* 
     *      Calcul des normales :
     *      - du centre vers le point moitié des deux points du même cercle
     *      - centre du cercle éloigné vers le courant
     */
    
    /* indexé : normale sur les sommets dans le prolongement des segments depuis le centre du cercle */
    
    
    
    m_model = glm::mat4(1.0);
    glGenBuffers(1, &m_pBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_cBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);
}

CylinderRenderable::~CylinderRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}

void CylinderRenderable::do_draw()
{
    // Here you must:
    // 1. get the model matrix and send it to the shader (as uniform)
    // 2. for each attribute: retrieve its location on the shader,
    //    activate it, bind a buffer, then specify internal format
    // 3. draw the triangles
    // 4. unbind everything
    
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model));
    
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    glEnableVertexAttribArray(positionLocation);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // Gestion des couleurs 
    int color = m_shaderProgram->getAttributeLocation("colorAttribute");
    glEnableVertexAttribArray(color);
    glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
    glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES,0, m_positions.size());
    glDrawArrays(GL_TRIANGLES, 0, m_colors.size());
      
    glDisableVertexAttribArray(positionLocation);
    glDisableVertexAttribArray(color); 
}

void CylinderRenderable::do_animate(float time)
{
}
