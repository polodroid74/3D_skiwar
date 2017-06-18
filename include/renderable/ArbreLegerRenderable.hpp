#ifndef ARBRE_LEGER_RENDERABLE_HPP
#define ARBRE_LEGER_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "../../include/Viewer.hpp"
#include "../../include/ShaderProgram.hpp"
#include "../../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../../include/keyframes/KeyframedConeRenderable.hpp"
#include "./../HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class ArbreLegerRenderable : public HierarchicalRenderable
{
public:
  ~ArbreLegerRenderable();
  ArbreLegerRenderable(ShaderProgramPtr shader, ShaderProgramPtr flatShader,
    Viewer & viewer, float l, glm::vec3 origin);

    //-------------------------------collisiions---------------------
    //-----------------------------collision-----------------------
    collisionSphere* computeCollisionSphere();
    glm::vec3 translation;
    float m_radius;//collisions
    bool Fixed;
    /**@brief Access to this particle's radius.
    *
    * Get the radius of this particle.
    * @return The particle's radius.
    */
    float getRadius() const;
    /**@brief Set the particle's radius.
    *
    * Set the radius of this particle.
    * @param radius The new radius of this particle.
    */
    void setRadius(const float& radius);
    /**@brief Check if this particle is fixed.
    *
    * Check if this particle is fixed.
    * @return True if the particle is fixed.
    */
    const glm::vec3& getPosition() const;
    bool isFixed() const;



  protected:
    void do_draw();
    void do_animate(float time);

  private:
    //KeyframedConeRenderablePtr m_root;

    /*    void branche(ShaderProgramPtr shader, ShaderProgramPtr flatShader,
    HierarchicalRenderablePtr father,
    float l, int alpha_h, int alpha_v, float rayon);

    void branche_principale(ShaderProgramPtr shader, ShaderProgramPtr flatShader,
    HierarchicalRenderablePtr father,
    float longTronc, int alpha_h, int alpha_v, int compt, float rayon);

    void tronc(const ShaderProgramPtr& shader, ShaderProgramPtr flatShader,
    HierarchicalRenderablePtr father, int a,
    float l, int alpha_h, int alpha_v, float r);
    */
  };

  typedef std::shared_ptr<ArbreLegerRenderable> ArbreLegerRenderablePtr;

  #endif
