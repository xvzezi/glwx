//
// Created by xvzezi on 2019/4/28.
//

#include "xline.h"

xline::xline() {

}

void xline::render(glm::mat4 &global_matrix) {
    DRAW_TYPE = GL_LINE_STRIP;
    glLineWidth(line_width);
    xobject::render(global_matrix);
}

void xline::set_line_width(float l_width) {
    if(l_width < 1) return;
    line_width = l_width;
}

void xline::mouse_drag_callback(int button, float dx, float dy, float dz) {
    return;
}
