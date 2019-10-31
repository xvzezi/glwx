//
// Created by xvzezi on 2019/4/27.
//

#include <xworld.h>
#include <xline.h>
#include "prob1_world.h"

prob1_world::prob1_world() : xworld("Problem 1", 1600, 800){
    // 1. prepare OPEN GL
    add_shader(new xshader("basic",  "../shader/basic.vert", "../shader/basic.frag"));
    point = new xparticle(0, 1, 0);
    point->ref_shader(shaders()[0]);
    point->set_point_size(10);
    xobject* line = get_circle();
    line->ref_shader(shaders()[0]);
    add_object(line);
    add_object(point);

    // 2. prepare SOLVER
    solver = prob1_solver();

    // 3. other settings
    camera.set_camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    timer = glfwGetTime();
}

xobject *prob1_world::get_circle() {
    static float slices = 50;
    static float round = 360;
    float step = round / slices;
    float up_bound = round + step;
    xline* line = new xline();
    line->begin(slices + 1);
    int counter = 0;
    for(float ptr = 0; ptr < up_bound; ptr += step)
    {
        float rad = glm::radians(ptr);
        line->set_vertex(counter, cosf(rad), sinf(rad), 0);
        counter++;
        if(counter > slices) break;
    }
    line->set_colors(1, 1, 1);
    line->end();
    return line;
}

void prob1_world::try_solve() {
    float cur = glfwGetTime();
    solver.solve_next(cur - timer);
//    solver.solve_next(0.005f);
    timer = cur;
    solver.get_location(0, point->x, point->y, point->z);
}

void prob1_world::render_gui() {
    if(auto_play)
        try_solve();
    else if(go_a_step)
    {
        try_solve();
        go_a_step = false;
    }
    else
        timer = glfwGetTime();
}

void prob1_world::input_processor() {
    xworld::input_processor();
    if(glfwGetKey(main_window, GLFW_KEY_N) == GLFW_PRESS)
        go_a_step = true;
    if(glfwGetKey(main_window, GLFW_KEY_C) == GLFW_PRESS)
        auto_play = true;
    if(glfwGetKey(main_window, GLFW_KEY_S) == GLFW_PRESS)
        auto_play = false;
}
