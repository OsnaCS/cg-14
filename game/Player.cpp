#include "Player.hpp"

using namespace std;
using namespace lumina;

Player::Player(Map m){
	m_pos = {0,0,0};
	m_health = 3;
	m_hunger = 0;
	m_velocity_x = 0;
  m_velocity_z = 0;
  m_map = m; 
}

EventResult Player::onEvent(InputEvent e){

	if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::W) {
      m_velocity_z = -1;
      return EventResult::Processed;
  }

	if(e.type == InputType::KeyReleased && e.keyInput.key == KeyCode::W) {
      m_velocity_z = 0;
      return EventResult::Processed;
  }

  if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::A){
    	m_velocity_x = -1;
    	return EventResult::Processed;
  }

  if(e.type == InputType::KeyReleased && e.keyInput.key == KeyCode::A) {
      m_velocity_x = 0;
      return EventResult::Processed;
  }

  if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::S){
    	m_velocity_z = 1;
    	return EventResult::Processed;
	}

	if(e.type == InputType::KeyReleased && e.keyInput.key == KeyCode::S) {
      m_velocity_z = 0;
      return EventResult::Processed;
  }

	if (e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::D){
    	m_velocity_x = 1;
    	return EventResult::Processed;
  }

  if(e.type == InputType::KeyReleased && e.keyInput.key == KeyCode::D) {
      m_velocity_x = 0;
      return EventResult::Processed;
  }

  //Springen
  if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Space){
  	 m_pos.y += 2;
  	 return EventResult::Processed;
  }

  //Sprinten
  if(e.type == InputType::KeyPressed && e.keyInput.key == KeyCode::Shift){
  	m_velocity_x *= 2;
  	m_velocity_z *= 2;
  }

  if(e.type == InputType::KeyReleased && e.keyInput.key == KeyCode::Shift){
  	m_velocity_x *= .5;
  	m_velocity_z *= .5;
  }

	//solange noch mindestens eine WASD-Taste gedrückt ist, wird die Position des
  //Players verändert
  while(m_velocity_x != 0 || m_velocity_z != 0){

  	Vec3i pos1 = {m_pos.x + m_velocity_x, m_pos.y, m_pos.z + m_velocity_z};
  	Vec3i pos2 = {m_pos.x, m_pos.y-1, m_pos.z};

    //man kann nur gehen, wenn in die Richtung Air ist
    if(m_map.getBlockType(pos1) == BlockType::Air){
    	m_pos = pos1;	
    }

   	//man fällt, wenn unter einem Air ist
   	while(m_map.getBlockType(pos2) == BlockType::Air){
   		m_pos = pos2;
   		pos2 = {m_pos.x, m_pos.y-1, m_pos.z};    	
   	}
  }

  //ggf. Aufnehmen, Ablegen, Abbauen, Aufbauen

  return EventResult::Skipped;
}

