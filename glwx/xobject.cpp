//
// Created by xvzezi on 2019/4/22.
//

#include "xobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

xobject::xobject() {
    model_matrix = glm::mat4(1.0f);
    vertices_amount = elements_amount = 0;
    raw_data =nullptr;
    raw_elements = nullptr;
    texture = nullptr;
    shader = nullptr;
    use_elements = false;
    use_texture = false;
    renderOrNot = true;
    DATA_MODE = GL_STATIC_DRAW;
    DRAW_TYPE = GL_TRIANGLES;
    bounding_ball = glm::vec3(0, 0, 0);
    bounding_radius = 0.0f;
    use_bounding_ball = false;
    cur_bounding_ball = bounding_ball;
    cur_bounding_radius = bounding_radius;
    this->alloc_from_gl();
}

xobject::xobject(const std::string &name)
    :xobject()
{
    this->name = name;
}

xobject::xobject(const xobject &tar) {
    // non-ptr
    model_matrix = tar.model_matrix;
    vertices_amount = 0;
    elements_amount = 0;
    raw_data =nullptr;
    raw_elements = nullptr;
    DATA_MODE = tar.DATA_MODE;
    DRAW_TYPE = tar.DRAW_TYPE;
    bounding_ball = tar.bounding_ball;
    bounding_radius = tar.bounding_radius;
    use_elements = tar.use_elements;
    use_texture = tar.use_texture;
    renderOrNot = tar.renderOrNot;
    texture = tar.texture;
    shader = tar.shader;
    use_bounding_ball = tar.use_bounding_ball;
    cur_bounding_radius = tar.cur_bounding_radius;
    cur_bounding_ball = tar.cur_bounding_ball;

    // update drawing buffer
    alloc_from_gl();
    begin(tar.vertices_amount);
    memcpy(raw_data, tar.raw_data, vertices_amount * sizeof(float) * unit_len);
    end();
    begin_elements(tar.elements_amount);
    memcpy(raw_elements, tar.raw_elements, elements_amount * sizeof(unsigned int));
    end_elements();
}

xobject::xobject(const std::string &name, const xobject &tar)
    :xobject(tar)
{
    this->name = name;
}

void xobject::begin(int amount) {
    if(amount == vertices_amount) return; // nothing changed
    // clear the current data
    if(raw_data) delete[] raw_data;

    // create given size
    vertices_amount = amount;
    raw_data = new float[amount * unit_len];
    // the buffer things will be given to end()
    glBindVertexArray(VA_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(float) * vertices_amount * unit_len,
            raw_data, DATA_MODE);
}

void xobject::set_vertex(int index, float x, float y, float z) {
    if(index >= vertices_amount || index < 0) return;
    int t_index = index * 8 + loc_offset;
    raw_data[t_index] = x;
    raw_data[t_index + 1] = y;
    raw_data[t_index + 2] = z;
}

void xobject::set_vertices(float *vertices, int firsts) {
    if(!vertices) return;
    set_raw_data_from_array(vertices, firsts, loc_offset, 3);
}

void xobject::set_color(int index, float r, float g, float b) {
    if(index >= vertices_amount || index < 0) return;
    int t_index = index * 8 + col_offset;
    raw_data[t_index] = r;
    raw_data[t_index + 1] = g;
    raw_data[t_index + 2] = b;
}

void xobject::set_colors(float *colors, int firsts) {
    if(!colors) return;
    set_raw_data_from_array(colors, firsts, col_offset, 3);
}

void xobject::set_colors(float r, float g, float b, int firsts) {
    if(firsts < 0 || firsts > vertices_amount)
        firsts = vertices_amount;

    for(unsigned i = 0; i < firsts; i++)
    {
        unsigned base_i = i * unit_len + col_offset;
        raw_data[base_i] = r;
        raw_data[base_i + 1] = g;
        raw_data[base_i + 2] = b;
    }
}

void xobject::set_uv(int index, float u, float v) {
    if(index >= vertices_amount || index < 0) return;
    int t_index = index * 8 + tex_offset;
    raw_data[t_index] = u;
    raw_data[t_index + 1] = v;
}

void xobject::set_uvs(float *uvs, int firsts) {
    if(!uvs) return;
    set_raw_data_from_array(uvs, firsts, tex_offset, 2);
}

void xobject::begin_elements(int amount) {
    if(amount == elements_amount) return;   // nothing changed
    // clear the current data
    if(raw_elements) delete[] raw_elements;

    // create as given size
    elements_amount = amount;
    raw_elements = new unsigned int[amount];
    // buffer the data
    glBindVertexArray(VA_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * elements_amount,
            raw_elements, DATA_MODE);
}

void xobject::set_element(int index, unsigned int v_index) {
    if(index >= elements_amount || index < 0) return;
    raw_elements[index] = v_index;
}

void xobject::set_elements(unsigned int *v_indices, int firsts) {
    if(firsts < 0 || firsts > elements_amount)
        firsts = elements_amount;

    for(unsigned i = 0; i < firsts; i++)
    {
        raw_elements[i] = v_indices[i];
    }
}

void xobject::end_elements() {
    glBindVertexArray(VA_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB_ID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
            sizeof(unsigned int) * elements_amount, raw_elements);
}

