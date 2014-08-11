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
		lumina::type get_matrix();

private:
	lumina::Vec3f m_position;
	lumina::Vec3f m_direction;
};