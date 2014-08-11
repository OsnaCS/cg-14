#include "Camera.hpp"
#include "lumina/lumina.hpp"

using namespace lumina;
using namespace std;

Camera::Camera()
{
	m_position = Vec3<float>(0.0f, 0.0f, 0.0f);
	m_direction = Vec3<float>(-1.0f, 0.0f, 0.0f);
	m_up = Vec3<float>(0.0f, 1.0f, 0.0f);
}

Mat4<float> Camera::get_matrix(){
		return viewMatrix(m_position,m_direction,m_up);
}

/*
void Camera::setPosition(Vec3<float> point){
	m_position = point;
};
*/



