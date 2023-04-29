#include "corepch.h"
#include "opengl_shader.h"

#include "glad/gl.h"
#include "glm/gtc/type_ptr.hpp"
#include "util/logger.h"

namespace zeus
{
	static uint32_t ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		else if (type == "pixel" || type == "fragment")
			return GL_FRAGMENT_SHADER;

		LOG_ENGINE(Error, "Runtime Error: Unknown shader type (Shader)");
		return 0;
	}

	static std::string ShaderTypeFromEnum(GLenum type)
	{
		if (type == GL_VERTEX_SHADER)
			return "vertex";
		else if (type == GL_FRAGMENT_SHADER)
			return "fragment";

		return "unknown";
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment, bool fromFile)
	{
		m_Name = name;

		std::unordered_map<uint32_t, std::string> sources;
		if (fromFile)
		{
			sources[GL_VERTEX_SHADER] = ReadFile(vertex);
			sources[GL_FRAGMENT_SHADER] = ReadFile(fragment);
		}
		else
		{
			sources[GL_VERTEX_SHADER] = vertex;
			sources[GL_FRAGMENT_SHADER] = fragment;
		}

		Compile(sources);
	}

	void OpenGLShader::Compile(const std::unordered_map<uint32_t, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		if (shaderSources.size() > 2)
		{
			LOG_ENGINE(Error, "Runtime Error: This only supports 2 shaders at a time");
		}

		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDidx = 0;
		for (auto& key_value : shaderSources)
		{
			GLenum type = key_value.first;
			const std::string& source = key_value.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCstr = source.c_str();
			glShaderSource(shader, 1, &sourceCstr, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				// TODO: logger needed
				//LOG(O_ERROR, "%s", infoLog.data());
				std::string errorMsg = "Runtime Error: " + ShaderTypeFromEnum(type) + " shader compilation failure!";
				LOG_ENGINE(Error, errorMsg.c_str());
				break; // if not success continue the program
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDidx++] = shader;
		}

		if (shaderSources.size() >= 2)
		{
			glLinkProgram(program);
		}
		else
		{
			std::stringstream ss;
			ss << "Runtime Error: Trying to link shaders with less amount of assets: ";
			if (shaderSources.size())
			{
				for (auto key_value : shaderSources)
				{
					ss << ShaderTypeFromEnum(key_value.first) << ", ";
				}
			}
			else
			{
				ss << "empty";
			}

			LOG_ENGINE(Error, ss.str().c_str());
		}

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			char* message = (char*)malloc(maxLength * sizeof(char));
			glGetProgramInfoLog(program, maxLength, &maxLength, message);

			glDeleteProgram(program);
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// TODO: logger needed
			//LOG(O_ERROR, "%s", message);
			LOG_ENGINE(Error, "Runtime Error: Program linking failure!");
			return; // if not success stop the program
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}

		m_ShaderID = program;
	}

	std::unordered_map<uint32_t, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<uint32_t, std::string> shaderSources;

		const char* token = "#type";
		size_t tokenLength = strlen(token);
		size_t pos = source.find(token, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol != std::string::npos)
			{
				LOG_ENGINE(Error, "Runtime Error: Syntax error with shaders!");
			}

			size_t begin = pos + tokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			if (ShaderTypeFromString(type))
			{
				LOG_ENGINE(Error, "Runtime Error: Invalid shader type spcified!");
			}

			size_t nextLine = source.find_first_not_of("\r\n", eol);
			pos = source.find(token, nextLine);
			shaderSources[ShaderTypeFromString(type)] = source.substr(
				nextLine,
				pos - (nextLine == std::string::npos ? source.size() - 1 : nextLine)
			);
		}

		return shaderSources;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result{ "" };
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			// go to the end of the file from 0
			in.seekg(0, std::ios::end);
			// resize the string as where the file pointer is
			result.resize(in.tellg());
			// go back to the begining to read the file
			in.seekg(0, std::ios::beg);
			// read the file into the string
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			std::string errorMsg = "Runtime Error: Could not load the shader files " + filepath;
			LOG_ENGINE(Error, errorMsg.c_str());
		}

		return result;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ShaderID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::Delete() const
	{
		glDeleteProgram(m_ShaderID);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int val)
	{
		glUniform1i(GetUniformLocation(name), val);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* vals, uint32_t count)
	{
		glUniform1iv(GetUniformLocation(name), count, vals);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float val)
	{
		glUniform1f(GetUniformLocation(name), val);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
	{
		glUniform2f(GetUniformLocation(name), vec.x, vec.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
	{
		glUniform3f(GetUniformLocation(name), vec.x, vec.y, vec.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
	{
		glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& mat)
	{
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
	}

	int OpenGLShader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[name];
		}

		int location = glGetUniformLocation(m_ShaderID, name.c_str());
		m_UniformLocationCache[name] = location;

		return location;
	}
}
