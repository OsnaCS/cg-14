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

	private:
      VertexSeq<Vec2f, Vec3f, Vec2f> updateHearts();
      Player& m_player;
	  Program m_program;
	  Program m_normalPass;
	  Program m_finalPass;
      Program m_markerProgram;
	  Tex2D m_pickaxeTexture;
	  Tex2D m_pickaxeNormals;
	  Tex2D m_colorTexture;
	  VertexSeq<Vec3f, Vec3f, Vec2f> m_pickaxe;
      VertexSeq<Vec2f, Vec3f> m_centerMarker;
};