void xobject::end() {
    glBindVertexArray(VA_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
            sizeof(float) * vertices_amount * unit_len, raw_data);
}

/*******************************
 * Draw Settings
 */

void xobject::render_by_elements(bool rb_ele) {
    this->use_elements = rb_ele;
}

void xobject::render_by_textures(bool rb_tex) {
    this->use_texture = rb_tex;
}

void xobject::ref_shader(xshader* shader) {
    this->shader = shader;
}

void xobject::ref_texture(xtexture* tex) {
    this->texture = tex;
}

void xobject::render_by_type(GLenum draw_type) {
    this->DRAW_TYPE = draw_type;
}

void xobject::data_stream_mode(GLenum stream_mode) {
    this->DATA_MODE = stream_mode;
}

void xobject::render(bool renderOrNot) {
    this->renderOrNot = renderOrNot;
}

bool xobject::enabled() {
    return renderOrNot;
}

/*******************************
 * Model Matrix Functions
 */
xobject& xobject::scale(float x, float y, float z) {
    model_matrix = glm::scale(model_matrix, glm::vec3(x, y, z));
    return *this;
}

xobject& xobject::rotate(float radian, float x, float y, float z) {
    model_matrix = glm::rotate(model_matrix, radian, glm::vec3(x, y, z));
    return *this;
}

xobject& xobject::translate(float x, float y, float z) {
    model_matrix = glm::translate(model_matrix, glm::vec3(x, y, z));
    return *this;
}

xobject& xobject::identity() {
    model_matrix = glm::mat4(1.0f);
    return *this;
}

/*******************************
 * Render Functions
 */
void xobject::render(glm::mat4 &global_matrix) {
    if(!renderOrNot) return; // disabled
    // prepare
    if(!shader)
    {
        std::cout << "QX::OBJECT::RENDER::SHADER INVALID" << " - Name: " << name << std::endl;
        return;
    }
    shader->use();
    if(texture)
    {
        shader->set_bool("useTexture", use_texture);
        if(use_texture)
            texture->bind_texture(shader, GL_TEXTURE0);
    }
    // matrix loading
    shader->set_mat4("viewTransform", global_matrix);
    shader->set_mat4("modelTransform", model_matrix);
    // draw
    glBindVertexArray(VA_ID);
    if(use_elements)
        glDrawElements(DRAW_TYPE, elements_amount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(DRAW_TYPE, 0, vertices_amount);
    glBindVertexArray(0);   // unbind
}

/*******************************
 * Mouse Interactions Functions
 */
void xobject::mouse_select_callback(int button) {

}

void xobject::mouse_release_callback(int button) {

}

void xobject::mouse_drag_callback(int button, float dx, float dy, float dz) {
    static float x = 0;
    static float y = 0;
    static float z = 0;
    x += dx; y += dy; z += dz;
    identity().translate(x, y, z);
}

void xobject::set_bounding_ball(float x, float y, float z, float radius) {
    bounding_ball = glm::vec3(x, y, z);
    bounding_radius = radius;
    use_bounding_ball = true;
}

void xobject::update_bounding_ball() {
    if(!use_bounding_ball) return;
    cur_bounding_ball = model_matrix * glm::vec4(bounding_ball, 1);  // the new origin point
    glm::vec3 bb0 = bounding_ball;
    bb0.x += bounding_radius;
    bb0 = model_matrix * glm::vec4(bb0, 1);
    cur_bounding_radius = glm::length(bb0 - cur_bounding_ball);
}

/*******************************
 * Helper Functions
 */

void xobject::alloc_from_gl() {
    static int unit_len = 8;
    static int loc_offset = 0;
    static int col_offset = 3;
    static int tex_offset = 6;
    glGenBuffers(1, &VB_ID);
    glGenBuffers(1, &EB_ID);
    glGenVertexArrays(1, &VA_ID);
    // binding
    glBindVertexArray(VA_ID);
    glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EB_ID);
    // set the pointer attribute
    // location
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, unit_len*sizeof(float), (void*)(loc_offset * sizeof(float)));
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, unit_len*sizeof(float), (void*)(col_offset * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Text
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, unit_len*sizeof(float), (void*)(tex_offset * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void xobject::set_raw_data_from_array(float *data, int amount, int offset, int dim) {
    if(amount < 0 || amount > vertices_amount)
        amount = vertices_amount;

    for(unsigned i = 0; i < amount; i++)
    {
        unsigned base_i = i * unit_len + offset;
        unsigned vi = i * dim;
        for(int j = 0; j < dim; j++)
        {
            raw_data[base_i] = data[vi];
            base_i++;vi++;
        }
    }
}

int xobject::unit_len = 8;
int xobject::loc_offset = 0;
int xobject::col_offset = 3;
int xobject::tex_offset = 6;

bool xobject::bounding_ball_enabled() {
    return use_bounding_ball;
}

glm::vec3 &xobject::get_cur_bounding_ball() {
    return this->cur_bounding_ball;
}

float xobject::get_cur_bounding_radius() {
    return this->cur_bounding_radius;
}

xobject::~xobject() {
    if(raw_data) delete[] raw_data;
    if(raw_elements) delete[] raw_elements;
}
