#ifndef TEXTURED_MESH_RENDERABLE_HPP
#define TEXTURED_MESH_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../lighting/LightedMeshRenderable.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class TexturedMeshRenderable : public HierarchicalRenderable
{
public:
    ~TexturedMeshRenderable();
    TexturedMeshRenderable(ShaderProgramPtr program,
      const std::string& meshFilename,
      const std::string& textureFilename,
      float radius,
      glm::vec3 translation,
      bool Fixed);
      void setMaterial(const MaterialPtr& material);
        //-----------------------------collision-----------------------
        collisionSphere* computeCollisionSphere();
        glm::vec3 translation;
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



protected:
    void do_draw();
    void do_animate(float time);

    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned int> m_indices;
    std::vector<glm::vec2> m_texCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_iBuffer;
    unsigned int m_tBuffer;

    unsigned int m_texId;
    /**@brief The particle's radius.
    *
    * The radius of this particle.
    */
    float m_radius;
    MaterialPtr m_material;


};

typedef std::shared_ptr<TexturedMeshRenderable> TexturedMeshRenderablePtr;

#endif
