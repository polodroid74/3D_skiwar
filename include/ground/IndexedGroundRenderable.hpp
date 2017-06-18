#ifndef INDEXED_GROUND_RENDERABLE_HPP
#define INDEXED_GROUND_RENDERABLE_HPP

#include "../Renderable.hpp"
#include "Height.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "../Utils.hpp"

class IndexedGroundRenderable : public Renderable
{
    public:
        ~IndexedGroundRenderable();
        IndexedGroundRenderable();
        IndexedGroundRenderable(ShaderProgramPtr program);

    private:
        void do_draw();
        void do_animate(float time);

        Height m_heightMap;
        std::vector<glm::vec3> m_positions;
        std::vector<glm::vec4> m_colors;
        std::vector< unsigned int > m_indexes;
        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_iBuffer;
};

typedef std::shared_ptr<IndexedGroundRenderable> IndexedGroundRenderablePtr;

#endif
