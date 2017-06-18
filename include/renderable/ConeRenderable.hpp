#ifndef CONE_RENDERABLE_HPP
#define CONE_RENDERABLE_HPP

#include "../../include/HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>


class ConeRenderable : public HierarchicalRenderable
{
    public:
        ~ConeRenderable();
        ConeRenderable(ShaderProgramPtr program, 
                            unsigned int nbSlices = 5);

    protected:
        void do_draw();
        void do_animate(float time);

    private:
        
        void getUnitCone(std::vector<glm::vec3>& positions, 
        std::vector<glm::vec3>& normals, const unsigned int& strips, 
        const unsigned int& slices);

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;
        std::vector<glm::ivec3> m_indices;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        unsigned int m_iBuffer;
};

#endif
