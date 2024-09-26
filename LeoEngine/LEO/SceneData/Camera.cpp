#include "Camera.h"

namespace LEO
{
	void Camera::GenarateProjectionMatrix(u32 width, u32 height, f32 fov, f32 near, f32 far)
	{
		m_fov = fov;
		m_near = near;
		m_far = far;

		m_projection = glm::perspective(m_fov, (f32)width / (f32)height, m_near, m_far);
	}

	const glm::mat4& Camera::GetProjectionMatrix() const
	{
		return m_projection;
	}

	glm::mat4 Camera::GetCameraView() const
	{
		return glm::lookAt(pos, pos + dir, up);
	}

	void Camera::OnEvent(const Event& e)
	{
		if (e.GetEventType() == LEO::EventType::KeyEvent)
		{
			OnEvent(EVENT_CAST(e, LEO::KeyEvent));
		}

		if (e.GetEventType() == LEO::EventType::MouseEvent)
		{
			OnEvent(EVENT_CAST(e, LEO::MouseEvent));
		}

		if (e.GetEventType() == LEO::EventType::WindowResizeEvent)
		{
			const LEO::WindowReseizeEvent resize = EVENT_CAST(e, LEO::WindowReseizeEvent);
			GenarateProjectionMatrix(resize.width, resize.height, m_fov, m_near, m_far);
		}
	}

	void Camera::OnEvent(const KeyEvent& e)
	{
		i32 keycode = e.keyCode;
		i32 action  = e.action;

		if (keycode == KEY_W)
		{
			m_forward = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_S)
		{
			m_backward = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_A)
		{
			m_left = !(action == KEY_RELEASE);
		}

		if (keycode == KEY_D)
		{
			m_right = !(action == KEY_RELEASE);
		}
	}

	void Camera::OnEvent(const MouseEvent& e)
	{
		static bool firstMouse = true;
		if (firstMouse) {
			m_lastMousePos = glm::vec2(e.x, e.y);
			firstMouse = false;
		}

		xoffset = e.x - m_lastMousePos.x;
		yoffset = m_lastMousePos.y - e.y;
		m_lastMousePos = glm::vec2(e.x, e.y);

		if (e.keycode == MOUSE_BUTTON_RIGHT)
		{
			m_mouse = !(e.action == KEY_RELEASE);
		}
	}

	void Camera::OnUpdate(f32 dt)
	{
		float move_speed = 10.0f;

		if (m_forward)
		{
			pos = pos + dir * move_speed * dt;
		}
		if (m_backward)
		{
			pos = pos - dir * move_speed * dt;
		}

		if (m_right)
		{
			pos += glm::normalize(glm::cross(dir, up)) * move_speed * dt;
		}
		if (m_left)
		{
			pos -= glm::normalize(glm::cross(dir, up)) * move_speed * dt;
		}

		if (m_mouse)
		{
			f32 sensitivity = 0.016f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			m_yaw += xoffset;
			m_pitch += yoffset;

			if (m_pitch > glm::pi<f32>() / 2.0f)
				m_pitch = glm::pi<f32>() / 2.0f;
			if (m_pitch < -glm::pi<f32>() / 2.0f)
				m_pitch = -glm::pi<f32>() / 2.0f;

			glm::vec3 direction;
			direction.x = glm::cos(m_yaw) * glm::cos(m_pitch);
			direction.y = glm::sin(m_pitch);
			direction.z = glm::sin(m_yaw) * glm::cos(m_pitch);
			dir = glm::normalize(direction);
		}
	}
}