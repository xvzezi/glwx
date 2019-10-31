//
// Created by xvzezi on 2019/4/28.
//

#ifndef OPENGL_WRAP_X_XLINE_H
#define OPENGL_WRAP_X_XLINE_H


#include "xobject.h"

class xline : public xobject{
public:
    /**
     * Construct a line
     */
    xline();

    void render(glm::mat4& global_matrix) override;

    void set_line_width(float l_width);
    void mouse_drag_callback(int button, float dx, float dy, float dz) override;

private:
    float line_width = 1;
};


#endif //OPENGL_WRAP_X_XLINE_H
