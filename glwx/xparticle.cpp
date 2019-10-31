//
// Created by xvzezi on 2019/4/27.
//

#include "xparticle.h"

xparticle::xparticle(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    is_still = false;
    // all the point will be drawn from the original point
    begin(1);
    set_vertex(0, 0, 0, 0);
    xobject::set_color(0, 1, 1, 1);
    end();
    // bounding box
    xobject::set_bounding_ball(0, 0, 0, 0.1);
}

void xparticle::mouse_drag_callback(int button, float dx, float dy, float dz) {
    if(is_still) return;
    x += dx; y += dy; z += dz;
}

void xparticle::set_still(bool is_still) {
    this->is_still = is_still;
}

void xparticle::render(glm::mat4 &global_matrix) {
    DRAW_TYPE = GL_POINTS;
    glPointSize(point_size);
    identity().translate(x, y, z);
    xobject::render(global_matrix);
}

void xparticle::set_point_size(float _size) {
    if(_size < 1) return;
    point_size = _size;
//    xobject::set_bounding_ball(0, 0, 0, point_size);
}

void xparticle::set_color(float r, float g, float b) {
    xobject::set_color(0, r, g, b);
}
