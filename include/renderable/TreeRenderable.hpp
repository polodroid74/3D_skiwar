#ifndef TREE_RENDERABLE_HPP
#define TREE_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "../../include/Viewer.hpp"
#include "../../include/ShaderProgram.hpp"
#include "../../include/keyframes/KeyframedCylinderRenderable.hpp"

#include <vector>
#include <glm/glm.hpp>

class TreeRenderable : public HierarchicalRenderable
{
    public:
        ~TreeRenderable();
        TreeRenderable(ShaderProgramPtr shader, ShaderProgramPtr flatShader, 
                Viewer & viewer, float l, float rayon, int height, 
                glm::vec3 origin);

    protected:
        void do_draw();
        void do_animate(float time);
        
    private:
        KeyframedCylinderRenderablePtr m_root;
        
        void branche(ShaderProgramPtr shader, ShaderProgramPtr flatShader,  
                HierarchicalRenderablePtr father,
                float l, int alpha_h, int alpha_v, float rayon);
        
        void branche_principale(ShaderProgramPtr shader, ShaderProgramPtr flatShader, 
                HierarchicalRenderablePtr father, 
                float longTronc, int alpha_h, int alpha_v, int compt, float rayon);
        
        void tronc(const ShaderProgramPtr& shader, ShaderProgramPtr flatShader, 
                HierarchicalRenderablePtr father, int a, 
                float l, int alpha_h, int alpha_v, float r);

};

typedef std::shared_ptr<TreeRenderable> TreeRenderablePtr;

#endif
