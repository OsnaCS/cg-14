#include "Camera.hpp"
#include "lumina/lumina.hpp"
#include <algorithm> 

using namespace lumina;
using namespace std;

const float BASIC_SPEED = 0.3f; 
const float FAST_SPEED = 3.0f;

Camera::Camera(Window& window) :
    m_position(Vec3f(0.0f, 80.0f, 0.0f))
    ,m_direction(Vec3f(0.0f, 0.0f, -1.0f))
    ,m_movingspeed(0.3f)
    ,m_mouseCaptured(false)
    ,m_ViewAngle(0.785f)
    ,m_backPlaneDistance(256.0f)
    ,m_window(window)
{
    m_movedKeys.wPressed = false;
    m_movedKeys.sPressed = false;
    m_movedKeys.aPressed = false;
    m_movedKeys.dPressed = false;
    m_movedKeys.SpacePressed = false;
    m_movedKeys.CtrlPressed = false;
    m_movedKeys.ShiftPressed = false;
    m_up =  cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
    Vec2i s = m_window.getSize();
    m_screenRatio = static_cast<float>(s[0]) / s[1];
}

Mat4f Camera::get_matrix()
{
    return viewMatrix(m_position,m_direction,m_up);
}

Mat4f Camera::get_ProjectionMatrix()
{
    return projectionMatrix(m_ViewAngle, m_screenRatio, 0.01f, m_backPlaneDistance);
};

EventResult Camera::processEvent( const InputEvent& e, Window& win)
{

    //Key Pressed and Released
    if(e.type == InputType::KeyPressed || e.type == InputType::KeyReleased){

        switch( (KeyCode)(e.keyInput.key))
        {
            case KeyCode::W :
            if(m_movedKeys.wPressed == false){
                m_movedKeys.wPressed = true;
            }else{
                m_movedKeys.wPressed = false;
            }

            break;

            case KeyCode::S :
            if(m_movedKeys.sPressed == false){
                m_movedKeys.sPressed = true;
            }else{
                m_movedKeys.sPressed = false;
            }

            break;

            case KeyCode::A :
            if(m_movedKeys.aPressed == false){
                m_movedKeys.aPressed = true;
            }else{
                m_movedKeys.aPressed = false;
            }

            break;

            case KeyCode::D :
            if(m_movedKeys.dPressed == false){
                m_movedKeys.dPressed = true;
            }else{
                m_movedKeys.dPressed = false;
            }

            break;

            case KeyCode::Space :
            if(m_movedKeys.SpacePressed == false){
                m_movedKeys.SpacePressed = true;
            }else{
                m_movedKeys.SpacePressed = false;
            }

            break;

            case KeyCode::Control :
            if(m_movedKeys.CtrlPressed == false){
                m_movedKeys.CtrlPressed = true;
            }else{
                m_movedKeys.CtrlPressed = false;
            }

            break;
            case KeyCode::Shift :
            if(m_movedKeys.ShiftPressed == false){
                m_movedKeys.ShiftPressed = true;
                m_movingspeed = FAST_SPEED; 
            }else{
                m_movedKeys.ShiftPressed = false;
                m_movingspeed = BASIC_SPEED;
            }

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

    // we process this event after update() is called..
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
    if(m_movedKeys.wPressed){
        move_forward();
    }else if(m_movedKeys.sPressed){
        move_backward();
    }
    if(m_movedKeys.aPressed){
        move_left();
    }else if(m_movedKeys.dPressed){
        move_right();
    }
    if(m_movedKeys.SpacePressed){
        move_up();
    }else if(m_movedKeys.CtrlPressed){
        move_down();
    }  
}

//Update Camera from position and Direction of the player
void Camera::updateFromPlayer(Vec3f pos, Vec3f dir)
{
    m_position = pos;
    m_position.y += 1; // this is to increase the eye position (as height) of the player
    m_direction = dir;
}
