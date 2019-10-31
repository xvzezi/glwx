//
// Created by xvzezi on 2019/4/23.
//

#ifndef OPENGL_WRAP_X_XWORLD_H
#define OPENGL_WRAP_X_XWORLD_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "xobject.h"
#include "xshader.h"
#include "xtexture.h"
#include "xcamera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class xworld {
public:
    std::string name;
    static int width, height;
    std::string msg;
    xcamera camera;
    GLFWwindow* main_window;

    /**
     * Construct a World
     * @param name
     * @param width
     * @param height
     */
    xworld(std::string name, int width, int height);

    /**
     * Add targets
     * @param target
     * @return >0 index of the object
     * @return -1 failures
     */
    int add_object(xobject* object);
    int add_texture(xtexture* texture);
    int add_shader(xshader* shader);

    /**
     * Getters
     * @return
     */
    std::vector<xobject*>& objects();
    std::vector<xtexture*>& textures();
    std::vector<xshader*>& shaders();

    /**
     * remove targets
     */
    void rm_objects(int index);
    void rm_textures(int index);
    void rm_shaders(int index);

    /**
     * User defined functions
     */
protected:
    // write your own static method and rewrite it
    virtual void set_frame_buffer_size_callback();
    void set_frame_buffer_size_callback(GLFWframebuffersizefun func);
public:
    virtual void input_processor();
    virtual void render_gui();

    /**
     * Run
     */
    void event_loop();
    bool should_close();
    void exit();
    void error_exit();
    ~xworld();

private:
    // data that ONLY GROW
    std::vector<xobject*> m_objects;
    std::vector<xtexture*> m_textures;
    std::vector<xshader*> m_shaders;
    // deleted target will be record here
    std::vector<unsigned> ep_objects, ep_textures, ep_shaders;


    // render
    void render();

    // helper funcs
    static void fbs_cb(GLFWwindow* window, int width, int height);
    static void error_cb(int error, const char* desc);
    /***************
     * Mouse Interaction
     */
protected:
    int last_left_mouse_state = GLFW_RELEASE;
    glm::vec3 last_left_mouse_inter;
    int last_left_mouse_select = -1;
    void mouse_event_depatcher();
    virtual void mouse_pressed(int button);
    virtual void mouse_released(int button);
    virtual void mouse_held(int button);
    void fetch_mouse_pos(float& x, float& y);
};


#endif //OPENGL_WRAP_X_XWORLD_H
