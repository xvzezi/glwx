//
// Created by xvzezi on 2019/4/29.
//

#ifndef OPENGL_WRAP_X_ARROW_H
#define OPENGL_WRAP_X_ARROW_H

#include <xobject.h>
#include <xline.h>

class arrow : public xobject {
public:
    float scale_factor = 1.0;
    arrow()
    {
        line = new xline();
        line->begin(2);
        line->set_vertex(0, 0, 0, 0);
        line->set_vertex(1, 0, 0, 0);
        line->end();
        tri = new xobject();
        tri->begin(3);
        tri->set_vertex(0, 0, 0, 0);
        tri->set_vertex(1, 0, 0, 0);
        tri->set_vertex(2, 0, 0, 0);
        tri->end();
    }

    void set_color(float r, float g, float b){
        line->set_colors(r, g, b);
        tri->set_colors(r, g, b);
        line->end();
        tri->end();
    }

    void ref_shader(xshader *shader) override {
        line->ref_shader(shader);
        tri->ref_shader(shader);
    }

    void render(glm::mat4 &global_matrix) override {
        line->render(global_matrix);
        tri->render(global_matrix);
    }

    void set_arrow(float sx, float sy, float sz, float ex, float ey, float ez)
    {
        glm::vec3 sp(sx, sy, sz);
        glm::vec3 ep(ex, ey, ez);
        glm::vec3 delta = ep - sp;
        delta.z = 0;
        // line
        delta = delta * scale_factor;
        ep = sp + delta;
        line->set_vertex(0, sp.x, sp.y, sp.z);
        line->set_vertex(1, ep.x, ep.y, ep.z);
        line->end();
        // triangle
        glm::vec2 ks(1/delta.x, -1/delta.y);
        ks = glm::normalize(ks);
        float len = glm::length(delta);
        ks = ks * len * 0.05f;
        delta = delta * 0.9f;
        glm::vec3 cross = sp + delta;

        tri->set_vertex(0, ep.x, ep.y, ep.z);
        tri->set_vertex(1, cross.x + ks.x, cross.y + ks.y, cross.z);
        tri->set_vertex(2, cross.x - ks.x, cross.y - ks.y, cross.z);
        tri->end();
    }

private:
    xline* line;
    xobject* tri;
};

#endif //OPENGL_WRAP_X_ARROW_H
