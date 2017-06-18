#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "./HierarchicalRenderable.hpp"
#include "./lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class SkyBox : public HierarchicalRenderable
{
public :
    SkyBox(ShaderProgramPtr shaderProgram, const std::string& textureFilename);
    ~SkyBox();
	void setPosition(const double x, const double y);
    void setMaterial(const MaterialPtr& material);

private:
    void do_draw();
    void do_animate(float time);

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec2 > m_texCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;
	double m_posx=0.0;
	double m_posy=0.0;

    MaterialPtr m_material;
};

typedef std::shared_ptr<SkyBox> SkyBoxPtr;

#endif
