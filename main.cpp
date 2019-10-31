//
// Created by xvzezi on 2019/4/22.
//


#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <xparticle.h>
#include "glwx/xworld.h"
#include "prob1_world.h"
#include "prob2_world.h"

using namespace std;

// system tester
void load_objects(xworld& world);
void basic_test();

/**********************
 * Problem 1: A Bead on A Wire
 */
void a_bead_on_a_wire();


/***********************
 * Problem 2: Double Pendulum
 */
void double_pendulum();

int main()
{
//    basic_test();
    a_bead_on_a_wire();
//    double_pendulum();
    return 0;
}

void load_objects(xworld& world)
{
    xobject* obj = new xobject("test");
    // feed basic data
    obj->begin(4);
    obj->set_vertex(0, 0, 0, 5);
    obj->set_color(0, 0.8, 0.8, 0.8);
    obj->set_vertex(1, 0, 0, -5);
    obj->set_color(1, 0.5, 0.5, 0.5);
    obj->set_vertex(2, 200, 0, -5);
    obj->set_color(2, 0.8, 0.8, 0.8);
    obj->set_vertex(3, 200, 0, 5);
    obj->set_color(3, 0.5, 0.5, 0.5);
    obj->end();
    // feed elements
    obj->begin_elements(6);
    obj->set_element(0, 0);
    obj->set_element(1, 1);
    obj->set_element(2, 3);
    obj->set_element(3, 1);
    obj->set_element(4, 2);
    obj->set_element(5, 3);
    obj->end_elements();
    // other settings
    obj->ref_shader(world.shaders()[0]);
    obj->render_by_elements(true);
    world.add_object(obj);

    // a cube prim
    float cube_vs[] = {
            1, 0, 1,    1, 0, -1,   -1, 0, -1,      -1, 0, 1,
            1, 2, 1,    1, 2, -1,   -1, 2, -1,      -1, 2, 1
    };
    float cube_cols[] = {
            1, 0, 0,    1, 0, 0,    1, 0, 0,        1, 0, 0,
            1, 0, 0,    1, 0, 0,    1, 0, 0,        1, 0, 0,
    };
    unsigned int cube_i[] = {
            0, 2, 1,    0, 3, 2,    0, 5, 1,    0, 4, 5,    0, 7, 4,    0, 3, 7,
            6, 4, 7,    6, 5, 4,    6, 7, 3,    6, 3, 2,    6, 2, 1,    6, 1, 5
    };
    xobject* prim = new xobject("prim");
    prim->ref_shader(world.shaders()[0]);
    prim->render_by_elements(true);
    prim->begin(8);
    prim->set_vertices(cube_vs);
    prim->set_colors(cube_cols);
    prim->end();
    prim->begin_elements(36);
    prim->set_elements(cube_i);
    prim->end_elements();
    prim->set_bounding_ball(0, 1, 0, 1);
    world.add_object(prim);

    xobject* copy = new xobject("copy", *prim);
    copy->set_colors(0, 1, 0);
    copy->render_by_type(GL_LINES);
    glLineWidth(2);
    copy->end();
    world.add_object(copy);
    prim->render(false);

    xparticle* p = new xparticle(10, 10, 10);
    p->set_point_size(10);
    p->ref_shader(world.shaders()[0]);
    world.add_object(p);
}

void basic_test()
{
    //    prob1_solver a;
//    a.solve_next();
    xworld world("Hello World!", 1600, 800);
    // set cameras
    world.camera.set_camera(
            glm::vec3(60, 30, 80),
            glm::vec3(0, 0, -1),
            glm::vec3(0, 1, 0));

    // load basic shader
    world.add_shader(new xshader("basic",  "../shader/basic.vert", "../shader/basic.frag"));
    load_objects(world);

    while(!world.should_close())
    {
        world.event_loop();
    }

}

void a_bead_on_a_wire()
{
    prob1_world w;
    while(!w.should_close())
    {
        w.event_loop();
    }
}

void double_pendulum()
{
    prob2_world w;
    while(!w.should_close())
    {
        w.event_loop();
    }

}