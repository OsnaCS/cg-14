#pragma once


#include "lumina/lumina.hpp"
#include "Player.hpp"

class PlayerView {	
	public:	
		PlayerView(Player& player);

		void draw();
		void init();
	private:
		Player& m_player;
	  VertexSeq m_heartPanel;
	  Program m_program;
	  Tex2D m_colorTexture;
};

