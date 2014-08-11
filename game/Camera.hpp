#pragma once

#include "lumina/lumina.hpp"

class Camera{

public:
	Camera();

		lumina::Vec3f get_position(){
			return m_position;
		}

		lumina::Vec3f get_direction(){
			return m_direction;
		}

	lumina::Mat4<float> get_matrix();
	lumina::Mat4<float> get_ProjectionMatrix();

private:
	lumina::Vec3<float> m_position;
	lumina::Vec3<float> m_direction;
	lumina::Vec3<float> m_up;
};