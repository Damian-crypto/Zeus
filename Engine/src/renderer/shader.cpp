#include "shader.h"

#include "opengl/opengl_shader.h"

namespace zeus
{
    std::shared_ptr<Shader> Shader::CreateShader(const std::string name, const std::string& file)
    {
        return std::make_shared<OpenGLShader>(name, file);
    }

    std::shared_ptr<Shader> Shader::CreateShader(const std::string name, const std::string& vertex, const std::string& fragment)
    {
        return std::make_shared<OpenGLShader>(name, vertex, fragment);
    }
}
