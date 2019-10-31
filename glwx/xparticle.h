//
// Created by xvzezi on 2019/4/27.
//

#ifndef OPENGL_WRAP_X_XPARTICLE_H
#define OPENGL_WRAP_X_XPARTICLE_H


#include "xobject.h"

class xparticle : public xobject{
public:
    float x, y, z;

    /**
     * Construct a particle from an object
     */
    xparticle(float x, float y, float z);

    void render(glm::mat4& global_matrix) override;

    void set_still(bool is_still);
    void set_color(float r, float g, float b);
    void set_point_size(float _size);

    void mouse_drag_callback(int button, float dx, float dy, float dz) override;

private:
    bool is_still;
    float point_size = 1;
};


#endif //OPENGL_WRAP_X_XPARTICLE_H
