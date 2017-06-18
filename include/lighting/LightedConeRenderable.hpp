#ifndef LIGHTED_CONE_RENDERABLE_HPP
#define LIGHTED_CONE_RENDERABLE_HPP

#include "./../lighting/Materiable.hpp"
#include "./../../include/renderable/ConeRenderable.hpp"

class LightedConeRenderable : public ConeRenderable, public Materiable
{
public:
    ~LightedConeRenderable();
    LightedConeRenderable(ShaderProgramPtr program, const MaterialPtr& material);

protected:
    void do_draw();
    void do_animate(float time);
};

typedef std::shared_ptr<LightedConeRenderable> LightedConeRenderablePtr;

#endif
