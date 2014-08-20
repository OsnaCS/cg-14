#pragma once

#include "lumina/lumina.hpp"
#include "lumina/input/InputEvent.hpp"
#include <algorithm> 

using namespace lum;

class Camera
{
 struct Movement {
     bool wPressed;
     bool sPressed;
     bool aPressed;
     bool dPressed;
     bool SpacePressed;
     bool CtrlPressed;
     bool ShiftPressed;

 };

public:

	Camera(Window& window);

    lumina::EventResult processEvent( const lumina::InputEvent& e, lumina::Window& win );

    lumina::Mat4f get_matrix();

    lumina::Mat4f get_ProjectionMatrix();

    void updateFromPlayer(lumina::Vec3f pos, lumina::Vec3f dir);

    void update();


    lumina::Vec3f get_position(){
			return m_position;
    }

   lumina::Vec3f get_direction(){
			return m_direction;
    }


    void setPosition(lumina::Vec3f position) {
		m_position = position;
	}

    lumina::Vec3f getPosition() {
		return m_position;
	}
    void set_movingspeed(float movingspeed) {
        m_movingspeed = movingspeed;
    }

    float getViewAngle() {
        return m_ViewAngle;
    }

    float getBackPlaneDistance() {
        return m_backPlaneDistance;
    }

    lumina::Vec3f getViewUp() {
        return m_up;
    }

    float getScreenRatio() {
        return m_screenRatio;
    }

    Window& getWindow() {
        return m_window;
    }

private:
    void move_left();
    void move_right();
    void move_forward();
    void move_backward();
    void move_up();
    void move_down();
    void turn_side(float deltaX);
    void turn_upDown(float deltaY);
    void reset_camera();
  
    lumina::Vec3f m_position;
    lumina::Vec3f m_direction;
    lumina::Vec3f m_up;
    float m_movingspeed;
    bool m_mouseCaptured;
    Movement m_movedKeys;
    float m_ViewAngle;
    float m_backPlaneDistance;
    float m_screenRatio;
    Window& m_window;
};
