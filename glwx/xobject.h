//
// Created by xvzezi on 2019/4/22.
//

#ifndef OPENGL_WRAP_X_XOBJECT_H
#define OPENGL_WRAP_X_XOBJECT_H


#include <string>
#include "xshader.h"
#include "xtexture.h"

class xobject {
public:
    std::string name;
    unsigned int VB_ID, EB_ID, VA_ID;
    unsigned int& ID = VA_ID;

    /**
     * Init an Object
     * @param name
     */
    xobject();
    xobject(const std::string& name);
    xobject(const xobject& tar);
    xobject(const std::string& name, const xobject& tar);

    /**
     * Draw Functions Suite
     */
    // 1. Begin
    /**
     * When vertices amount changed, Update the buffer
     * @param amount
     */
    void begin(int amount);
    // 2. Set vertices
    void set_vertex(int index, float x, float y, float z);
    void set_vertices(float* vertices, int firsts = -1);

    // 3. set colors
    void set_color(int index, float r, float g, float b);
    void set_colors(float r, float g, float b, int firsts = -1);
    void set_colors(float* colors, int firsts = -1);
    // 4. set textures
    void set_uv(int index, float u, float v);
    void set_uvs(float* uvs, int firsts = -1);
    // 5. set elements
    void begin_elements(int amount);
    void set_element(int index, unsigned int v_index);
    void set_elements(unsigned int* v_indices, int firsts = -1);
    void end_elements();
    //6.
    void end();

    /**
     * Draw Settings
     */
    /* use elements but not raw vertices to do rendering */
    void render_by_elements(bool rb_ele);
    /* use textures to render mixed with colors */
    void render_by_textures(bool rb_tex);

    virtual /* reference to shader/textures needed */
    void ref_shader(xshader* shader);
    void ref_texture(xtexture* tex);
    /* draw hints */
    void render_by_type(GLenum draw_type);
    void data_stream_mode(GLenum stream_mode);

    /**
     * Model Matrix
     */
    xobject& scale(float x, float y, float z);
    xobject& rotate(float radian, float x, float y, float z);
    xobject& translate(float x, float y, float z);
    xobject& identity();

    /**
     * Other Switch & Funcs
     */
    void render(bool renderOrNot);
    bool enabled();

    /**
     * Render
     */
    virtual void render(glm::mat4& global_matrix);

    /**
     * Mouse Interaction
     */
    virtual void mouse_select_callback(int button);
    virtual void mouse_release_callback(int button);
    virtual void mouse_drag_callback(int button, float dx, float dy, float dz);
    void set_bounding_ball(float x, float y, float z, float radius);
    /* Warning: Manually update! Not automated */
    void update_bounding_ball();
    bool bounding_ball_enabled();
    glm::vec3& get_cur_bounding_ball();
    float get_cur_bounding_radius();

    ~xobject();
protected:
    int vertices_amount, elements_amount;
    float* raw_data;
    unsigned int* raw_elements;

    xtexture* texture;
    xshader* shader;

    bool use_elements, use_texture;
    bool renderOrNot;

    glm::mat4 model_matrix;

    GLenum DATA_MODE, DRAW_TYPE;

    glm::vec3 bounding_ball;
    float bounding_radius;
    glm::vec3 cur_bounding_ball;
    float cur_bounding_radius;
    bool use_bounding_ball;

    // helper functions
    void alloc_from_gl();
    void set_raw_data_from_array(float* data, int amount, int offset, int dim);
    static int unit_len;
    static int loc_offset;
    static int col_offset;
    static int tex_offset;
};

#endif //OPENGL_WRAP_X_XOBJECT_H
