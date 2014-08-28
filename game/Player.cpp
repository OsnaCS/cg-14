#include "Player.hpp"

using namespace std;
using namespace lumina;


//The Block has a radius of 0.5
const float BLOCK_DIFFERENCE_X = 0.5f; 
const float BLOCK_DIFFERENCE_Y = 0.5f;
const float BLOCK_DIFFERENCE_Z = 0.5f;

//To not collide into other blocks you have to stay at least away an amount of FREE_SPACE
const float FREE_SPACE = 0.3f;

//The Speeds are for normal walking, running, falling and Jumping
const float FRAME_PER_MOVE = 30;
const float BASIC_SPEED = 0.2f; 
const float FAST_SPEED = 0.4f;

const float FALL_SPEED = -0.03f;
const float JUMP_SPEED = 0.5f;

const float CAMERA_HIGH = 1.8f;
const int MAX_HEARTS = 10;
//Not used right now, but in near future
//const float TIME_STEP  = 1.0f;

const Vec3f INIT_POSITION = Vec3f(0.0f, 80.5f, 0.0f);


Player::Player(Map& map, MapView& mapView)
    :m_position(INIT_POSITION)
    ,m_direction(Vec3f(0.0f, 0.0f, -1.0f))
    ,m_movingspeed(0.3f)
    ,m_xMovementspeed(0.0f)
    ,m_yMovementspeed(0.0f)
    ,m_zMovementspeed(0.0f)
    ,m_timePassed(0.0f)
    ,m_mouseCaptured(false)
    ,m_rightMouseCaptured(false)
    ,m_wPressed(false)
    ,m_sPressed(false)
    ,m_aPressed(false)
    ,m_dPressed(false)
    ,m_ePressed(false)
    ,m_SpacePressed(false)
    ,m_CtrlPressed(false)
    ,m_ShiftPressed(false)
    ,m_map(map)
    ,m_mapView(mapView)
    ,m_fallen(0)
    ,m_attrib(MAX_HEARTS)
    ,m_inventory(10)
{

}

EventResult Player::processEvent( InputEvent& e , Window& win, bool cheatmode)
{
    //Key Pressed and Released
    if(e.type == InputType::KeyPressed || e.type == InputType::KeyReleased){

        switch( (KeyCode)(e.keyInput.key))
        {
            case KeyCode::W :
            if(m_wPressed == false){
                m_wPressed = true;
            }else{
                m_wPressed = false;
            }

            break;

            case KeyCode::S :
            if(m_sPressed == false){
                m_sPressed = true;
            }else{
                m_sPressed = false;
            }

            break;

            case KeyCode::A :
            if(m_aPressed == false){
                m_aPressed = true;
            }else{
                m_aPressed = false;
            }

            break;

            case KeyCode::D :
            if(m_dPressed == false){
                m_dPressed = true;
            }else{
                m_dPressed = false;
            }

            break;

            case KeyCode::Space :
            if(m_SpacePressed == false){
                m_SpacePressed = true;
            }else{
                m_SpacePressed = false;
            }

            break;

            case KeyCode::Control :
            if(m_CtrlPressed == false){
                m_CtrlPressed = true;
            }else{
                m_CtrlPressed = false;
            }

            break;

            case KeyCode::Shift :
            if(m_ShiftPressed == false){
                m_ShiftPressed = true;
                m_movingspeed = FAST_SPEED; 
            }else{
                m_ShiftPressed = false;
                m_movingspeed = BASIC_SPEED;
            }

            break;

            case KeyCode::E :
            if(m_ePressed == false&&e.type == InputType::KeyPressed){
                m_ePressed = true;
            }
            break;
           case KeyCode::K1:
            m_blockType = 0;

            break;
           case KeyCode::K2:
            m_blockType = 1;
            break;
           case KeyCode::K3:
            m_blockType = 2;
            break;
           case KeyCode::K4:
            m_blockType = 3;
            break;
           case KeyCode::K5:
            m_blockType = 4;
            break;
           case KeyCode::K6:
            m_blockType = 5;
            break;
            case KeyCode::K7:
             m_blockType = 6;
            break;
            case KeyCode::K8:
             m_blockType = 7;
            break;
            case KeyCode::K9:
            m_blockType = 8;
            break;
            case KeyCode::K0:
            m_blockType = 9;
            break;
            default:
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
    //Right Click
    if(e.type == InputType::RMousePressed) {
      m_rightMouseCaptured = true;
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


void Player::update(float timePassed)
{

  if(m_ePressed){
    m_ePressed = false;
    auto nextBlock = getLastAir();
    //If there is only air or we are standing to near, don't place a block
    if(nextBlock != m_position){
        m_map.setBlockType(getLastAir(), m_inventory.getType(m_blockType) );
        m_inventory.removeItem( m_inventory.getType(m_blockType) );
        m_mapView.notifyBlockUpdate(nextBlock);
        }
  }
  //handle block destroy
  if(m_rightMouseCaptured){
    m_rightMouseCaptured = false;
    auto nextBlock = getNextBlock();
    if (m_map.exists(nextBlock)) {
      m_inventory.addItem(m_map.getBlockType(nextBlock));
      m_map.setBlockType(nextBlock, BlockType::Air);
      m_mapView.notifyBlockUpdate(nextBlock);
    }
  }
  
  m_timePassed += timePassed*1000;
  while(m_timePassed >= FRAME_PER_MOVE){
    m_timePassed -= FRAME_PER_MOVE;
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
    if(m_SpacePressed){
        move_up();
      }else if(m_CtrlPressed){
        move_down();
    }  
    movement();
  }
    if ( m_attrib.getHearts() == 0 ) {
        m_attrib.setHearts(MAX_HEARTS); // reset the game
        m_position = INIT_POSITION; // reset position
    }
}


int Player::getHearts()
{
  return m_attrib.getHearts();
}

int Player::getMaxHearts() const
{
  return m_attrib.maxHearts();
}

void Player::move_left()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), (float)(M_PI/2) ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_xMovementspeed += m_movingspeed*left_direction.x;
   m_zMovementspeed += m_movingspeed*left_direction.z;
}

void Player::move_right()
{
   Mat4f rotationM = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), (float)(-M_PI/2) ));
   Vec4f left_direction =  rotationM * Vec4f(m_direction.x, m_direction.y, m_direction.z, 1.0f);
   m_xMovementspeed += m_movingspeed*left_direction.x;
   m_zMovementspeed += m_movingspeed*left_direction.z;

}

