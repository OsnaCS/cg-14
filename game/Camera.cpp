#include "Camera.hpp"
#include "lumina/lumina.hpp"

using namespace lumina;
using namespace std;

Camera::Camera() :
    m_position(Vec3f(0.0f, 0.0f, 10.0f))
    ,m_direction(Vec3f(0.0f, 0.0f, -1.0f))
    ,m_movingspeed(0.1f)
{
    m_up =  cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
}

Mat4<float> Camera::get_matrix(){
    //slog("Pos ", m_position);
    slog("Dir ", m_direction);
    return viewMatrix(m_position,m_direction,m_up);
}

Mat4<float> Camera::get_ProjectionMatrix(){
		return projectionMatrix(0.785f, 1.0f, 0.0001f, 1000000.0f);
};

EventResult Camera::processEvent( InputEvent& e )
{
    // Key
    if(e.type == InputType::KeyHold || e.type == InputType::KeyPressed)
    {
        switch( (KeyCode)(e.keyInput.key) ) {
        case KeyCode::Q :
            move_left();
            return EventResult::Processed;
        case KeyCode::E :
            move_right();
            return EventResult::Processed;
        case KeyCode::W :
            move_forward();
            return EventResult::Processed;
        case KeyCode::S :
            move_backward();
            return EventResult::Processed;
        case KeyCode::Space :
            move_up();
            return EventResult::Processed;
        case KeyCode::C :
            move_down();
            return EventResult::Processed;
        case KeyCode::A :
            turn_left();
            return EventResult::Processed;
        case KeyCode::D :
            turn_right();
            return EventResult::Processed;
        default:
            break;

       }
    }

    // Mouse
    if (e.type == InputType::MouseMoveDir || e.type == InputType::MouseMovePos)
    {

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
}


void Camera::move_backward()
{
   m_position.x -= m_movingspeed*m_direction.x;
   m_position.z -= m_movingspeed*m_direction.z;
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
void Camera::turn_left()
{
    Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), (float)(M_PI_2/2)));
    Vec4f new_direction =  rotationM*Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    m_direction.x = new_direction.x;
    m_direction.z = new_direction.z;
}
// change direction
void Camera::turn_right()
{
    Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f),(float)(-M_PI_2/10)));
    Vec4f new_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    m_direction.x = new_direction.x;
    m_direction.z = new_direction.z;
}
