#include <Core/Camera.h>
#include <Platform/Inputs.h>

namespace Yavr
{
	glm::vec3 CrossProduct(const glm::vec3& v1, const glm::vec3& v2)
	{
		glm::vec3 res;
		res.x = v1.y * v2.z - v1.z * v2.y;
		res.y = v1.z * v2.x - v1.x * v2.z;
		res.z = v1.x * v2.y - v1.y * v2.x;
		return res;
	}

	Camera::Camera(double x, double y, double z, float fov) : m_up(0, 1, 0), m_fov(fov), m_position(x, y, z)
	{
		UpdateView();
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void Camera::OnUpdate(float aspect, const Inputs& in)
	{
		if(m_is_mouse_grabed)
		{
			m_theta -= in.GetXRel() * m_sensivity;
			m_phi -= in.GetYRel() * m_sensivity;
		}
		if(in.IsKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			m_is_mouse_grabed = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		if(in.IsMouseButtonPressed(SDL_BUTTON_LEFT))
		{
			m_is_mouse_grabed = true;
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		m_mov = glm::vec3(0.0, 0.0, 0.0);

		if(in.IsKeyPressed(SDL_SCANCODE_W) || in.IsKeyPressed(SDL_SCANCODE_UP))
			m_mov -= m_forward;
		if(in.IsKeyPressed(SDL_SCANCODE_S) || in.IsKeyPressed(SDL_SCANCODE_DOWN))
			m_mov += m_forward;
		if(in.IsKeyPressed(SDL_SCANCODE_D) || in.IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_mov -= m_left;
		if(in.IsKeyPressed(SDL_SCANCODE_A) || in.IsKeyPressed(SDL_SCANCODE_LEFT))
			m_mov += m_left;
		if(in.IsKeyPressed(SDL_SCANCODE_LSHIFT) || in.IsKeyPressed(SDL_SCANCODE_RSHIFT))
			m_mov -= m_up;
		if(in.IsKeyPressed(SDL_SCANCODE_SPACE))
			m_mov += m_up;

		m_position += m_mov * m_speed;

		UpdateView();
		m_target = m_position + m_direction;
		m_view = glm::lookAt(m_position, m_target, m_up);
		m_proj = glm::perspective<float>(glm::radians(m_fov), aspect, 0.1f, 1000.0f);
		m_proj[1][1] *= -1;
	}

	void Camera::UpdateView()
	{
		m_phi = m_phi > 89 ? 89 : m_phi;
		m_phi = m_phi < -89 ? -89 : m_phi;

		m_direction.x = cos(m_phi * M_PI / 180) * cos(m_theta * M_PI / 180);
		m_direction.y = sin(m_phi * M_PI / 180);						// Spherical coordinate system
		m_direction.z = cos(m_phi * M_PI / 180) * sin(-m_theta * M_PI / 180);

		m_left = CrossProduct(m_up, m_direction);
		m_left = glm::normalize(m_left);

		m_forward = CrossProduct(m_up, m_left);
		m_forward = glm::normalize(m_forward);
	}
}
