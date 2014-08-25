#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
#include "PlayerAttributes.hpp"


/** @Class Player 
*/
class Player {	


public:

	//Constructor
  Player(Map& m);

  /**
   * @brief getHearts Get current number of hearts indicating health of the players
   * @return current number of hearts
   */
  int getHearts();

  /**
   * @brief getMaxHearts Maximum number of hearts to be initialized
   * @return maximum number of hearts
   */
  int getMaxHearts() const;

  /** @brief Processing all actual movements
  For every pressed key, the movement will be added and processed.
  */
  void update();

  /** @brief Get the direction of the Players view
  		@return Vec3f direction The vector where the Player is looking at
  */
  inline	Vec3f getDirection();

  /** @brief Get the position of the Player
  		@return Vec3f position Get the Position in 3D Space
  */
  inline Vec3f getPosition();

  /** @brief Process all input Events by Mouse and Keyboard

			Handle different Input:
			W = Move forward
			S = Move backward
			A = Move left
			D = Move right
			Space = Jump
			Mouse Movement = Turn Side // Turn Up and Down

  		@param e Event that thows up in the Game (Keyboard or Mouseinput)
  		@param win The current window for calculating the ratio of dimensions (for the View-Matrix)
  		@param cheatmode True - No Changes in this class are allowed
  										 False - Changes are allowed

  		@return EventResult Give back the result of processing (Skipped/Processed)
  */
  lumina::EventResult processEvent( lumina::InputEvent& e, Window& win, bool cheatmode);

private:
	// Get the sign of the Movement
    inline int get_sign(float x);

	//Methods
	void move_left();
  void move_right();
  void move_forward();
  void move_backward();
  void move_up();
  void move_down();
  
  // Check the Delta of Movement and the Collision. Move in different Cases
  void movement();

  //Check the position for case of air (x/y/z coordinates)
	bool collide(float x, float y, float z);
 
  void turn_side(float deltaX);
  void turn_upDown(float deltaY);
  
  //3D Atributes
  Vec3f m_position;
  Vec3f m_direction;
  //Movement
  float m_movingspeed;
	float m_xMovementspeed;
  float m_yMovementspeed;
  float m_zMovementspeed;
  bool m_mouseCaptured;
  bool m_wPressed;
  bool m_sPressed;
  bool m_aPressed;
  bool m_dPressed;
  bool m_SpacePressed;
  bool m_CtrlPressed;
  bool m_ShiftPressed;
  Map& m_map;
  float m_fallen;
  PlayerAttributes m_attrib;
};


/////////////INLINE

Vec3f Player::getPosition()
{
  return Vec3f(m_position.x, m_position.y+1.1f, m_position.z);

}

Vec3f Player::getDirection()
{
  return m_direction;
}

int Player::get_sign(float x) {
    const float eps = 0.0001;
    return (x > eps)? 1 : ( fabs(x) < eps)? 0: -1;
}



///////////////////////////////END OF FILE/////////////////////////////////////
