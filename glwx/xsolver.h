//
// Created by xvzezi on 2019/4/27.
//

#ifndef OPENGL_WRAP_X_XSOLVER_H
#define OPENGL_WRAP_X_XSOLVER_H

class xsolver
{
public:
    virtual void init_solver(int amount) = 0;
    virtual void get_location(int index, float& x, float& y, float& z) = 0;
    virtual void get_velocity(int index, float& vx, float& vy, float& vz) = 0;
    virtual void set_location(int index, float x, float y, float z) = 0;
    virtual void set_velocity(int index, float vx, float vy, float vz) = 0;
    virtual void solve_next() = 0;
};

#endif //OPENGL_WRAP_X_XSOLVER_H
