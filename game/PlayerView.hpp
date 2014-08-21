#pragma once


#include "lumina/lumina.hpp"
#include "Player.hpp"

class PlayerView {	
	public:	
		PlayerView(Player& player);

		void draw(Mat4f viewProjectionMatrix);
		void init();
	private:
		VertexSeq updateHearts();
		Player& m_player;
	  //VertexSeq m_heartPanel;
	  Program m_program;
	  Program m_program2;
	  Tex2D m_pickaxeTexture;
	  Tex2D m_colorTexture;
	  VertexSeq m_pickaxe;
};

