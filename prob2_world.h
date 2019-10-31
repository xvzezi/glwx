//
// Created by xvzezi on 2019/4/28.
//

#ifndef OPENGL_WRAP_X_PROB2_WORLD_H
#define OPENGL_WRAP_X_PROB2_WORLD_H


#include <xworld.h>
#include <xparticle.h>
#include <xline.h>
#include "force_particle.h"
#include "prob2_solver.h"
#include "arrow.h"

class prob2_world : public xworld{
public:
    /**
     * Problem 2: Double Pendulum
     */
    prob2_world() : xworld("Problem 2", 1000, 800)
    {
        // 1. prepare OPEN GL
        add_shader(new xshader("basic",  "../shader/basic.vert", "../shader/basic.frag"));
        // a. points
        p0 = new force_particle(1, 1, 0);
        p1 = new force_particle(2, 1, 0);
        p0->ref_shader(shaders()[0]);
        p1->ref_shader(shaders()[0]);
        p0->set_point_size(10);
        p1->set_point_size(10);
        add_object(p0);add_object(p1);
        p0->set_color(1, 1, 0);
        p1->set_color(0, 1, 0);
        p0->end();p1->end();
        // b. line
        line = new xline();
        line->begin(2);
        line->set_vertex(0, 1, 1, 0);
        line->set_vertex(1, 2, 1, 0);
        line->end();
        line->ref_shader(shaders()[0]);
        add_object(line);
        // c. parabola
        xobject* para = get_para();
        para->ref_shader(shaders()[0]);
        add_object(para);
        // d. others
        camera.set_camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
        timer = glfwGetTime();
        // e. forces
        drag_f0 = new xline();
        drag_f0->begin(2);
        drag_f0->set_vertex(0, 0, 0, 0);
        drag_f0->set_vertex(1, 0, 0, 0);
        drag_f0->set_colors(1, 0, 0);
        drag_f0->end();
        drag_f0->ref_shader(shaders()[0]);
        add_object(drag_f0);
        drag_f1 = new xline();
        drag_f1->begin(2);
        drag_f1->set_vertex(0, 0, 0, 0);
        drag_f1->set_vertex(1, 0, 0, 0);
        drag_f1->set_colors(1, 0, 0);
        drag_f1->end();
        drag_f1->ref_shader(shaders()[0]);
        add_object(drag_f1);
        // f. forces
        a0 = new arrow();
        a0->set_color(0, 0, 1);
        a0->ref_shader(shaders()[0]);
        a1 = new arrow();
        a1->set_color(0, 0, 1);
        a1->ref_shader(shaders()[0]);
        add_object(a0);
        add_object(a1);
        a0->scale_factor = 0.05;
        a1->scale_factor = 0.05;

        // 2. solver
        solver = prob2_solver();
        // 3. other settings
        p0->force_scalar = 10;
        p1->force_scalar = 10;
    }

    /**
     * Gui & Solvers
     */
    void render_gui() override
    {
        // 1. do things of solvers
        if(auto_play)
            try_solve();
        else if(go_a_step)
        {
            try_solve();
            go_a_step = false;
        }
        else
            timer = glfwGetTime();


        // 2. render lines & forces
        line->set_vertex(0, p0->x, p0->y, p0->z);
        line->set_vertex(1, p1->x, p1->y, p1->z);
        line->end();
        // a. drag force
        float rx, ry, rz;
        p0->read_force(rx, ry, rz);
        rx /= p0->force_scalar; ry /= p0->force_scalar; rz /= p0->force_scalar;
        drag_f0->set_vertex(0, p0->x, p0->y, p0->z);
        drag_f0->set_vertex(1, p0->x + rx, p0->y + ry, p0->z + rz);
        drag_f0->end();
        p1->read_force(rx, ry, rz);
        rx /= p1->force_scalar; ry /= p1->force_scalar; rz /= p1->force_scalar;
        drag_f1->set_vertex(0, p1->x, p1->y, p1->z);
        drag_f1->set_vertex(1, p1->x + rx, p1->y + ry, p1->z + rz);
        drag_f1->end();
        // b. constraint forces
        solver.get_cforce(0, rx, ry);
        a0->set_arrow(p0->x, p0->y, p0->z, p0->x + rx, p0->y + ry, p0->z);
        solver.get_cforce(1, rx, ry);
        a1->set_arrow(p1->x, p1->y, p1->z, p1->x + rx, p1->y + ry, p1->z);

    }
    void input_processor() override
    {
        xworld::input_processor();
        if(glfwGetKey(main_window, GLFW_KEY_N) == GLFW_PRESS)
            go_a_step = true;
        if(glfwGetKey(main_window, GLFW_KEY_C) == GLFW_PRESS)
            auto_play = true;
        if(glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS)
            auto_play = false;
    }

private:
    xobject* get_para()
    {
        static float left = -1;
        static float right = 1;
        static float slices = 100;
        float step = (right - left) / slices;
        float up_bound = right + step;
        xline* line = new xline();
        line->begin(slices);
        int counter = 0;
        for(float ptr = left; ptr < up_bound; ptr += step)
        {
            line->set_vertex(counter, ptr, ptr * ptr, 0);
            counter++;
            if(counter >= slices) break;
        }
        line->set_colors(1, 1, 1);
        line->end();
        return line;
    }

    void try_solve()
    {
        float cur = glfwGetTime();
        float force_x, force_y, tz;
        p0->read_force(force_x, force_y, tz);
        solver.apply_force(0, force_x, force_y);
        p1->read_force(force_x, force_y, tz);
        solver.apply_force(1, force_x, force_y);
        solver.solve_next(cur - timer);
//        solver.solve_next(0.001);
        timer = cur;
        solver.get_location(0, p0->x, p0->y);
        solver.get_location(1, p1->x, p1->y);
    }

    force_particle* p0, *p1;
    xline* line;
    xline* drag_f0, *drag_f1;
    xline* cons_f0, *cons_f1;
    arrow* a1, *a0;

    prob2_solver solver;

    float timer;
    bool auto_play = false;
    bool go_a_step = false;
};


#endif //OPENGL_WRAP_X_PROB2_WORLD_H
