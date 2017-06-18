#ifndef TEXTURED_MESH_RENDERABLE_PHYSIC_HPP
#define TEXTURED_MESH_RENDERABLE_PHYSIC_HPP

#include "./TexturedMeshRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../ground/Height.hpp"
#include "../lighting/LightedMeshRenderable.hpp"
#include "../Viewer.hpp"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "./../SkyBox.hpp"

/* Stand-alone class, without inheritance from existing objects */

class TexturedMeshRenderablePhysic : public TexturedMeshRenderable
{
public:
    ~TexturedMeshRenderablePhysic();
    TexturedMeshRenderablePhysic(ShaderProgramPtr program,
        const std::string& meshFilename,
        const std::string& textureFilename,
        const glm::vec3 &position, const glm::vec3 &velocity,
                           const float &mass, const float &radius,bool follow,
                          Viewer *v, Height hm, SkyBoxPtr sb );
    void setAngle (float* a);
    void do_draw();
    float* angle;
    float precangle;
    float roulis;
    bool follow;
    Viewer *viewer;
    float prechm;
    Height hm;
    SkyBoxPtr sb;


//--------------------------------gestion physique-------------------------------

const glm::vec3& getPosition() const;

/**@brief Access to this particle's velocity.
 *
 * Get the velocity of this particle.
 * @return The particle's velocity.
 */
const glm::vec3& getVelocity() const;

/**@brief Access to this particle's applied force.
 *
 * Get the force applied to this particle.
 * @return The particle's applied force.
 */
const glm::vec3& getForce() const;

/**@brief Access to this particle's mass.
 *
 * Get the mass of this particle.
 * @return The particle's mass.
 */
float getMass() const;





/**@brief Set the particle's position.
 *
 * Set the position of this particle.
 * @param pos The new position of this particle.
 */
void setPosition(const glm::vec3 &pos);

/**@brief Set the particle's velocity.
 *
 * Set the velocity of this particle.
 * @param vel The new velocity of this particle.
 */
void setVelocity(const glm::vec3 &vel);

/**@brief Set the particle's applied force.
 *
 * Set the force applied to this particle.
 * @param force The new force applied to this particle.
 */
void setForce(const glm::vec3 &force);




/**@brief Increment the particle's position.
 *
 * Increment the position of this particle.
 * @param pos The position to add to this particle's position,
 * i.e. m_position += pos.
 */
void incrPosition(const glm::vec3 &pos);

/**@brief Increment the particle's velocity.
 *
 * Increment the velocity of this particle.
 * @param vel The velocity to add to this particle's velocity,
 * i.e. m_velocity += vel.
 */
void incrVelocity(const glm::vec3 &vel);

/**@brief Increment the particle's applied force.
 *
 * Increment the force applied to this particle.
 * @param force The force to add to this particle's applied force,
 * i.e. m_force += force.
 */
void incrForce(const glm::vec3 &force);

/**@brief Restart the particle.
 *
 * Set the particle's position and velocity to their initial values.
 */
void restart();

//-----------------------------collision-----------------------
collisionSphere* computeCollisionSphere();
private:
/**@brief The initial particle's position.
 *
 * The initial position of this particle, stored to be set at restart.
 */
const glm::vec3 m_initialPosition;

/**@brief The initial particle's velocity.
 *
 * The initial velocity of this particle, stored to be set at restart.
 */
const glm::vec3 m_initialVelocity;

/**@brief The particle's position.
 *
 * The position of this particle.
 */
glm::vec3 m_position;

/**@brief The particle's velocity.
 *
 * The position of this particle.
 */
glm::vec3 m_velocity;

/**@brief The particle's applied force.
 *
 * The force applied to this particle.
 */
glm::vec3 m_force;

/**@brief The particle's mass.
 *
 * The mass of this particle.
 */
float m_mass;
bool isFixed() const;


//------------------------------------------------------------------------------------

};

typedef std::shared_ptr<TexturedMeshRenderablePhysic> TexturedMeshRenderablePhysicPtr;

/**
* \brief output stream operator, as non-member
*/
std::ostream& operator<<(std::ostream& os, const TexturedMeshRenderablePhysicPtr& p);
#endif
