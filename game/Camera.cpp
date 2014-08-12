#include "Camera.hpp"
#include "lumina/lumina.hpp"

using namespace std;

Camera::Camera()
{
	m_position = Vec3<float>(0.0f, 0.0f, 5.0f);
	m_direction = Vec3<float>(0.0f, 0.0f, -1.0f);
	m_up = cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
}

Mat4<float> Camera::get_matrix(){
		return viewMatrix(m_position,m_direction,m_up);
}

Mat4<float> Camera::get_ProjectionMatrix(){
		return projectionMatrix(22.f * static_cast<float>(M_PI) / 180.f, 4.f / 3.f, 0.1f, 100.f);
};
