#pragma once

#include "glm/glm.hpp"

#include <memory>

namespace zeus
{
	class Shader
	{
	protected:
		uint32_t m_ShaderID = 0;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocationCache;

		virtual std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source) = 0;
		virtual std::string ReadFile(const std::string& filepath) = 0;
		virtual void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources) = 0;
		virtual int GetUniformLocation(const std::string& name) = 0;

	public:
		static std::shared_ptr<Shader> CreateShader(const std::string& name, const std::string& vertex, const std::string& fragment, bool fromFile = true);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Delete() const = 0;

		const std::string& GetName() const { return m_Name; }
		uint32_t GetShaderID() const { return m_ShaderID; }

		// Upload data to GPU
		virtual void UploadUniformInt(const std::string& name, int val) = 0;
		virtual void UploadUniformIntArray(const std::string& name, int* vals, uint32_t count) = 0;
		virtual void UploadUniformFloat(const std::string& name, float val) = 0;
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& vec) = 0;
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& vec) = 0;
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& vec) = 0;
		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& mat) = 0;
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& mat) = 0;
	};
}
