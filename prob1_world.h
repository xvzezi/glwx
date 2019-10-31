//
// Created by xvzezi on 2019/4/27.
//

#ifndef OPENGL_WRAP_X_PROB1_WORLD_H
#define OPENGL_WRAP_X_PROB1_WORLD_H


#include <xworld.h>
#include "xparticle.h"
#include "prob1_solver.h"

class prob1_world : public xworld{
public:
    /**
     * Problem 1: A Bead on a Wire
     */
    prob1_world();

    /**
     * Gui & Solvers
     */
    void render_gui() override;
    void input_processor() override ;

private:
    xparticle* point;
    prob1_solver solver;

    xobject* get_circle();
    void try_solve();
    float timer = 0;

    bool auto_play = false;
    bool go_a_step = false;
};


#endif //OPENGL_WRAP_X_PROB1_WORLD_H
