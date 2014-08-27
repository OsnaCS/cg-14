#ifndef PLAYERVIEW_HPP
#define PLAYERVIEW_HPP

#include "lumina/lumina.hpp"
#include "Player.hpp"
#include "Camera.hpp"

#include <vector>

/** Converse char to int */
#define toDigit(c) (c-'0')

/**
 * @brief The PlayerView class Player's panel
 */
class PlayerView {	
	public:	
        /**
         * @brief PlayerView
         * @param player
         */
		PlayerView(Player& player);

        /**
         * @brief init Initalize the player's status, graphics and its inventory e.g. heart status panel,
         *             objects in inventory, picked-axe and centered-marker (to hit with picked-axe).
         */
        void init();

        /**
         * @brief draw 2D-Drawer for the following states:
         *             (i) heart panel shows status of the player's heart,
         *             (ii) inventory and
         *             (iii) centered-marker (to hit with axe)
         */
        void draw();

        /**
         * @brief drawNormalPass
         * @param viewMat
         * @param projMat
         */
		void drawNormalPass(Mat4f viewMat, Mat4f projMat);
		void drawFinalPass(Mat4f viewMat, Mat4f projMat, Camera cam, Tex2D& lBuffer);

	private:
      VertexSeq<Vec2f, Vec3f, Vec2f> updateHearts();
      VertexSeq<Vec2f, Vec2f> updateInventory();
      VertexSeq<Vec2f, Vec3f, Vec2f> updateInventoryNumbers();

      void initInventory();
      void initCenterMarker();
      void initHeartPanel();
      void initInventoryNumber();
      std::vector<Vec2f> positionOfNumber(int num);

      Player& m_player;
	  Program m_program;
	  Program m_normalPass;
	  Program m_finalPass;
      Program m_markerProgram;
      Program m_inventoryProg;
      Program m_numberProg;
	  Tex2D m_pickaxeTexture;
	  Tex2D m_pickaxeNormals;
	  Tex2D m_colorTexture;
      Tex2D m_inventoryTex;
      Tex2D m_numberTex;
	  VertexSeq<Vec3f, Vec3f, Vec2f> m_pickaxe;
      VertexSeq<Vec2f, Vec3f> m_centerMarker;
};

#endif
