#ifndef EPINE_RENDERABLE_HPP
#define EPINE_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "./../lighting/Materiable.hpp"
#include <vector>
#include <glm/glm.hpp>

class EpineRenderable : public HierarchicalRenderable//, public Materiable
{
    public:
        ~EpineRenderable();
        EpineRenderable();
	EpineRenderable(ShaderProgramPtr program); //, const MaterialPtr& material = Material::Emerald());

    private:
        void do_draw();
        void do_animate(float time);

        std::vector<glm::vec3> m_positions;
        unsigned int m_pBuffer;

        std::vector<glm::vec4> m_colors;
        unsigned int m_cBuffer;

        std::vector<int> m_index;
        unsigned int m_iBuffer;
};

typedef std::shared_ptr<EpineRenderable> EpineRenderablePtr;

#endif
