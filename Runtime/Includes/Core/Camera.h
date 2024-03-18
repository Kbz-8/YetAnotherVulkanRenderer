#ifndef __YAVR_CORE_CAMERA__
#define __YAVR_CORE_CAMERA__

#include <PreCompiled.h>

namespace Yavr
{
	class Camera
	{
		public:
			Camera(double x, double y, double z, float fov = 90.0f);

			void OnUpdate(float aspect, const class Inputs& inputs);

			inline const glm::mat4& GetProjectionMat() const noexcept { return m_proj; }
			inline const glm::mat4& GetViewMat() const noexcept { return m_view; }

			~Camera() = default;

		private:
			void UpdateView();

		private:
			glm::mat4 m_view = glm::mat4(1.0f);
			glm::mat4 m_proj = glm::mat4(1.0f);

			const glm::vec3 m_up;
			glm::vec3 m_position;
			glm::vec3 m_left;
			glm::vec3 m_forward;
			glm::vec3 m_target;
			glm::vec3 m_direction;
			glm::vec3 m_mov;

			double m_theta = 0.0;
			double m_phi = 0.0;

			float m_speed = 0.02f;
			const float m_sensivity = 0.7f;
			float m_fov = 90.0f;

			bool m_is_mouse_grabed = true;
	};
}

#endif
