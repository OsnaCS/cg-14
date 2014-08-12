#include "Camera.hpp"
#include "lumina/lumina.hpp"

using namespace lumina;
using namespace std;

Camera::Camera() :
    m_position(Vec3f(0.0f, 0.0f, 10.0f))
    ,m_movingspeed(0.1f)
{
	m_direction = Vec3<float>(0.0f, 0.0f, -1.0f);
	m_up = cross(cross(m_direction.normalized(), Vec3f(0.f, 1.f, 0.f)), m_direction);
}

Mat4<float> Camera::get_matrix(){
		return viewMatrix(m_position,m_direction,m_up);
}

Mat4<float> Camera::get_ProjectionMatrix(){
		return projectionMatrix(0.785f, 1.0f, 0.0001f, 1000000.0f);
};

EventResult Camera::processEvent( InputEvent& e )
{
    // Key
    if(e.type == InputType::KeyPressed)
    {
        switch( e.keyInput.key)
        case KeyCode::Q :
            move_left();
            return EventResult::Processed;
         //case KeyCode::W :
         //   return EventResult::Processed;
    }

    if (e.type == InputType::LMousePressed )
        {

    }


    return EventResult::Skipped;
}

void Camera::move_left()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), -1.5708f ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   for( int i=0; i< left_direction.size; i++  ) {
       cout<<left_direction[i]<<" ";
   }
   cout<<endl;
   m_position.x += m_movingspeed*left_direction.x;
   m_position.z += m_movingspeed*left_direction.z;
}



