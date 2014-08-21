#include "PlayerView.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "ObjectLoader.hpp"

using namespace std;
using namespace lumina;

const float HEART_SIZE = 0.05f;
const float HEART_POSY = -0.7f; 

PlayerView::PlayerView(Player& player):
m_player(player)
{
}
 
void PlayerView::draw(Mat4f viewProjectionMatrix)
{
  //updateHearts();
	m_program.prime([&](HotProgram& hotprog)
	{
 		m_colorTexture.prime(0, [&](HotTex2D& hotTex) {
			hotprog.draw(hotTex, updateHearts(), PrimitiveType::TriangleStrip);
		});
	});

	//Show pickaxe
	m_program2.prime([&](HotProgram& hotPickaxe){

		hotPickaxe.uniform["u_transform"] = viewProjectionMatrix;

		m_pickaxeTexture.prime(0, [&](HotTex2D& hotTex2){
			hotPickaxe.draw(hotTex2, m_pickaxe, PrimitiveType::Triangle);
		});
	});

}


void PlayerView::init()
{

	//Initialize of the texture
	ImageBox image_box = loadJPEGImage("gfx/hearts.jpeg");
  m_colorTexture.create(Vec2i(256,128), TexFormat::RGB8, image_box.data());
  m_colorTexture.params.filterMode = TexFilterMode::Trilinear;
  m_colorTexture.params.useMipMaps = true;

	VShader vs;
	vs.compile(loadShaderFromFile("shader/HeartPanel.vsh"));
	FShader fs;
	fs.compile(loadShaderFromFile("shader/HeartPanel.fsh"));
  // create program and link the two shaders
	m_program.create(vs, fs);
	m_program.perFragProc.blendFuncRGB = BlendFunction::Add;
	m_program.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
	m_program.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;

  // Pickaxe
	//Initialize of the texture
	ImageBox image_box2 = loadJPEGImage("gfx/pickaxe_texture512.jpg");
  m_pickaxeTexture.create(Vec2i(512,512), TexFormat::RGB8, image_box2.data());
  m_pickaxeTexture.params.filterMode = TexFilterMode::Linear;
  m_pickaxeTexture.params.useMipMaps = true;

	VShader vs2;
	vs2.compile(loadShaderFromFile("shader/Pickaxe.vsh"));
	FShader fs2;
	fs2.compile(loadShaderFromFile("shader/Pickaxe.fsh"));
  // create program and link the two shaders
	m_program2.create(vs2, fs2);
	m_program2.perFragProc.enableDepthTest();
	m_program2.perFragProc.setDepthFunction(DepthFunction::Less);

	// m_program2.perFragProc.blendFuncRGB = BlendFunction::Add;
	// m_program2.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
	// m_program2.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;

	m_pickaxe = loadOBJ("gfx/pickaxe2.obj");
	
}

VertexSeq PlayerView::updateHearts(){
  
  int activeHearts = m_player.getHearts();
  int maxHearts = m_player.getMaxHearts();
  float nextHeart = -(maxHearts / 2.0f) * HEART_SIZE;
 	VertexSeq m_heartPanel;
	m_heartPanel.create(7, 4 * maxHearts, 5 * maxHearts);
  m_heartPanel.prime<Vec2f, Vec3f, Vec2f>([&](HotVertexSeq<Vec2f, Vec3f, Vec2f>& hot)
  {
 	for(int i = 0; i < maxHearts; i++){
	  if(activeHearts > i){
		 	hot.vertex[0+i*4].set(Vec2f(nextHeart +	HEART_SIZE * i, HEART_POSY), Vec3f(0,1,1), Vec2f(0.5,0));
	  	hot.vertex[1+i*4].set(Vec2f(nextHeart +	HEART_SIZE * i, HEART_POSY - HEART_SIZE), Vec3f(1,1,1),Vec2f(0.5,1));
	  	hot.vertex[2+i*4].set(Vec2f(nextHeart + HEART_SIZE * (i + 1), HEART_POSY), Vec3f(1,1,0),Vec2f(1,0));
	  	hot.vertex[3+i*4].set(Vec2f(nextHeart + HEART_SIZE * (i + 1), HEART_POSY - HEART_SIZE), Vec3f(1,0,1),Vec2f(1,1));
	 	}else{
		 	hot.vertex[0+i*4].set(Vec2f(nextHeart +	HEART_SIZE * i, HEART_POSY), Vec3f(0,1,1), Vec2f(0.0,0));
	  	hot.vertex[1+i*4].set(Vec2f(nextHeart +	HEART_SIZE * i, HEART_POSY - HEART_SIZE), Vec3f(1,1,1),Vec2f(0.0,1));
		 	hot.vertex[2+i*4].set(Vec2f(nextHeart + HEART_SIZE * (i + 1), HEART_POSY), Vec3f(1,1,0),Vec2f(0.5,0));
		 	hot.vertex[3+i*4].set(Vec2f(nextHeart + HEART_SIZE * (i + 1), HEART_POSY - HEART_SIZE), Vec3f(1,0,1),Vec2f(0.5,1));
		}
	  hot.index[0+ i * 5] = 0 + i * 4;
	  hot.index[1+ i * 5] = 1 + i * 4;
	 	hot.index[2+ i * 5] = 2 + i * 4;
		hot.index[3+ i * 5] = 3 + i * 4;
		hot.index[4+ i * 5] = GLIndex::PrimitiveRestart;
	}
	});
	return m_heartPanel;
}