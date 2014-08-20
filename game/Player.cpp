#include "Player.hpp"

using namespace std;
using namespace lumina;

const float BASIC_SPEED = 0.3f; 
const float FAST_SPEED = 0.8f;
const float FALL_SPEED = -0.06f;
const float TIME_STEP  = 1.0f;
const float JUMP_SPEED = 0.6;
const int MAX_HEARTS = 10;

Player::Player( Map& m) :
    m_position(Vec3f(0.0f, 80.0f, 0.0f))
    ,m_direction(Vec3f(0.0f, 0.0f, -1.0f))
    ,m_movingspeed(0.3f)
    ,m_yMovementspeed(0.0f)
    ,m_mouseCaptured(false)
    ,m_wPressed(false)
    ,m_sPressed(false)
    ,m_aPressed(false)
    ,m_dPressed(false)
    ,m_SpacePressed(false)
    ,m_CtrlPressed(false)
    ,m_ShiftPressed(false)
    ,m_map(m)
    ,m_hearts(MAX_HEARTS)



{

}


EventResult Player::processEvent( InputEvent& e , Window& win, bool cheatmode)
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

    if(!cheatmode){
      // Mouse
      if (m_mouseCaptured && e.type == InputType::MouseMoveDir)
      {
        turn_side(-e.mouseInput.x / 300.0f);
        turn_upDown(-e.mouseInput.y / 50.0f);
      }
    }

    return EventResult::Skipped;
}
/*
   	//man fällt, wenn unter einem Air ist
   	while(m_map.getBlockType(pos2) == BlockType::Air){
   		m_pos = pos2;
   		pos2 = {m_pos.x, m_pos.y-1, m_pos.z};    	
   	}
  }
*/
  //ggf. Aufnehmen, Ablegen, Abbauen, Aufbauen


void Player::turn_side(float deltaX)
{
    Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), deltaX));
    Vec4f new_direction =  rotationM*Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    float sum_vector = abs(new_direction.x) + abs(new_direction.y) + abs(new_direction.z);
    m_direction.x = new_direction.x / sum_vector;
    m_direction.y = new_direction.y / sum_vector;
    m_direction.z = new_direction.z / sum_vector;
}

void Player::turn_upDown(float deltaY)
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


void Player::update()
{
    
    Vec3i pos = Vec3i(static_cast<int>(round(m_position.x)),static_cast<int>(round(m_position.y)),static_cast<int>(round(m_position.z)) );
    //TODO
    //Check for too much high // to less high

    
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

    if( pos.y>= 0 & pos.y<=127 ){
      if(m_map.getBlockType(pos) == BlockType::Air){
        m_yMovementspeed = max(-1.0f, m_yMovementspeed + FALL_SPEED) ;
      }else{
        m_yMovementspeed = 0;
      }
      if(m_SpacePressed){
        move_up();
      }else if(m_CtrlPressed){
        move_down();
    }  
      m_position.y += m_yMovementspeed*TIME_STEP;
    }

}

Vec3f Player::getPosition(){
  return Vec3f(m_position.x, m_position.y+1.0f, m_position.z);
//  return m_position;
}

Vec3f Player::getDirection(){
  return m_direction;
}

void Player::move_left()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), 1.5708f ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_position.x += m_movingspeed*left_direction.x;
   m_position.z += m_movingspeed*left_direction.z;
}

void Player::move_right()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), -1.5708f ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_position.x += m_movingspeed*left_direction.x;
   m_position.z += m_movingspeed*left_direction.z;
}

void Player::move_forward()
{  // Calculate the movement by dividing the y part to the x and z in ratio
   m_position.x += m_movingspeed*(m_direction.x+(m_direction.x/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   m_position.z += m_movingspeed*(m_direction.z+(m_direction.z/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
  // m_position.z += m_movingspeed*m_direction.z;
}


void Player::move_backward()
{
   // Calculate the movement by dividing the y part to the x and z in ratio
   m_position.x -= m_movingspeed*(m_direction.x+(m_direction.x/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   m_position.z -= m_movingspeed*(m_direction.z+(m_direction.z/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
  
}
void Player::move_up() //Jump
{
  
  if(m_yMovementspeed == 0){
     m_yMovementspeed += JUMP_SPEED;
  } 
}

void Player::move_down()
{
//    m_position.y -= m_movingspeed;
}

int Player::getHearts(){
  return m_hearts;
}

int Player::getMaxHearts(){
  return MAX_HEARTS;
}
