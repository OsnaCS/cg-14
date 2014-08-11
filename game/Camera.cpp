#include "Camera.hpp"

using namespace lumina;
using namespace std;

Camera::Camera()
{
	m_position = Vec3f(0.0f, 0.0f, 0.0f);
	m_direction = Vec3f(-1.0f, 0.0f, 0.0f);

	get_matrix(){

	};
}



