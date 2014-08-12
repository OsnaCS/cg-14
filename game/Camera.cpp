#include "Camera.hpp"
#include "lumina/lumina.hpp"

using namespace std;

Camera::Camera()
{
	m_position = Vec3<float>(0.0f, 0.0f, 10.0f);
	m_direction = Vec3<float>(0.0f, 0.0f, -1.0f);
	m_up = cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
}

Mat4<float> Camera::get_matrix(){
		return viewMatrix(m_position,m_direction,m_up);
}

Mat4<float> Camera::get_ProjectionMatrix(){
		return projectionMatrix(0.785f, 1.0f, 0.0001f, 1000000.0f);
};
