#pragma once

#include "lumina/lumina.hpp"
#include "lumina/input/InputEvent.hpp"
//#include <functional>

using namespace lumina;

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
	void setPosition(Vec3f position) {
		m_position = position;
	}

	Vec3f getPosition() {
		return m_position;
	}

    void set_movingspeed(float movingspeed) {
        m_movingspeed = movingspeed;
    };

    lumina::EventResult processEvent( lumina::InputEvent& e );

private:
    void move_left();
    void move_right();
    void move_forward();
    void move_backward();
    void move_up();
    void move_down();
    void turn_left();
    void turn_right();

	lumina::Vec3<float> m_position;
	lumina::Vec3<float> m_direction;
	lumina::Vec3<float> m_up;
    float m_movingspeed;
};
