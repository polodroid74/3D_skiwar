#include "./../../include/ground/IndexedGroundRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/ground/NoiseProvider.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <GL/glew.h>

IndexedGroundRenderable::IndexedGroundRenderable(ShaderProgramPtr shaderProgram) :
    Renderable(shaderProgram),
    m_pBuffer(0),m_cBuffer(0),m_iBuffer(0)
{
	m_heightMap = Height(100,100,10.0,10.0,0); //nb carreau x,y ; taille x,y
	NoiseProvider::randomizeHMap(&m_heightMap);

	int xNbMax = m_heightMap.sizeNx(); 
	int yNbMax = m_heightMap.sizeNy();
	double xsize = m_heightMap.getLx();
	double ysize = m_heightMap.getLy();
	//Position
	for(int i=0; i<xNbMax ;i++)
	{
		for(int j=0; j<yNbMax ;j++)
		{
			//std::cout << "Point i : " << i << "\tj : " << j <<"\n";
			m_positions.push_back(glm::vec3(i*xsize/xNbMax,j*ysize/yNbMax,m_heightMap(i,j)));
		}
	}

	m_model = glm::mat4(1.0);

	glGenBuffers(1, &m_pBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);

	//Color
	glm::vec4 col = glm::vec4(255.0,255.0,255.0,255.0);
	for(int i=0;i<10000;i++)
	{
		col = randomColor();
		for(int j=0;j<4;j++)
		{
			m_colors.push_back(col);
		}
	}

	glGenBuffers(1, &m_cBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW);

	for(int i=0; i<xNbMax-1 ;i++)
	{
		for(int j=0; j<yNbMax-1 ;j++)
		{
			//std::cout << "Ind i : " << i << "\tj : " << j <<"\n";
			m_indexes.push_back(i*(yNbMax)+j);
			m_indexes.push_back(i*(yNbMax)+(j+1));
			m_indexes.push_back((i+1)*(yNbMax)+j);
			m_indexes.push_back((i+1)*(yNbMax)+j);
			m_indexes.push_back((i+1)*(yNbMax)+(j+1));
			m_indexes.push_back(i*(yNbMax)+(j+1));
		}
	}

	//std::cout << "xNbMax : " << xNbMax << "\tyNbMax : " << yNbMax <<"\n";
	glGenBuffers(1, &m_iBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size()* sizeof(unsigned int), m_indexes.data(), GL_STATIC_DRAW);
}

IndexedGroundRenderable::~IndexedGroundRenderable()
{
	    glDeleteBuffers(1, &m_pBuffer);
	    glDeleteBuffers(1, &m_cBuffer);
}
void IndexedGroundRenderable::do_draw()
{
	//Lien entre variable GPU et conenu du shader
	//
	 int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
	 //Send the data corresponding to this identifier on the GPU
		 glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model));

	 //Get the identifier of the attribute vPosition in the shader program
	 int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
	 //Activate the attribute array at this location
	 glEnableVertexAttribArray(positionLocation);
	 //Bind the position buffer on the GL_ARRAY_BUFFER target
	 glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer);
	 //Specify the location and the format of the vertex position attribute
	 glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	 int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
	 glEnableVertexAttribArray(colorLocation);
	 glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer);
	 glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	 //For index
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_iBuffer);
	 //Draw the triangles
	 //glDrawElements(GL_QUADS,m_indexes.size(),GL_UNSIGNED_INT,(void*)0);
	 glDrawElements(GL_TRIANGLES,m_indexes.size(),GL_UNSIGNED_INT,(void*)0);

	 //Release the vertex attribute array
	 glDisableVertexAttribArray(positionLocation);
}

void IndexedGroundRenderable::do_animate(float time)
{
}


