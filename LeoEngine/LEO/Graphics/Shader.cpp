#include <glad/glad.h>
#include "../Utilities/Log.h"
#include "../Utilities/FileUtilities.h"
#include "Shader.h"

u32 CompileShader(const char* source, u32 type);
u32 CreateShaderProgramFromSource(const char* vertSrc, const char* geoSrc, const char* fragSrc);

namespace LEO
{
	ShaderProgram::ShaderProgram(const std::string& filepath)
		:
		m_program_id(0),
		m_filepath(filepath)
	{
		Reload(filepath);
	}

	ShaderProgram::ShaderProgram(const char* vertSrc, const char* fragSrc)
		:
		ShaderProgram(vertSrc, nullptr, fragSrc)
	{
	}

	ShaderProgram::ShaderProgram(const char* vertSrc, const char* geoSrc, const char* fragSrc)
		:
		m_program_id(0)
	{
		Reload(vertSrc, geoSrc, fragSrc);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
		:
		m_program_id(other.m_program_id),
		m_filepath(other.m_filepath),
		m_uniforms(std::move(other.m_uniforms))
	{
		other.m_program_id = 0;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
	{
		glDeleteProgram(m_program_id);

		m_program_id = other.m_program_id;
		m_filepath = other.m_filepath;
		m_uniforms = std::move(other.m_uniforms);

		other.m_program_id = 0;

		return *this;
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_program_id);
	}

	bool ShaderProgram::Reload(const char* vertexSrc, const char* geoSrc, const char* fragSrc)
	{
		u32 new_program = CreateShaderProgramFromSource(vertexSrc, geoSrc, fragSrc);

		if (new_program == 0) // if new program failed to Compiled
		{
			return false; // we do nothing and return false
		}

		// delete old 
		glDeleteProgram(m_program_id);
		m_uniforms.clear();

		// replace
		m_program_id = new_program;

		return true;
	}

	bool ShaderProgram::Reload(const char* vertexSrc, const char* fragSrc)
	{
		return Reload(vertexSrc, nullptr, fragSrc);
	}

	bool ShaderProgram::Reload(const std::string& filepath)
	{
		m_filepath = filepath;
		std::string vert_source = ReadFile(filepath + ".vert");
		std::string geom_source = ReadFile(filepath + ".geom");
		std::string frag_source = ReadFile(filepath + ".frag");

		LEOASSERT(vert_source != "", "Vertex shader missing!");
		LEOASSERT(frag_source != "", "Fragment shader missing!");

		Reload(vert_source.c_str(), ((geom_source == "") ? nullptr : geom_source.c_str()), frag_source.c_str());
	}

	bool ShaderProgram::Reload()
	{
		return Reload(m_filepath);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_program_id);
	}

	void ShaderProgram::UnBind() const
	{
		glUseProgram(0);
	}

	bool ShaderProgram::SetUniform(const std::string& name, float num) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform1f(location, num);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, glm::vec2 a) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform2f(location, a.x, a.y);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, glm::vec3 a) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform3f(location, a.x, a.y, a.z);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, glm::vec4 a) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform4f(location, a.x, a.y, a.z, a.w);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, int i) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform1i(location, i);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, glm::ivec3 a) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform3i(location, a.x, a.y, a.z);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, glm::uvec3 a) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform3ui(location, a.x, a.y, a.z);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, unsigned int i) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniform1ui(location, i);
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, const glm::mat3& mat) const
	{
		Bind();
		i32 location = GetLocation(name);

		if (location != -1)
		{
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
			UnBind();
			return true;
		}

		UnBind();
		return false;
	}

	bool ShaderProgram::SetUniform(const std::string& name, const std::vector<glm::vec2>& vec_arr, u32 size)
	{
		Bind();

		for (u32 i = 0; i < size; i++)
		{
			i32 location = GetLocation(name + "[" + std::to_string(i) + "]");

			if (location == -1) {
				UnBind();
				return false;
			}

			glUniform2f(location, vec_arr[i].x, vec_arr[i].y);
		}

		UnBind();
		return true;
	}

	bool ShaderProgram::SetUniform(const std::string& name, const std::vector<glm::vec3>& vec_arr, u32 size)
	{
		Bind();

		for (u32 i = 0; i < size; i++)
		{
			i32 location = GetLocation(name + "[" + std::to_string(i) + "]");

			if (location == -1) {
				UnBind();
				return false;
			}

			glUniform3f(location, vec_arr[i].x, vec_arr[i].y, vec_arr[i].z);
		}

		UnBind();
		return true;
	}

	i32 ShaderProgram::GetLocation(const std::string& name) const
	{
		if (m_uniforms.find(name) != m_uniforms.end())
		{
			return m_uniforms[name];
		}

		i32 loc;
		loc = glGetUniformLocation(m_program_id, name.c_str());

		if (loc != -1)
		{
			m_uniforms[name] = loc;
		}

		return loc;
	}

	bool ShaderProgram::IsValid() const
	{
		return m_program_id != 0;
	}
}

u32 CompileShader(const char* source, u32 type)
{
	if (source == nullptr) return 0;

	u32 id = glCreateShader(type);

	glShaderSource(id, 1, &source, nullptr);
	glCompileShader(id);

	int resoult;
	glGetShaderiv(id, GL_COMPILE_STATUS, &resoult);
	if (resoult == GL_FALSE)
	{
		//if (type == GL_VERTEX_SHADER)
		//{
		//	LOGERROR("<VERTEX SHADER COMPILE ERROR>");
		//}
		//else if (type == GL_FRAGMENT_SHADER)
		//{
		//	LOGERROR("<FRAGMENT SHADER COMPILE ERROR>");
		//}
		//else if (type == GL_GEOMETRY_SHADER)
		//{
		//	LOGERROR("<GEOMETRY SHADER COMPILE ERROR>");
		//}

		//char msg[512];
		//glGetShaderInfoLog(id, 512, NULL, msg);
		//LOGERROR("error").Note(msg);

		glDeleteShader(id);
		return 0;
	}

	return id;
}

u32 CreateShaderProgramFromSource(const char* vertSrc, const char* geoSrc, const char* fragSrc)
{
	u32 vs = CompileShader(vertSrc, GL_VERTEX_SHADER);
	u32 gs = CompileShader(geoSrc, GL_GEOMETRY_SHADER);
	u32 fs = CompileShader(fragSrc, GL_FRAGMENT_SHADER);

	u32 programid = glCreateProgram();

	if (vs == 0 || fs == 0) return 0;
	if (geoSrc != nullptr && gs == 0) return 0;


	glAttachShader(programid, vs);
	if (gs != 0)
	{
		glAttachShader(programid, gs);
	}
	glAttachShader(programid, fs);

	glLinkProgram(programid);
	glValidateProgram(programid);

	glDeleteShader(vs);
	if (gs != 0)
	{
		glDeleteShader(gs);
	}	
	glDeleteShader(fs);

	return programid;
}
