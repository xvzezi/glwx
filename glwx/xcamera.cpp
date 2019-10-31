//
// Created by xvzezi on 2019/4/22.
//

#include "xcamera.h"

xcamera::xcamera(glm::vec3 loc, glm::vec3 front, glm::vec3 up) {
    // calculate matrix
    set_camera(loc, front, up);
    set_projection(glm::radians(45.0), 100.0, 50.0, 0.1, 100.0);
    set_viewport(0, 0, 100, 50);
}

xcamera::xcamera()
    :xcamera(glm::vec3(0,0,0), glm::vec3(0,0,-1), glm::vec3(0,1,0))
{

}


void xcamera::set_camera(glm::vec3 loc, glm::vec3 front, glm::vec3 up) {
    this->vec_loc = loc;
    this->vec_font = front;
    this->vec_up = up;
    m_view = glm::lookAt(loc, loc + front, up);
    updated = true;
}

void xcamera::set_projection(float rd_fov, float width, float height, float near_pl, float far_pl) {
    m_projection = glm::perspective(rd_fov, width/height, near_pl, far_pl);
    updated = true;
}

void xcamera::set_viewport(float sx, float sy, float width, float height) {
    viewport[0] = sx;viewport[1] = sy;viewport[2] = width;viewport[3] = height;
}

glm::mat4 &xcamera::matrix() {
    if(updated) update_matrix();
    return m_combine;
}

glm::mat4 &xcamera::inverse() {
    if(updated) update_matrix();
    return m_inv;
}

glm::vec3 xcamera::unproject(float winX, float winY, float depth) {
    glm::vec4 tmp = glm::vec4(winX, winY, depth, 1);
    tmp.x = (tmp.x - viewport[0]) / viewport[2];
    tmp.y = (tmp.y - viewport[1]) / viewport[3];
#		if GLM_DEPTH_CLIP_SPACE == GLM_DEPTH_ZERO_TO_ONE
    tmp.x = tmp.x * 2 - 1;
			tmp.y = tmp.y * 2 - 1;
#		else
    tmp = tmp * float(2) - float(1);
#		endif

    glm::vec4 obj = inverse() * tmp;
    obj /= obj.w;

    return glm::vec3(obj);
}

glm::vec3 xcamera::project(glm::vec3 &coords) {
    glm::vec4 result = matrix() * glm::vec4(coords, 1.0);
    result /= result.w;
    return glm::vec3(result);
}

void xcamera::update_matrix() {
    if(!updated) return;
    m_combine = m_projection * m_view;
    m_inv = glm::inverse(m_combine);
    updated = false;
}

glm::vec3 &xcamera::location()  {
    return vec_loc;
}

glm::vec3 &xcamera::direction()  {
    return vec_font;
}

glm::vec3 &xcamera::up()  {
    return vec_up;
}







