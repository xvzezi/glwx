//
// Created by xvzezi on 2019/4/23.
//

#include "xworld.h"
int xworld::width = 0;
int xworld::height = 0;

xworld::xworld(std::string name, int width, int height)
    :name(name)
{
    this->width = width;
    this->height = height;
    glfwSetErrorCallback(this->error_cb);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    main_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (main_window == NULL)
    {
        msg = "QX::WORLD::INIT\tFailed to create GLFW window";
        error_exit();
    }
    glfwMakeContextCurrent(main_window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        msg = "QX::WORLD::INIT\tFailed to initialize GLAD";
        error_exit();
    }

    glViewport(0, 0, width, height);
    camera.set_projection(glm::radians(45.0f), width, height, 0.1f, 100.0f);
    camera.set_viewport(0, 0, width, height);

    // features
    glEnable(GL_DEPTH_TEST);

    // call backs
    set_frame_buffer_size_callback();
}

int xworld::add_object(xobject *object) {
    if(!object) return -1;
    if(ep_objects.size() > 0)
    {
        unsigned loc = ep_objects.back();
        m_objects[loc] = object;
        ep_objects.pop_back();
        return loc;
    }
    else
    {
        unsigned loc = m_objects.size();
        m_objects.push_back(object);
        return loc;
    }
    return -1;
}

int xworld::add_texture(xtexture *texture) {
    if(!texture) return -1;
    unsigned loc = -1;
    if(ep_textures.size() > 0)
    {
        loc = ep_textures.back();
        m_textures[loc] = texture;
        ep_textures.pop_back();
    }
    else
    {
        loc = m_textures.size();
        m_textures.push_back(texture);
    }
    return loc;
}

int xworld::add_shader(xshader *shader) {
    if(!shader) return -1;
    unsigned loc = -1;
    if(ep_shaders.size() > 0)
    {
        loc = ep_shaders.back();
        m_shaders[loc] = shader;
        ep_shaders.pop_back();
    }
    else
    {
        loc = m_shaders.size();
        m_shaders.push_back(shader);
    }
    return loc;
}

void xworld::set_frame_buffer_size_callback() {
    set_frame_buffer_size_callback(this->fbs_cb);
}

void xworld::set_frame_buffer_size_callback(GLFWframebuffersizefun func) {
    glfwSetFramebufferSizeCallback(main_window, func);
}

void xworld::input_processor() {
    // process keys
    if(glfwGetKey(main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(main_window, true);

    // process mouse
    mouse_event_depatcher();
}

void xworld::render_gui() {
    return;
}

void xworld::event_loop() {
    // update camera info
    camera.set_viewport(0, 0, width, height);

    // inputs
    input_processor();

    // render
    render();

    // gui
    render_gui();

    // back pro
    glfwSwapBuffers(main_window);
    glfwPollEvents();
}

bool xworld::should_close() {
    return glfwWindowShouldClose(main_window);
}

void xworld::exit() {
    glfwSetWindowShouldClose(main_window, true);
}

void xworld::error_exit() {
    std::cerr << msg << std::endl;
    delete this;
    ::exit(-1);
}

xworld::~xworld() {
    for(auto ptr : m_objects)
    {
        if(ptr) delete ptr;
    }
    for(auto ptr : m_textures)
    {
        if(ptr) delete ptr;
    }
    for(auto ptr : m_shaders)
    {
        if(ptr) delete ptr;
    }
}

void xworld::render() {
    // settings
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // clear the buffer bits
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render each object
    for(auto ptr : m_objects)
    {
        if(ptr)
        {
            ptr->render(camera.matrix());
        }
    }
}

void xworld::fbs_cb(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    xworld::width = width;
    xworld::height = height;
}

void xworld::error_cb(int error, const char *desc) {
    std::cerr << "GLFW::ERROR::" << error << "::" << desc << std::endl;
    ::exit(-1);
}


/**
 * Mouse Interaction
 */
void xworld::mouse_event_depatcher() {
    // left mouse
    int mouse_state = glfwGetMouseButton(main_window, GLFW_MOUSE_BUTTON_LEFT);
    if(mouse_state == GLFW_PRESS)
    {
        if(last_left_mouse_state == GLFW_PRESS)
            mouse_held(GLFW_MOUSE_BUTTON_LEFT);
        else
            mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    }
    else if (mouse_state == GLFW_RELEASE && last_left_mouse_state == GLFW_PRESS)
        mouse_released(GLFW_MOUSE_BUTTON_LEFT);
    last_left_mouse_state = mouse_state;

    // right mouse
    // mid mouse
}

void xworld::mouse_pressed(int button) {
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // 1. get Mouse position
        float winX, winY;
        fetch_mouse_pos(winX, winY);
        glm::vec3 cur_pos = camera.unproject(winX, winY, 1.0f);
        // 2. get a ray : camera + K * dir
        glm::vec3 dir = cur_pos - camera.location();
        dir = glm::normalize(dir);
        // 3. iterate all the object
        unsigned _count = m_objects.size();
        for(unsigned i = 0; i < _count; i++)
        {
            auto obj = m_objects[i];
            if(!obj) continue;
            if(!obj->bounding_ball_enabled()) continue;
            if(!obj->enabled()) continue;
            obj->update_bounding_ball();
            float surface_const = glm::dot(obj->get_cur_bounding_ball(), dir);
            float diss = glm::dot(camera.location(), dir);
            float t = surface_const - diss;
            glm::vec3 inter = camera.location() + t * dir;
            glm::vec3 dis = obj->get_cur_bounding_ball() - inter;
            float distance = glm::length(dis);
            if(distance < obj->get_cur_bounding_radius())
            {
                // clicked on
                obj->mouse_select_callback(button);
                last_left_mouse_inter = inter;
                last_left_mouse_select = i;
                break;
            }
        }
    }
}

void xworld::mouse_released(int button) {
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(last_left_mouse_select >= 0)
        {
            std::cout << "QX::WORLD::MOUSE::LEFT::RELEASE::id: "
                << last_left_mouse_select << " name: "
                << m_objects[last_left_mouse_select]->name << std::endl;
            m_objects[last_left_mouse_select]->mouse_release_callback(button);
            last_left_mouse_select = -1;
        }
    }
}

