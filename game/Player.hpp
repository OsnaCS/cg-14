#pragma once

#include "lumina/lumina.hpp"
using namespace std;


class Player {
public:

	//Konstruktor
	Player();

	//Setter-Methoden
	setPos(int x, int y, int z){
		m_pos = Vec3i(x,y,z);
	}

	//getter-Methoden
  Vec3i getPos(){
		return m_pos;
	}

	int getHealth(){
		return m_health;
	}

	int m_hunger(){
		return m_hunger;
	}

	void onEvent(InputEvent e);



private:
	Vec3i m_pos;
	int m_health;
	int m_hunger;
	Inventar m_inventar;
}