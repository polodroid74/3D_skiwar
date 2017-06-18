#ifndef TEXTURED_GROUND_RENDERABLE_HPP
#define TEXTURED_GROUND_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../ground/Height.hpp"
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class TexturedGroundRenderable : public HierarchicalRenderable
{
public :
    ~TexturedGroundRenderable();
    TexturedGroundRenderable(Height heightMap, ShaderProgramPtr shaderProgram, const std::string& textureFilename);
    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate(float time);

	Height m_heightMap;
    std::vector< glm::vec3 > m_positions;
	std::vector< unsigned int > m_indexes;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2 > m_texCoords;
    std::vector< glm::vec2 > m_origTexCoords;

    unsigned int m_pBuffer;
    unsigned int m_iBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    unsigned int m_wrapOption;
    unsigned int m_filterOption;

    MaterialPtr m_material;
};

typedef std::shared_ptr<TexturedGroundRenderable> TexturedGroundRenderablePtr;

#endif
