//
// Created by xvzezi on 2019/4/22.
//

#ifndef OPENGL_WRAP_X_XSHADER_H
#define OPENGL_WRAP_X_XSHADER_H

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class xshader {
public:
    std::string name;
    unsigned int ID;

    /**
     * Init the shader
     * @param name
     * @param vertexPath
     * @param fragmentPath
     */
    xshader(const std::string& vertexPath, const std::string& fragmentPath);
    xshader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    // 使用/激活程序
    void use()
    {
        glUseProgram(this->ID);
    }
    // uniform工具函数
    void set_bool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void set_int(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set_float(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set_mat4(const std::string &name, glm::mat4 value) const
    {
        GLint matLoc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(matLoc, 1, GL_FALSE, glm::value_ptr(value));
    }
private:
    void load_shaders(const std::string& vertexPath, const std::string& fragmentPath);
};


#endif //OPENGL_WRAP_X_XSHADER_H
