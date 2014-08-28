#include "PlayerView.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "ObjectLoader.hpp"
#include "Camera.hpp"
#include "lumina/util/Transformation.hpp"

using namespace std;
using namespace lumina;

const float HEART_SIZE = 0.05f;
const float HEART_POSY = -0.7f; 

PlayerView::PlayerView(Player& player)
    : m_player(player)
{

}
 
void PlayerView::draw(Mat4f viewMatrix, Mat4f projectionMatrix)
{

	m_program.prime([&](HotProgram& hotprog)
	{
 		m_colorTexture.prime(0, [&](HotTex2D& hotTex) {
			hotprog.draw(hotTex, updateHearts(), PrimitiveType::TriangleStrip);
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

  ImageBox image_box3 = loadJPEGImage("gfx/player_texture.jpg");
  m_playerTexture.create(Vec2i(512,512), TexFormat::RGB8, image_box3.data());
  m_playerTexture.params.filterMode = TexFilterMode::Linear;
  m_playerTexture.params.useMipMaps = true;

	VShader vsNP;
	vsNP.compile(loadShaderFromFile("shader/PickaxeNormalPass.vsh"));
	FShader fsNP;
	fsNP.compile(loadShaderFromFile("shader/PickaxeNormalPass.fsh"));
  // create program and link the two shaders
	m_normalPass.create(vsNP, fsNP);
	m_normalPass.perFragProc.enableDepthTest();
	m_finalPass.perFragProc.enableDepthWrite();
	m_normalPass.perFragProc.setDepthFunction(DepthFunction::Less);

	VShader vsFP;
	vsFP.compile(loadShaderFromFile("shader/PickaxeFinalPass.vsh"));
	FShader fsFP;
	fsFP.compile(loadShaderFromFile("shader/PickaxeFinalPass.fsh"));
  // create program and link the two shaders
	m_finalPass.create(vsFP, fsFP);
	m_finalPass.perFragProc.enableDepthTest();
	m_finalPass.perFragProc.enableDepthWrite();
	m_finalPass.perFragProc.setDepthFunction(DepthFunction::Less);

	m_pickaxe = loadOBJ("gfx/pickaxe.obj");
	m_playerFigure = loadOBJ("gfx/player.obj");

	m_showPickaxe = false;
	
}

VertexSeq<Vec2f, Vec3f, Vec2f> PlayerView::updateHearts(){
  
  int activeHearts = m_player.getHearts();
  int maxHearts = m_player.getMaxHearts();
  float nextHeart = -(maxHearts / 2.0f) * HEART_SIZE;
 	VertexSeq<Vec2f, Vec3f, Vec2f> m_heartPanel;
	m_heartPanel.create(4 * maxHearts, 5 * maxHearts);
  m_heartPanel.prime([&](HotVertexSeq<Vec2f, Vec3f, Vec2f>& hot)
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

void PlayerView::drawNormalPass(Mat4f viewMat, Mat4f projMat, Camera& cam) {

	m_normalPass.prime([&](HotProgram& hotPlayer) {
		Mat4f tmp = getRotMatrixPlayer();
		tmp = tmp.inverted();
    tmp = tmp.transposed();
		hotPlayer.uniform["u_view"] = viewMat* getTransMatrixPlayer();
    hotPlayer.uniform["u_projection"] = projMat;
    hotPlayer.uniform["u_transform"] = tmp;
    hotPlayer.uniform["u_backPlaneDistance"] = cam.getBackPlaneDistance();

    TexCont contPlayer;
    contPlayer.addTexture(0, m_playerTexture);
      contPlayer.prime([&](HotTexCont& hotTexCont) { 
      	hotPlayer.draw(hotTexCont, m_playerFigure, PrimitiveType::Triangle); 
      });
  });


  if(m_showPickaxe) {
    // Show pickaxe
    m_normalPass.prime([&](HotProgram& hotPickaxe) {
    	Mat4f tmp = getRotMatrixPickaxe();
			tmp = tmp.inverted();
   	  tmp = tmp.transposed();
      hotPickaxe.uniform["u_view"] = viewMat*getTransMatrix();
      hotPickaxe.uniform["u_projection"] = projMat;
      hotPickaxe.uniform["u_transform"] = tmp;
      hotPickaxe.uniform["u_backPlaneDistance"] = cam.getBackPlaneDistance();

      TexCont cont;
      cont.addTexture(0, m_pickaxeTexture);
      cont.prime([&](HotTexCont& hotTexCont) { 
      	hotPickaxe.draw(hotTexCont, m_pickaxe, PrimitiveType::Triangle); 
      });
    });
  }
}

void PlayerView::drawFinalPass(Mat4f viewMat, Mat4f projMat, Camera& cam, Tex2D& lBuffer) {

		m_finalPass.prime([&](HotProgram& hotPlayer) {

      hotPlayer.uniform["u_view"] = viewMat * getTransMatrixPlayer();
      hotPlayer.uniform["u_projection"] = projMat;
      hotPlayer.uniform["u_winSize"] = cam.getWindow().getSize();
      hotPlayer.uniform["s_lightTexture"] = 0;
      hotPlayer.uniform["s_colorTexture"] = 1;

      lBuffer.prime(0, [&](HotTex2D& hotLightingTex) {
        m_playerTexture.prime(1, [&](HotTex2D& hotTex) {
          HotTexCont hotTexContPlayer(hotLightingTex, hotTex);
          hotPlayer.draw(hotTexContPlayer, m_playerFigure, PrimitiveType::Triangle);
        });
      });
    });

  if(m_showPickaxe) {
    m_finalPass.prime([&](HotProgram& hotP) {

      hotP.uniform["u_view"] = viewMat * getTransMatrix();
      hotP.uniform["u_projection"] = projMat;
      hotP.uniform["u_winSize"] = cam.getWindow().getSize();
      hotP.uniform["s_lightTexture"] = 0;
      hotP.uniform["s_colorTexture"] = 1;

      lBuffer.prime(0, [&](HotTex2D& hotLightingTex) {
        m_pickaxeTexture.prime(1, [&](HotTex2D& hotTex) {
          HotTexCont hotTexCont(hotLightingTex, hotTex);
          hotP.draw(hotTexCont, m_pickaxe, PrimitiveType::Triangle);
        });
      });
    });
  }
}

Mat4f PlayerView::getTransMatrixPlayer(){

	//Scaling
		Mat4f scaling = scalingMatrix(Vec3f(0.6,0.6,0.6));

		//Translation to origin
		Mat4f translationOrigin = translationMatrix(Vec3f(0,0,0));

		//Rotation Y-axis in player direction
    float angle;
    Vec3f direc = m_player.getDirection();
    direc.y = 0;
		float cosinus = dot(Vec3f(0.0f, 0.0f, -1.0f), direc)/direc.length();
		angle = acos(cosinus);
		if(m_player.getDirection().x >0) {
			angle = -angle;
		}

  	Mat4f rotationY = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), angle+1.57f));

  	//Translation to player height and to the front right
  	Mat4f translation = translationMatrix(m_player.getPosition() - m_player.getDirection() + Vec3f(0, -1.1, 0));


  	return translation * rotationY * translationOrigin * scaling;

}

Mat4f PlayerView::getRotMatrixPlayer(){
	//Rotation Y-axis in player direction
    float angle;
    Vec3f direc = m_player.getDirection();
    direc.y = 0;
		float cosinus = dot(Vec3f(0.0f, 0.0f, -1.0f), direc)/direc.length();
		angle = acos(cosinus);
		if(m_player.getDirection().x >0) {
			angle = -angle;
		}
	return rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), angle+1.57f));
}

