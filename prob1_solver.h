//
// Created by xvzezi on 2019/4/27.
//

#ifndef OPENGL_WRAP_X_PROB1_SOLVER_H
#define OPENGL_WRAP_X_PROB1_SOLVER_H

#include <xsolver.h>
#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;

class prob1_solver
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    prob1_solver()
    {
        float init_rad = glm::radians(85.0f);
        loc << cosf(init_rad), sinf(init_rad);
        vec << 0, 0;
        force << 0, - mass * gravity;
    }

    void get_location(int index, float &x, float &y, float &z)  {
        x = loc(0);
        y = loc(1);
        z = 0;
    }

    void solve_next(float time_step)  {
        // 1. calculate the true force
        float cons0 = force.dot(loc);
        float cons1 = mass * vec.dot(vec);
        float cons2 = loc.dot(loc);
        float fb = - ks * (loc.dot(loc) - radius) - kd * loc.dot(vec);
        float lam = (-cons0 - cons1 + fb) / cons2;
//        float lam = (-cons0 - cons1) / cons2;
        Vector2f com_force = lam * loc;
        Vector2f acc = (com_force + force) / mass;

        // 2. mid point
        Vector2f mid_loc = loc + time_step * vec / 2;
        Vector2f mid_vec = vec + time_step * acc / 2;
        cons0 = force.dot(mid_loc);
        cons1 = mass * mid_vec.dot(mid_vec);
        cons2 = mid_loc.dot(mid_loc);
        fb = -ks * (mid_loc.dot(mid_loc) - radius) - kd * mid_loc.dot(mid_vec);
        float mid_lam = (-cons0 - cons1 + fb) / cons2;
//        float mid_lam = (-cons0 - cons1) / cons2;
        Vector2f mid_acc = (mid_lam * mid_loc + force) / mass;

        // 2-2. euler
//        Vector2f mid_vec = vec;
//        Vector2f mid_acc = acc;

        // 2. try to cal the answer by euler
        loc = loc + time_step * mid_vec;
        vec = vec + time_step * mid_acc;

        std::cout << "loc:" << loc(0) << ' ' << loc(1) << std::endl;
        std::cout << "vec:" << vec(0) << ' ' << vec(1) << std::endl;
        std::cout << "--------------" << std::endl;
    }

private:
    Vector2f loc;
    Vector2f vec;
    Vector2f force;
    float gravity = 9.8;
    float mass = 1;
    float radius = 1;
    float ks = 1;
    float kd = 1;
};

#endif //OPENGL_WRAP_X_PROB1_SOLVER_H
