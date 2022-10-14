#include "renderer/shader.h"

#include <string>
#include <unordered_map>

namespace zeus
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& vertex, const std::string& fragment, bool fromFile = true);

		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);

		void Bind() const override;
		void Unbind() const override;
		void Delete() const override;

		// Upload data to GPU
		void UploadUniformInt(const std::string& name, int val) override;
		void UploadUniformIntArray(const std::string& name, int* vals, uint32_t count) override;
		void UploadUniformFloat(const std::string& name, float val) override;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec) override;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec) override;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec) override;
		void UploadUniformMat3(const std::string& name, const glm::mat3& mat) override;
		void UploadUniformMat4(const std::string& name, const glm::mat4& mat) override;

		int GetUniformLocation(const std::string& name) override;
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source) override;
		std::string ReadFile(const std::string& filepath) override;
	};
}