void xworld::mouse_held(int button) {
    if(button == GLFW_MOUSE_BUTTON_LEFT){
        if(last_left_mouse_select >= 0)
        {
            // something is selected
            if(!m_objects[last_left_mouse_select] || !m_objects[last_left_mouse_select]->enabled())
            {
                last_left_mouse_select = -1;
                return; // destroyed during motion or disabled
            }
            // try to calculate the motion
            // 1. get data
            float winX, winY;
            fetch_mouse_pos(winX, winY);
            glm::vec3 cur_pos = camera.unproject(winX, winY, 1.0f);
            // 2. calculate
            glm::vec3 dir = glm::normalize(camera.direction());
            float surface_const = glm::dot(last_left_mouse_inter, dir);
            float diss = glm::dot(camera.location(), dir);
            float res_const = surface_const - diss;
            glm::vec3 new_dir = cur_pos - camera.location();
            float t = res_const / glm::dot(new_dir, dir);
            glm::vec3 inter = camera.location() + t * new_dir;
            glm::vec3 delta = inter - last_left_mouse_inter;
            //
            m_objects[last_left_mouse_select]->mouse_drag_callback(button, delta.x, delta.y, delta.z);
            last_left_mouse_inter = inter;
            return;
        }
    }
}

void xworld::fetch_mouse_pos(float &x, float &y) {
    double _x, _y;
    glfwGetCursorPos(main_window, &_x, &_y);
    x = _x;
    y = height - _y - 1;
}

/**
 * Data Getters & deleters
 */
std::vector<xobject *> &xworld::objects() {
    return m_objects;
}

std::vector<xtexture *> &xworld::textures() {
    return m_textures;
}

std::vector<xshader *> &xworld::shaders() {
    return m_shaders;
}

void xworld::rm_objects(int index) {
    if(index < 0 || index >= m_objects.size()) return;
    delete m_objects[index];
    m_objects[index] = nullptr;
    ep_objects.push_back(index);
}

void xworld::rm_textures(int index) {
    if(index < 0 || index >= m_textures.size()) return;
    delete m_textures[index];
    m_textures[index] = nullptr;
    ep_textures.push_back(index);
}

void xworld::rm_shaders(int index) {
    if(index < 0 || index >= m_shaders.size()) return;
    delete m_shaders[index];
    m_shaders[index] = nullptr;
    ep_shaders.push_back(index);
}