Mat4f PlayerView::getTransMatrix(){
	//Scaling
		Mat4f scaling = scalingMatrix(Vec3f(0.15,0.15,0.15));

		//Rotation Y-axis in player direction
    float angle;
    Vec3f direc = m_player.getDirection();
    direc.y = 0;
		float cosinus = dot(Vec3f(0.0f, 0.0f, -1.0f), direc)/direc.length();
		angle = acos(cosinus);
		if(m_player.getDirection().x >0) {
			angle = -angle;
		}
  	Mat4f rotationY = rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), angle));

  	//Translation to player height and to the front right
		Vec3f dir = m_player.getDirection().normalize();
  	Vec3f cross1 = -cross(Vec3f(0,1,0), dir);
  	cross1.normalize();
  	Mat4f translation = translationMatrix(m_player.getPosition() + 2*m_player.getDirection() + 0.75f*cross1);

  	return translation * rotationY * scaling;

}

Mat4f PlayerView::getRotMatrixPickaxe(){
	//Rotation Y-axis in player direction
    float angle;
    Vec3f direc = m_player.getDirection();
    direc.y = 0;
		float cosinus = dot(Vec3f(0.0f, 0.0f, -1.0f), direc)/direc.length();
		angle = acos(cosinus);
		if(m_player.getDirection().x >0) {
			angle = -angle;
		}
  	return rotationMatrix(quaternionFromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), angle));
}

EventResult PlayerView::processEvent(InputEvent& e, Window& win, bool cheatmode) {

  if(e.type == InputType::KeyPressed) {
    if((KeyCode)(e.keyInput.key) == KeyCode::T &&  !cheatmode) {
      togglePickaxe();
    }else if((KeyCode)(e.keyInput.key) == KeyCode::K){
    	m_showPickaxe = false;
    }
  }
  return EventResult::Skipped;
}

void PlayerView::togglePickaxe(){
	m_showPickaxe = !m_showPickaxe;
}