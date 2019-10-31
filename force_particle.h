//
// Created by xvzezi on 2019/4/28.
//

#ifndef OPENGL_WRAP_X_FORCE_PARTICLE_H
#define OPENGL_WRAP_X_FORCE_PARTICLE_H

#include <xparticle.h>

class force_particle : public xparticle{
public:
    float force_scalar = 1.0;
    force_particle(float x, float y, float z) : xparticle(x, y, z) {
        fx = fy = fz = 0;
        mx = my = mz = 0;
    }

    void mouse_select_callback(int button) override {
        mx = x;
        my = y;
        mz = z;
    }

    void mouse_release_callback(int button) override {
        fx = fy = fz = 0;
        mx = my = mz = 0;
    }


    void mouse_drag_callback(int button, float dx, float dy, float dz) override {
        mx += dx; my += dy; mz += dz;
        glm::vec3 dif(mx - x, my - y, mz - z);
        dif *= force_scalar;
        fx = dif.x;
        fy = dif.y;
        fz = dif.z;
    }

    void read_force(float& fx, float& fy, float& fz){
        fx = this->fx;
        fy = this->fy;
        fz = this->fz;
    }

private:
    float fx, fy, fz;
    float mx, my, mz;
};


#endif //OPENGL_WRAP_X_FORCE_PARTICLE_H
