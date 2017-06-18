#include "../../include/texturing/TexturedMeshRenderablePhysic.hpp"
#include "../../include/texturing/TexturedMeshRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../include/Io.hpp"
#include "../../include/ground/Dvector.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


TexturedMeshRenderablePhysic::TexturedMeshRenderablePhysic(ShaderProgramPtr shaderProgram,
  const std::string& meshFilename, const std::string& textureFilename,
  const glm::vec3 &position, const glm::vec3 &velocity,
  const float &mass, const float &radius, bool follow,
  Viewer *v, Height hm, SkyBoxPtr sb)
  :TexturedMeshRenderable(shaderProgram, meshFilename, textureFilename,
    radius, position, false),
    m_initialPosition( position ), m_initialVelocity( velocity ),
    m_position(position),
    m_velocity(velocity),
    m_force(glm::vec3(0.0,0.0,0.0)),
    m_mass(mass),
    viewer(v)
    {
    this->follow=follow;
    this->precangle=0;
    this->roulis=0;
    this->hm=hm;
    this->prechm=0;
    this->sb=sb;
    }

    void TexturedMeshRenderablePhysic::do_draw()
    {
      //Locations
      int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
      int nitLocation = m_shaderProgram->getUniformLocation("NIT");
      int texSamplerLocation = m_shaderProgram->getUniformLocation("texSampler");

      int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
      int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
      int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");

      //Update the parent and local transform matrix to position the geometric data according to the particle's data.

      const float& pRadius = this->getRadius();
      const glm::vec3& pPosition = this->getPosition();

      glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(pRadius/7.0));
      glm::mat4 translate =glm::mat4(1.0);

      //gestion des dépassements de la carte
      uint Nx=hm.sizeNx();
      uint Ny=hm.sizeNy();
      if(this->getPosition()[0] >= Nx-5){
        this->setPosition(glm::vec3(Nx-5,getPosition()[1],getPosition()[2]));
      }
      if(this->getPosition()[0] <= 5.0){
        this->setPosition(glm::vec3(5.0,getPosition()[1],getPosition()[2]));
      }
      if(this->getPosition()[1] >= Ny-5){
        this->setPosition(glm::vec3(getPosition()[0],Ny-5,getPosition()[2]));
      }
      if(this->getPosition()[1] <= 5.0){
        this->setPosition(glm::vec3(getPosition()[0],5.0,getPosition()[2]));
      }

      float h=hm(this->getPosition()[0], this->getPosition()[1]);
      //suivi du terrain
      if(h<=this->getPosition()[2]/2.0 || (prechm-h<=0.05 && prechm-h>=-0.05)){
        translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition)+glm::vec3(0,0,prechm));
      }else{
        translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition)+glm::vec3(0,0,h));
        prechm=h;
      }
      //Roulis
      glm::mat4 rot= glm::rotate(glm::mat4(1.0), float(M_PI_2), glm::vec3(1,0,0));
      rot= glm::rotate(rot, *angle+float(M_PI_2), glm::vec3(0,1,0));

      if (precangle !=*angle){
        roulis=roulis+(*angle-precangle)*(glm::length(this->getVelocity()))/200.0;
      }else if(roulis*roulis > 0.01){
        while (roulis >= float(M_PI)){
          roulis -=2*float(M_PI);
        }
        while (roulis <= -float(M_PI)){
          roulis +=2*float(M_PI);
        }
        if(roulis <0){
          roulis=roulis +0.1;
        }else{
          roulis=roulis -0.1;
        }
      }else{
        roulis=0;
      }
      rot= glm::rotate(rot, -roulis, glm::vec3(0,0,1));
      precangle=*angle;
      setLocalTransform(translate*scale*rot);
      //Send material as uniform to GPU
      Material::sendToGPU(m_shaderProgram, m_material);
      //camera
      if (follow){
        viewer->getCamera().setViewMatrix( glm::lookAt( glm::vec3(-10*cos(*angle),-10*sin(*angle),3.0)+this->getPosition()+glm::vec3(0,0,prechm),
         this->getPosition()+glm::vec3(0,0,prechm), glm::vec3( 0, 0, 5)));
         this->sb->setPosition(this->getPosition()[0],this->getPosition()[1]);
      }
      //Send uniforms to GPU
      if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
      }
      if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
          glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
        }

        //Send vertex attributes to GPU
        if (positionLocation != ShaderProgram::null_location) {
          glcheck(glEnableVertexAttribArray(positionLocation));
          glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
          glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
        }
        if (normalLocation != ShaderProgram::null_location) {
          glcheck(glEnableVertexAttribArray(normalLocation));
          glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
          glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
        }

        // Texture and texture coordinates
        if (texCoordLocation != ShaderProgram::null_location) {
          // Bind texture on texture unit 0
          glcheck(glActiveTexture(GL_TEXTURE0));
          glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

          // Tells the sampler to use the texture unit 0
          glcheck(glUniform1i(texSamplerLocation, 0));

          // Send texture coordinates attributes
          glcheck(glEnableVertexAttribArray(texCoordLocation));
          glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
          glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
        }

        //Draw triangles elements
        glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
        glcheck(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0));


        //Release everything
        if (positionLocation != ShaderProgram::null_location) {
          glcheck(glDisableVertexAttribArray(positionLocation));
        }
        if (normalLocation != ShaderProgram::null_location) {
          glcheck(glDisableVertexAttribArray(normalLocation));
        }

        if (texCoordLocation != ShaderProgram::null_location) {
          glcheck(glDisableVertexAttribArray(texCoordLocation));
          glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
        }
      }

      void TexturedMeshRenderablePhysic::setAngle (float* a)
      {
        angle=a;
      }
      TexturedMeshRenderablePhysic::~TexturedMeshRenderablePhysic()
      {
          glcheck(glDeleteBuffers(1, &m_pBuffer));
          glcheck(glDeleteBuffers(1, &m_nBuffer));
          glcheck(glDeleteBuffers(1, &m_iBuffer));
          glcheck(glDeleteBuffers(1, &m_tBuffer));

          glcheck(glDeleteTextures(1, &m_texId));
      }
      //----------------------------------Ajout pour gestion physique-----------------------
      const glm::vec3 & TexturedMeshRenderablePhysic::getPosition() const
      {
        return m_position;
      }

      const glm::vec3 & TexturedMeshRenderablePhysic::getVelocity() const
      {
        return m_velocity;
      }

      const glm::vec3 & TexturedMeshRenderablePhysic::getForce() const
      {
        return m_force;
      }

      float TexturedMeshRenderablePhysic::getMass() const
      {
        return m_mass;
      }



      void TexturedMeshRenderablePhysic::setPosition(const glm::vec3 &pos)
      {
        if(pos[2] > m_position[2] || pos[2] < 2.0){
          m_position = glm::vec3(pos[0],pos[1], m_position[2]);
        }else{
          m_position=pos;
        }
      }

      void TexturedMeshRenderablePhysic::setVelocity(const glm::vec3 &vel)
      {
        m_velocity = vel;
      }

      void TexturedMeshRenderablePhysic::setForce(const glm::vec3 &force)
      {
        m_force = force;
      }

      void TexturedMeshRenderablePhysic::incrPosition(const glm::vec3 &pos)
      {
        m_position += pos;
      }

      void TexturedMeshRenderablePhysic::incrVelocity(const glm::vec3 &vel)
      {
        m_velocity += vel;
      }

      void TexturedMeshRenderablePhysic::incrForce(const glm::vec3& force)
      {
        m_force +=force;
      }

      void TexturedMeshRenderablePhysic::restart()
      {
        m_position = m_initialPosition;
        m_velocity = m_initialVelocity;
      }
      collisionSphere* TexturedMeshRenderablePhysic::computeCollisionSphere()
      {
        collisionSphere *cs = new collisionSphere(this->getPosition(),this->getRadius());
        return cs;
      }
      bool TexturedMeshRenderablePhysic::isFixed() const
      {
        return this->Fixed;
      }

      std::ostream& operator<<(std::ostream& os, const TexturedMeshRenderablePhysicPtr& p)
      {
        const glm::vec3& x = p->getPosition();
        const glm::vec3& v = p->getVelocity();
        const glm::vec3& f =p->getForce();


        os << "pos (" << x[0] << ", " << x[1] << ", " << x[2] << ")";
        os << " ; ";
        os << "vel (" << v[0] << ", " << v[1] << ", " << v[2] << ")";
        os << " ; ";
        os << "force (" << f[0] << ", " << f[1] << ", " << f[2] << ")";

        return os;
      }
