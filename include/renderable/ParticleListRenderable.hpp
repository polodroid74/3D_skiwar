# ifndef ARBRELISTRENDERABLE_HPP_
# define ARBRELISTRENDERABLE_HPP_

#include "../renderable/ArbreLegerRenderable.hpp"
#include "../../include/renderable/ArbreLegerRenderable.hpp"
#include "../HierarchicalRenderable.hpp"
#include "../Utils.hpp"
#include "../gl_helper.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

/**@brief Draw a list of particles.
 *
 * This class is used to draw a list of particle. This is more efficient
 * than to render individually each particle. We could do even more
 * efficient but it is beyond the objectives of those practicals.
 */
class ArbreLegerListRenderable :
        public HierarchicalRenderable
{
public:
    /**@brief Build a renderable to render a set of particles.
     *
     * Build a renderable to render a set of particles.
     * @param program The shader program used to render the particles.
     * @param particles The set of particles to render.
     */
    ArbreLegerListRenderable(ShaderProgramPtr program, std::vector<ArbreLegerRenderablePtr>& particles);

    ~ArbreLegerListRenderable();

private:
    void do_draw();
    void do_animate(float time);

    std::vector<ArbreLegerPtr> m_particles;

    size_t m_numberOfVertices;
    unsigned int m_positionBuffer;
    unsigned int m_colorBuffer;
    unsigned int m_normalBuffer;
};

typedef std::shared_ptr<ArbreLegerListRenderable> ArbreLegerListRenderablePtr;

# endif
