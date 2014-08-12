#pragma once

#include "lumina/lumina.hpp"
#include "BlockType.hpp"
#include "Chunk.hpp"
#include "Map.hpp"
using namespace std;


class Player {	
public:

	//Konstruktor
	Player(Map m);

	//getter-Methoden
  Vec3i getPos(){
		return m_pos;
	}
	int getHealth(){
		return m_health;
	}
	int getHunger(){
		return m_hunger;
	}

	//Zum Bewegen
	//ggf. Aufnehmen, Ablegen, Abbauen, Aufbauen
	EventResult onEvent(lumina::InputEvent e);

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
	Vec3i m_pos;
	int m_health;
	int m_hunger;
	int m_velocity_x;
	int m_velocity_z;
	Map m_map;
	//Inventar m_inventar;
};