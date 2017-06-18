#ifndef FORCE_FIELD_HPP
#define FORCE_FIELD_HPP

#include <memory>

/**@brief Force field interface.
 *
 * Define an interface for a force field. A force field applies forces
 * to a set of handled meshes. Those meshes are stored in derived classes.
 */
class ForceField
{
public:
    ForceField();
    virtual ~ForceField();

    /**@brief Add a force to meshes.
     *
     * Add a force to the meshes influenced by this force field.
     */
    void addForce();

private:
    /**@brief Add force implementation.
     *
     * The actual implementation to add force to the meshes.
     * This should be implemented in derived classes.
     */
    virtual void do_addForce() = 0;
};

typedef std::shared_ptr<ForceField> ForceFieldPtr;

#endif //FORCE_FIELD
