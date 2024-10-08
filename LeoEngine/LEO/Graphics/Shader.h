#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <string>
#include <unordered_map>
#include "../Utilities/Types.h"

namespace LEO
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& filepath);
		ShaderProgram(const char* vertSrc, const char* fragSrc);
		ShaderProgram(const char* vertSrc, const char* geoSrc, const char* fragSrc);

		ShaderProgram(const ShaderProgram& other) = delete;
		ShaderProgram& operator=(const ShaderProgram& other) = delete;

		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		~ShaderProgram();
	public:
		bool Reload(const char* vertexSrc, const char* geoSrc, const char* fragSrc);
		bool Reload(const char* vertexSrc, const char* fragSrc);
		bool Reload(const std::string& filepath);
		bool Reload();
	public:
		void Bind() const;
		void UnBind() const;
		bool IsValid() const;
	public:
		bool SetUniform(const std::string& name, float num) const;
		bool SetUniform(const std::string& name, glm::vec2 a) const;
		bool SetUniform(const std::string& name, glm::vec3 a) const;
		bool SetUniform(const std::string& name, glm::vec4 a) const;
		bool SetUniform(const std::string& name, int i) const;
		bool SetUniform(const std::string& name, glm::ivec3 a) const;
		bool SetUniform(const std::string& name, glm::uvec3 a) const;
		bool SetUniform(const std::string& name, unsigned int i) const;
		bool SetUniform(const std::string& name, const glm::mat4& mat) const;
		bool SetUniform(const std::string& name, const glm::mat3& mat) const;
	public:
		bool SetUniform(const std::string& name, const std::vector<glm::vec2>& vec_arr, u32 size);
		bool SetUniform(const std::string& name, const std::vector<glm::vec3>& vec_arr, u32 size);
	private:
		i32 GetLocation(const std::string& name) const;
	private:
		u32 m_program_id;
		std::string m_filepath;
		mutable std::unordered_map<std::string, i32> m_uniforms; // storing uniforms locations
	private:
		ShaderProgram() = default;
		friend class AssetManager;
	};
}