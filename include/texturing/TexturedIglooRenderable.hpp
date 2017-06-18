#ifndef TEXTURED_IGLOO_RENDERABLE_HPP
#define TEXTURED_IGLOO_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class TexturedIglooRenderable : public HierarchicalRenderable
{
public :
    ~TexturedIglooRenderable();
    TexturedIglooRenderable(ShaderProgramPtr shaderProgram, const std::string& textureFilename);
    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate(float time);

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2 > m_texCoords;
    std::vector< glm::vec2 > m_origTexCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    MaterialPtr m_material;
};

typedef std::shared_ptr<TexturedIglooRenderable> TexturedIglooRenderablePtr;

#endif
