#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../Scene/Event.h"

namespace LEO
{
	class Camera
	{
	public:
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up  = glm::vec3(0.0f, 1.0f, 0.0f);
	public:
		void GenarateProjectionMatrix(u32 width, u32 height, f32 fov = glm::radians(45.0f), f32 near = 0.1f, f32 far = 100.0f);
		const glm::mat4& GetProjectionMatrix() const;
	public:
		glm::mat4 GetCameraView() const;
	public:
		void OnEvent(const Event& e);
		void OnEvent(const KeyEvent& e);
		void OnEvent(const MouseEvent& e);
		void OnUpdate(f32 dt);
	private:
		glm::mat4 m_projection = glm::mat4(1.0f);
		f32 m_fov  = glm::radians(45.0f);
		f32 m_near = 0.1f;
		f32 m_far  = 100.0f;
	private:
		bool m_forward  = false;
		bool m_backward = false;
		bool m_left     = false;
		bool m_right    = false;
		bool m_mouse    = false;
	private:
		glm::vec2 m_lastMousePos = glm::vec2(0.0f);
		f32 m_yaw   = -glm::pi<f32>() / 2.0f;
		f32 m_pitch = 0.0f;
		f32 xoffset = 0.0f;
		f32 yoffset = 0.0f;
	};
}