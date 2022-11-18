#include "corepch.h"
#include "shader.h"

#include "opengl/opengl_shader.h"

namespace zeus
{
    std::shared_ptr<Shader> Shader::CreateShader(const std::string& name, const std::string& vertex, const std::string& fragment, bool fromFile)
    {
        return std::make_shared<OpenGLShader>(name, vertex, fragment, fromFile);
    }
}
