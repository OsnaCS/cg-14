#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
using namespace std;


class Player {	
public:

	//Konstruktor
    Player(Map& m);
    //void setMap(Map& map);

	//getter-Methoden
  Vec3f getPosition();
	int getHealth();
	int getHunger();
  void turn_side(float deltaX);
  void turn_upDown(float deltaY);
  void update();
	Vec3f getDirection();

	//Zum Bewegen
	//ggf. Aufnehmen, Ablegen, Abbauen, Aufbauen
  lumina::EventResult processEvent( lumina::InputEvent& e, Window& win, bool cheatmode );

	//Updated Health
	//Fallen und Hunger verursacht Schaden
	//über Zeit wird Health regeneriert
	void updateHealth();

	//Updated Hunger
	//Hunger wird nach Zeiteinheit berechnet:
	//Wenn Hunger > 10, wird -0.5 Leben abgezogen
	//Essbares senkt Hunger 
	void updateHunger();

	//ggf. Inventar

private:
	//Methods
	void move_left();
  void move_right();
  void move_forward();
  void move_backward();
  void move_up();
  void move_down();

  //3D Atributes
  Vec3f m_position;
  Vec3f m_direction;
  //Movement
  float m_movingspeed;
  float m_yMovementspeed;
  bool m_mouseCaptured;
  bool m_wPressed;
  bool m_sPressed;
  bool m_aPressed;
  bool m_dPressed;
  bool m_SpacePressed;
  bool m_CtrlPressed;
  bool m_ShiftPressed;

  Map& m_map;

};
