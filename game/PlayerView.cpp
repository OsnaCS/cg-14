#include "PlayerView.hpp"
#include "lumina/io/ImageJPEG.hpp"

using namespace std;
using namespace lumina;

PlayerView::PlayerView(Player& player):
m_player(player)
{
}
 
void PlayerView::draw()
{

	m_program.prime([&](HotProgram& hotprog)
	{

		//viewMat.setColumn(3, Vec4f(0,0,0,1));
		//Mat4f mat;
		//mat.setToIdentity();
		//mat.setDiagonal(Vec4f(2,2,2,1));

		//hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.draw(m_heartPanel, PrimitiveType::TriangleStrip);

	});
}


void PlayerView::init()
{

	VShader vs;
	vs.compile(loadShaderFromFile("shader/HeartPanel.vsh"));
	FShader fs;
	fs.compile(loadShaderFromFile("shader/HeartPanel.fsh"));

	constexpr float PI = 3.1415926;

  int activeHearts = m_player.getHearts();
  int maxHearts = m_player.getMaxHearts();
  float changeX = maxHearts / 2.0f;
  float heartWidth = changeX / 20 / changeX;
  float nextHeart = 0.0f - changeX / 20;

  // create program and link the two shaders
	m_program.create(vs, fs);
  m_heartPanel.create(2 + 3, 4, 4);
  m_heartPanel.prime<Vec2f, Vec3f>([&](HotVertexSeq<Vec2f, Vec3f>& hot)
  {
  	slog(nextHeart);
 		//for(int i = 0; i<maxHearts; i++){
  	
  	hot.vertex[0].set(Vec2f(nextHeart, -0.45f),  Vec3f(1,1,1));
  	hot.vertex[1].set(Vec2f(nextHeart, -0.4f),  Vec3f(1,1,1));
  	hot.vertex[2].set(Vec2f(nextHeart + 0.01f, -0.45f) , Vec3f(1,1,1));
  	hot.vertex[3].set(Vec2f(nextHeart + 0.01f, -0.4f), Vec3f(1,1,1));
		//}

  	hot.index[0] = 0;
  	hot.index[1] = 1;
  	hot.index[2] = 2;
  	hot.index[3] = 3;

  });

}
