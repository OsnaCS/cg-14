#pragma once


#include "lumina/lumina.hpp"
#include "Player.hpp"
#include "Camera.hpp"

class PlayerView {	
	public:	
		PlayerView(Player& player);

		void draw(Mat4f viewMatrix, Mat4f projectionMatrix);
		void init();

		void drawNormalPass(Mat4f viewMat, Mat4f projMat);
		void drawFinalPass(Mat4f viewMat, Mat4f projMat, Camera cam, Tex2D& lBuffer);
		Mat4f getTransMatrix();
		Mat4f getTransMatrixPlayer();
		void togglePickaxe();
		EventResult processEvent(InputEvent& e, Window& win, bool cheatmode);

	private:
        VertexSeq<Vec2f, Vec3f, Vec2f> updateHearts();
        Player& m_player;
      //VertexSeq m_heartPanel;
	  Program m_program;
	  Program m_normalPass;
	  Program m_finalPass;
	  Tex2D m_pickaxeTexture;
	  Tex2D m_playerTexture;
	  Tex2D m_colorTexture;
	  VertexSeq<Vec3f, Vec3f, Vec2f> m_pickaxe;
	  VertexSeq<Vec3f, Vec3f, Vec2f> m_playerFigure;
	  bool m_showPickaxe;
};

