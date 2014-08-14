#include "Camera.hpp"
#include "lumina/lumina.hpp"
#include <algorithm> 

using namespace lumina;
using namespace std;

const float BASIC_SPEED = 0.3f; 
const float FAST_SPEED = 3.0f;

Camera::Camera() :
    m_position(Vec3f(0.0f, 80.0f, 0.0f))
    ,m_direction(Vec3f(0.0f, 0.0f, -1.0f))
    ,m_movingspeed(0.3f)
    ,m_mouseCaptured(false)
    ,m_wPressed(false)
    ,m_sPressed(false)
    ,m_aPressed(false)
    ,m_dPressed(false)
    ,m_SpacePressed(false)
    ,m_CtrlPressed(false)
    ,m_ShiftPressed(false)
{
    m_up =  cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
}

Mat4<float> Camera::get_matrix(){
    return viewMatrix(m_position,m_direction,m_up);
}

Mat4<float> Camera::get_ProjectionMatrix(Window& w){
       Vec2i s = w.getSize();
		return projectionMatrix(0.785f, static_cast<float>(s[0]) / s[1], 0.01f, 1000.0f);
};

EventResult Camera::processEvent( InputEvent& e , Window& win)
{
    //Key Pressed and Released
    if(e.type == InputType::KeyPressed || e.type == InputType::KeyReleased){
        bool proccessed = false;
        switch( (KeyCode)(e.keyInput.key))
        {
            case KeyCode::W :
            if(m_wPressed == false){
                m_wPressed = true;
            }else{
                m_wPressed = false;
            }
            proccessed = true;
            break;

            case KeyCode::S :
            if(m_sPressed == false){
                m_sPressed = true;
            }else{
                m_sPressed = false;
            }
            proccessed = true;
            break;

            case KeyCode::A :
            if(m_aPressed == false){
                m_aPressed = true;
            }else{
                m_aPressed = false;
            }
            proccessed = true;
            break;

            case KeyCode::D :
            if(m_dPressed == false){
                m_dPressed = true;
            }else{
                m_dPressed = false;
            }
            proccessed = true;
            break;

            case KeyCode::Space :
            if(m_SpacePressed == false){
                m_SpacePressed = true;
            }else{
                m_SpacePressed = false;
            }
            proccessed = true;
            break;

            case KeyCode::Control :
            if(m_CtrlPressed == false){
                m_CtrlPressed = true;
            }else{
                m_CtrlPressed = false;
            }
            proccessed = true;
            break;
            case KeyCode::Shift :
            if(m_ShiftPressed == false){
                m_ShiftPressed = true;
                m_movingspeed = FAST_SPEED; 
            }else{
                m_ShiftPressed = false;
                m_movingspeed = BASIC_SPEED;
            }
            proccessed = true;
            break;
            default:
            //Do Nothing
              break;
               
        }
    }

    // Mouse handle
    if(e.type == InputType::LMousePressed) {
    	m_mouseCaptured = true;
    	win.setCursorMode(CursorMode::Free);
    }
    if(e.type == InputType::LMouseReleased) {
    	m_mouseCaptured = false;
    	win.setCursorMode(CursorMode::Normal);
    }

    // Mouse
    if (m_mouseCaptured && e.type == InputType::MouseMoveDir)
    {
    	turn_side(-e.mouseInput.x / 300.0f);
    	turn_upDown(-e.mouseInput.y / 50.0f);
    }


    return EventResult::Skipped;
}

void Camera::move_left()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), 1.5708f ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_position.x += m_movingspeed*left_direction.x;
   m_position.z += m_movingspeed*left_direction.z;
}

void Camera::move_right()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), -1.5708f ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_position.x += m_movingspeed*left_direction.x;
   m_position.z += m_movingspeed*left_direction.z;
}

void Camera::move_forward()
{
   m_position.x += m_movingspeed*m_direction.x;
   m_position.z += m_movingspeed*m_direction.z;
   m_position.y += m_movingspeed*m_direction.y;
}


void Camera::move_backward()
{
   m_position.x -= m_movingspeed*m_direction.x;
   m_position.z -= m_movingspeed*m_direction.z;
   m_position.y -= m_movingspeed*m_direction.y;   
}

void Camera::move_up()
{
    m_position.y += m_movingspeed;
}

void Camera::move_down()
{
    m_position.y -= m_movingspeed;
}

// change direction to 9 degree
void Camera::turn_side(float deltaX)
{
    Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), deltaX));
    Vec4f new_direction =  rotationM*Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    float sum_vector = abs(new_direction.x) + abs(new_direction.y) + abs(new_direction.z);
    m_direction.x = new_direction.x / sum_vector;
    m_direction.y = new_direction.y / sum_vector;
    m_direction.z = new_direction.z / sum_vector;
}

void Camera::turn_upDown(float deltaY)
{
    if((m_direction.y < 0.9f && deltaY >0 ) || (m_direction.y > -0.9f && deltaY <0)){
    Vec3f trans_Vec = cross(m_direction, Vec3f(0.f, 1.f, 0.f));
    Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(trans_Vec.x, trans_Vec.y, trans_Vec.z),1.5708f/10* deltaY));

    Vec4f new_direction =  rotationM*Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    float sum_vector = abs(new_direction.x) + abs(new_direction.y) + abs(new_direction.z);

    m_direction.x = new_direction.x / sum_vector;
    m_direction.y = new_direction.y / sum_vector;
    m_direction.z = new_direction.z / sum_vector;
    }
}

void Camera::reset_camera()
{
    m_position = Vec3f(0.0f, 0.0f, 10.0f);
    m_direction = Vec3f(0.0f, 0.0f, -1.0f);
}

void Camera::update()
{
    if(m_wPressed){
        move_forward();
    }else if(m_sPressed){
        move_backward();
    }
    if(m_aPressed){
        move_left();
    }else if(m_dPressed){
        move_right();
    }
    if(m_SpacePressed){
        move_up();
    }else if(m_CtrlPressed){
        move_down();
    }  
}

//Update Camera from position and Direction of the player
void Camera::updateFromPlayer(Vec3f pos, Vec3f dir){
m_position = pos;
m_position.y += 1;
}
