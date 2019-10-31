//
// Created by xvzezi on 2019/4/22.
//

#ifndef OPENGL_WRAP_X_XCAMERA_H
#define OPENGL_WRAP_X_XCAMERA_H

// glm matrix
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class xcamera
{
public:
    /**
     * Construct Camera with given params
     * @param loc
     * @param front
     * @param up
     */
    xcamera(glm::vec3 loc, glm::vec3 front, glm::vec3 up);
    xcamera();

    /**
     *
     * @param loc
     * @param front
     * @param up
     */
    void set_camera(glm::vec3 loc, glm::vec3 front, glm::vec3 up);

    /**
     * Update the Projection Matrix
     * @param rd_fov
     * @param width
     * @param height
     * @param near_pl
     * @param far_pl
     */
    void set_projection(float rd_fov, float width, float height, float near_pl, float far_pl);

    /**
     * Inform camera about the view port changing
     * @param sx
     * @param sy
     * @param width
     * @param height
     */
    void set_viewport(float sx, float sy, float width, float height);

    /**
     * Matrix of current camera
     * @return
     */
    glm::mat4& matrix();

    /**
     * Inverse of the current camera
     * @return
     */
    glm::mat4& inverse();

    /**
     * from Window coords to 3D world coords
     * @param winX
     * @param winY
     * @param depth
     * @return (x, y, z)
     */
    glm::vec3 unproject(float winX, float winY, float depth);

    /**
     * from 3D world coords to Window coords
     * @param coords
     * @return (x, y, depth)
     */
    glm::vec3 project(glm::vec3& coords);


    /**
     * Basic Attribute Getter
     * @return vector
     */
    glm::vec3& location();
    glm::vec3& direction();
    glm::vec3& up();



private:
    // helper functions
    void update_matrix();

    // attrs
    bool updated;
    glm::vec3 vec_loc, vec_font, vec_up;
    glm::mat4 m_view, m_projection;
    glm::mat4 m_combine, m_inv;
    glm::vec4 viewport;
};

#endif //OPENGL_WRAP_X_XCAMERA_H
