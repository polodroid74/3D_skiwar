#include "../../include/texturing/TexturedGroundRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

TexturedGroundRenderable::TexturedGroundRenderable(
        Height heightMap, ShaderProgramPtr shaderProgram, const std::string& textureFilename)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_iBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0),
      m_wrapOption(0), m_filterOption(0)
{
	m_heightMap = heightMap;
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
			//	texture
		//	m_texCoords.push_back(glm::vec2((double)i/xNbMax,(double)j/yNbMax));
			//m_texCoords.push_back(glm::vec2(i,j));
			double dx =2*m_heightMap(i,j)-(m_heightMap((i-1+xNbMax)%xNbMax,j)+m_heightMap((i+1)%xNbMax,j));
			double dy = 2*m_heightMap(i,j)-(m_heightMap(i,(j-1+yNbMax)%yNbMax)+m_heightMap(i,(j+1)%yNbMax));
			if(dx*dx > 0.25 || dy*dy > 0.25 ){
				m_texCoords.push_back(glm::vec2(i/30.0,j/20.0));
			} else {
				m_texCoords.push_back(glm::vec2(i/30.0,j/20.0));
			}
			double nx = 3*(m_heightMap((i-1+xNbMax)%xNbMax,j)-m_heightMap((i+1)%xNbMax,j));
			double ny = 3*(m_heightMap(i,(j-1+yNbMax)%yNbMax)-m_heightMap(i,(j+1)%yNbMax));
			m_normals.push_back(glm::vec3(nx,ny,1.0));
		}
	}

	m_model = glm::mat4(1.0);


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

    // Initialize geometry
    //teachers::getUnitPlane(m_positions, m_normals, m_origTexCoords);
	//IndexedGroundRenderable::getGround();
    //m_texCoords = m_origTexCoords;

    // === PART 1: Vertex attributes, except texture coordinates
    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));

    // === PART 2: Texture
    // texture coordinates: just like any other vertex attributes!
    glGenBuffers(1, &m_tBuffer); //texture coords
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    // now handle the "texture image" itself
    // load the image (here using the sfml library)
    sf::Image image;
    image.loadFromFile(textureFilename);
    // sfml inverts the v axis...
    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
    image.flipVertically();

    // create a GPU buffer then bind the texture
    glcheck(glGenTextures(1, &m_texId));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

    // texture options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    //glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    //glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // Transfer the texture image texture to the GPU
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
        image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        (const GLvoid*)image.getPixelsPtr()));

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
}

TexturedGroundRenderable::~TexturedGroundRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));

    glcheck(glDeleteTextures(1, &m_texId));
}

void TexturedGroundRenderable::do_draw()
{
    //Locations
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocaction = m_shaderProgram->getUniformLocation("texSampler");

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");
	



    //Send material as uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniforms to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    //Send vertex attributes to GPU
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    // Texture and texture coordinates
    if (texCoordLocation != ShaderProgram::null_location) {
        // Bind texture on texture unit 0
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocaction, 0));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    //glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_iBuffer));
	glcheck(glDrawElements(GL_TRIANGLES,m_indexes.size(),GL_UNSIGNED_INT,(void*)0));

    //Release everything
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }

    if (texCoordLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(texCoordLocation));
        glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
    }
}

void TexturedGroundRenderable::do_animate(float time)
{
}

void TexturedGroundRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