void Player::move_forward()
{  // Calculate the movement by dividing the y part to the x and z in ratio

  //TODO - Stop moving if falling
   m_xMovementspeed += m_movingspeed*(m_direction.x+(m_direction.x/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   m_zMovementspeed += m_movingspeed*(m_direction.z+(m_direction.z/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   //slog("move_forward()...collided, recalc by x_speed ", m_xMovementspeed, ", z_seed ", m_zMovementspeed );

}


void Player::move_backward()
{
   // Calculate the movement by dividing the y part to the x and z in ratio

  //TODO - Stop moving if falling
   m_xMovementspeed += (-1) * m_movingspeed*(m_direction.x+(m_direction.x/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   m_zMovementspeed += (-1) * m_movingspeed*(m_direction.z+(m_direction.z/(fabs(m_direction.x)+fabs(m_direction.z))*fabs(m_direction.y)));
   //slog("move_backward()...collided, recalc by x_speed ", m_xMovementspeed, ", z_seed ", m_zMovementspeed );

}

void Player::move_up() //Jump
{
  
  if(m_yMovementspeed == 0){
     m_yMovementspeed += JUMP_SPEED;
  } 
}

//No use anymore, because of falling to the ground by default
void Player::move_down()
{
//    m_position.y -= m_movingspeed;
}



//Handle all Movements while checking for collision with Blocks
void Player::movement()
{
  Vec3i pos = Vec3i(static_cast<int>(round(m_position.x)),static_cast<int>(round(m_position.y)),static_cast<int>(round(m_position.z)) );
  //1*get_sign(m_yMovementspeed) > 0?2:-1 ==> If the  y Movement is negative, look 1 down, else look 2 up
  Vec3i posY = Vec3i(static_cast<int>(round(m_position.x)),static_cast<int>(round(m_position.y+(1*get_sign(m_yMovementspeed) > 0?2:-1) )),
              static_cast<int>(round(m_position.z)) );
  
  float delta = m_position.y - pos.y + BLOCK_DIFFERENCE_Y;
     
  if( pos.y <= 0 || pos.y>=127 )
    return;


  //Y-Movement
  //Check collision for air
  if( !collide(pos.x, pos.y, pos.z))
   {
        //slog("Speed: ",m_yMovementspeed);
        //slog("Not collision in Y!");
        //get fallen distance
        if(m_yMovementspeed < 0){
          //slog("Fallen:", m_fallen);
          m_fallen += m_yMovementspeed;
        }
        if(!collide(posY.x, posY.y, posY.z)){
          m_yMovementspeed = max(-1.0f, m_yMovementspeed + FALL_SPEED) ;
          m_position.y += m_yMovementspeed;
        }else{
          //Hit the floor
          //float delta = m_position.y - pos.y + BLOCK_DIFFERENCE_Y;  
          if(m_yMovementspeed+FALL_SPEED < delta){
            m_yMovementspeed = -delta;
            m_position.y += m_yMovementspeed;
            //If the distance fallen is bigger than 4, 
            //the player takes damage equal to 2 powered by fallen-5
            if(m_fallen < -4){
                m_attrib.updateHeartsBy( static_cast<int>(-pow(2, abs(m_fallen+4))) );
            }
            //Reset Movementspeed
            m_yMovementspeed = 0;
            m_fallen = 0;
          } else{
            //Freefall
            m_yMovementspeed += FALL_SPEED;
            m_position.y += m_yMovementspeed;  
          }
        }
    }


    pos = Vec3i(static_cast<int>(round(m_position.x)),static_cast<int>(round(m_position.y)),static_cast<int>(round(m_position.z)) );

    //X/Z-Movement, the delta is Absolute and subtracted by the space to prohibid the collision!
    float deltaX =  fabs(m_position.x - pos.x - BLOCK_DIFFERENCE_X * get_sign(m_xMovementspeed)) - FREE_SPACE;
    float deltaZ =  fabs(m_position.z - pos.z - BLOCK_DIFFERENCE_Z * get_sign(m_zMovementspeed)) - FREE_SPACE; 

    //slog("m_pos.x: ", m_position.x, " pos.x:",pos.x, " DeltaX:", deltaX, " Movement X: ", m_xMovementspeed);
    //slog("m_pos.z: ", m_position.z, " pos.z:",pos.z, " DeltaZ:", deltaZ, " Movement Z: ", m_zMovementspeed);
    //slog("m_pos.y: ", m_position.y, " pos.y: ", pos.y);
    //slog("m_dir.x: ", m_direction.x, " m_direction.z: ",m_direction.z, " m_direction.y: ", m_direction.y);

   //Check if we can move for XZ-direction
   if(!collide(pos.x, pos.y, pos.z))
   {
        //Free Moving because of free space in moving direction.
        //The movement is only in the current block
        if(fabs(m_xMovementspeed) <= deltaX && fabs(m_zMovementspeed) <= deltaZ ) {
           m_position.x += m_xMovementspeed;
           m_position.z += m_zMovementspeed;
           //slog("Case 1: free moving x<=delta//z<=delta");
        }
        //Move freely in X direction but make a limited move in Z
        //The X Movement is only in the current Block, but the Y Movement is outside
        else if(fabs(m_xMovementspeed) <= deltaX){ 
           m_position.x += m_xMovementspeed;
           // we check if we can move in the Z direction...
           // if there is air, we can move freely
           if(!collide(pos.x,pos.y,pos.z + (1 * get_sign(m_zMovementspeed)))&&!collide(pos.x,pos.y+1,pos.z + (1 * get_sign(m_zMovementspeed)))){
              m_position.z += m_zMovementspeed;
              //slog("Case 2: move freely in Z but limited on  x<=delta//no collision at z");
            }else{
               //Only move by delta changed by sign of the movement
               m_position.z += deltaZ * get_sign(m_zMovementspeed);
               //slog("Case 3: move in Z with limited delta x<=delta//COLLISION at z");
            }
        }
        // Move freely in Z direction but make a limited move in X
        else if(fabs(m_zMovementspeed) <= deltaZ)
        {
           //Move in X but look for Z
           m_position.z += m_zMovementspeed;
           // we check if we can move in the Z direction...
           if(!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y, pos.z)&&!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y+1, pos.z)){
                m_position.x += m_xMovementspeed;
                //slog("Case 4: move freely in Z and X z<=delta//no collision at x");
           }else{
               //Only move by delta
                m_position.x += deltaX * get_sign(m_xMovementspeed);
                //slog("Case 5: move freely in Z and with limited on X z<=delta//COLLISION at x");
            }
        }
        else
        {
          //slog("Case 6: Movement are both bigger than delta");
          if(!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y, pos.z )&&!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y+1, pos.z )){
            if(!collide(pos.x, pos.y, pos.z + (1 * get_sign(m_zMovementspeed)) )&&!collide(pos.x, pos.y+1, pos.z + (1 * get_sign(m_zMovementspeed)) )){
              if(!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y, pos.z + (1 * get_sign(m_zMovementspeed)))
                &&!collide(pos.x + (1 * get_sign(m_xMovementspeed)) , pos.y+1, pos.z + (1 * get_sign(m_zMovementspeed)))){
                //No collison, move freely in X and Z
                //slog("Case 6-1: No Colliision in X and Z");
                m_position.x += m_xMovementspeed;
                m_position.z += m_zMovementspeed;
              }else if(m_xMovementspeed > m_zMovementspeed){
                //slog("Case 6-1.1 ");
                m_position.x += m_xMovementspeed;
                m_position.z += deltaZ * get_sign(m_zMovementspeed);
              }else if(m_zMovementspeed > m_xMovementspeed){
                //slog("Case 6-1.2 ");
                m_position.z += m_zMovementspeed;
                m_position.x += deltaX * get_sign(m_xMovementspeed);
              }else{
                //Same Movementspeed
                //slog("Case 6-1.3 ");
                m_position.x += deltaX * get_sign(m_xMovementspeed);
                m_position.z += deltaZ * get_sign(m_zMovementspeed);
              }
            }else{
              // only collision in Z and we can move freely in X
              m_position.x += m_xMovementspeed;
              m_position.z += deltaZ * get_sign(m_zMovementspeed);
              //slog("Case 6-2: Only collide in Z (Delta Both too big)");

            }
          }else{
            if(!collide(pos.x, pos.y, pos.z + (1 * get_sign(m_zMovementspeed)) )&&!collide(pos.x, pos.y+1, pos.z + (1 * get_sign(m_zMovementspeed)) )){
            // only collision in X and we can move freely in Z
              m_position.z += m_zMovementspeed;
              m_position.x += deltaX * get_sign(m_xMovementspeed);
              //slog("Case 6-3: Only collide in X (Delta Both too big)");
            }else{
              //Collision in both, X and Z
              m_position.x += deltaX * get_sign(m_xMovementspeed);
              m_position.z += deltaZ * get_sign(m_zMovementspeed);
              //slog("Case 6-4: Both collision");
            }
          }

        }
   }
   else // collide with other objects ...we don't update the m_position of x and z
   {
        //slog("Collide with other objects with ");

   }
   //Reset Movementspeed
   m_xMovementspeed = 0;
   m_zMovementspeed = 0;

   //slog("-------------------------------------------------------------");

}

bool Player::collide(float x, float y, float z)
{
  Vec3i pos = Vec3i(static_cast<int>(round(x)),static_cast<int>(round(y)),static_cast<int>(round(z)));
  if(m_map.exists(pos) && m_map.getBlockType(pos) == BlockType::Air){
      return false;
    }

    return true;
}



Vec3i Player::getNextBlock()
{
  float x = 0;
  float y = 0;
  float z = 0;

  //Return current position (where already air is);
  Vec3i resultNull = Vec3i(m_position.x, m_position.y+1, m_position.z);
  
  while(sqrt(x*x+y*y+z*z)<4){
    x += m_direction.x*0.03;
    y += m_direction.y*0.03;
    z += m_direction.z*0.03;
    
    if(collide(m_position.x+x,m_position.y+y+CAMERA_HIGH,m_position.z+z)){
      Vec3i pos = Vec3i(static_cast<int>(round(x+m_position.x)),static_cast<int>(round(y+m_position.y+CAMERA_HIGH)),static_cast<int>(round(z+m_position.z)));

      return pos;
    }
  }
  return resultNull;
}


Vec3i Player::getLastAir()
{
  float x = 0;
  float y = 0;
  float z = 0;

  //Return current position (where already air is);
  Vec3i resultNull = Vec3i(m_position.x, m_position.y, m_position.z);
  
  while(sqrt(x*x+y*y+z*z)<4){
    x += m_direction.x*0.03;
    y += m_direction.y*0.03;
    z += m_direction.z*0.03;
    
    if(collide(m_position.x+x,m_position.y+y+CAMERA_HIGH,m_position.z+z)){
      Vec3i pos = Vec3i(static_cast<int>(round(x+m_position.x-m_direction.x*0.03)),
                        static_cast<int>(round(y+m_position.y+CAMERA_HIGH-m_direction.y*0.03)),
                        static_cast<int>(round(z+m_position.z-m_direction.z*0.03)));
      return pos;
    }
  }
  return resultNull;
}

const map<BlockType, int>& Player::getInventoryItems() const
{
    return m_inventory.getItems();
}


 int Player::maxDisplayItems() const
 {
     return m_inventory.maxDisplayItems();
 }

void Player::reset(Vec3f pos)
{
  m_position = pos;
}

void Player::resetkeys()
{
  m_wPressed = false;
  m_aPressed = false;
  m_sPressed = false;
  m_dPressed = false;
  m_SpacePressed = false;
  m_CtrlPressed = false;
  m_ShiftPressed = false;
}

