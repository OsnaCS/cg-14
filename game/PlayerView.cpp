#include "PlayerView.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "ObjectLoader.hpp"
#include "Camera.hpp"
#include "lumina/util/Transformation.hpp"
#include <map>
#include "BlockType.hpp"

using namespace std;
using namespace lumina;

const float HEART_SIZE = 0.05f;
const float HEART_POSY = -0.7f; 

PlayerView::PlayerView(Player& player)
    : m_player(player)
{

}
 
void PlayerView::draw( )
{

	m_program.prime([&](HotProgram& hotprog)
	{
        m_colorTexture.prime(0, [&](HotTex2D& hotTex)
        {
			hotprog.draw(hotTex, updateHearts(), PrimitiveType::TriangleStrip);
		});
	});	

    m_markerProgram.prime([this](HotProgram& hotprog)
    {
        hotprog.draw(m_centerMarker, PrimitiveType::Line);
    });

    m_inventoryProg.prime([&,this](HotProgram& hotprog)
    {
        m_inventoryTex.prime(0, [&,this](HotTex2D& hotTex)
        {
            hotprog.draw(hotTex, updateInventory(), PrimitiveType::TriangleStrip);
        });
    });

}


void PlayerView::init()
{
    // Pickaxe
	//Initialize of the texture
	ImageBox image_box2 = loadJPEGImage("gfx/pickaxe_texture512.jpg");
  m_pickaxeTexture.create(Vec2i(512,512), TexFormat::RGB8, image_box2.data());
  m_pickaxeTexture.params.filterMode = TexFilterMode::Linear;
  m_pickaxeTexture.params.useMipMaps = true;

  // ImageBox image_box3 = loadJPEGImage("gfx/pickaxe_normals512.jpg");
  // m_pickaxeNormals.create(Vec2i(512,512), TexFormat::RGB8, image_box3.data());
  // m_pickaxeNormals.params.filterMode = TexFilterMode::Linear;
  // m_pickaxeNormals.params.useMipMaps = true;

	VShader vsNP;
	vsNP.compile(loadShaderFromFile("shader/PickaxeNormalPass.vsh"));
	FShader fsNP;
	fsNP.compile(loadShaderFromFile("shader/PickaxeNormalPass.fsh"));
    // create program and link the two shaders
	m_normalPass.create(vsNP, fsNP);
	m_normalPass.perFragProc.enableDepthTest();
	m_normalPass.perFragProc.setDepthFunction(DepthFunction::Less);

	VShader vsFP;
	vsFP.compile(loadShaderFromFile("shader/PickaxeFinalPass.vsh"));
	FShader fsFP;
	fsFP.compile(loadShaderFromFile("shader/PickaxeFinalPass.fsh"));
    // create program and link the two shaders
	m_finalPass.create(vsFP, fsFP);
	m_finalPass.perFragProc.enableDepthTest();
	m_finalPass.perFragProc.setDepthFunction(DepthFunction::Less);

	m_pickaxe = loadOBJ("gfx/pickaxe.obj");


    // Display hearts panel
    initHeartPanel();

    // For center-marker at the center of the screen
    initCenterMarker();

    // Display player's inventory
    initInventory();

}

VertexSeq<Vec2f, Vec2f> PlayerView::updateInventory()
{
    const map<BlockType, int> items = m_player.getInventoryItems();
    const Vec2f INIT_POS(-0.8f, -0.85f);
    const float ITEM_SIZE = 0.07f;

    VertexSeq<Vec2f, Vec2f> inventoryPanel;
    inventoryPanel.create(4*items.size(), 5*m_player.maxDisplayItems() );
    inventoryPanel.prime([&](HotVertexSeq<Vec2f, Vec2f>& hot)
    {
        auto it=items.begin();
        for ( int i=0;  it != items.end(); ++it, ++i )
        {
            Vec2f texCoord = getTexCoords(it->first, BlockSide::North);
            hot.vertex[i*4+0].set( Vec2f(INIT_POS.x+  i  *ITEM_SIZE, INIT_POS.y), texCoord );
            hot.vertex[i*4+1].set( Vec2f(INIT_POS.x+(i+1)*ITEM_SIZE, INIT_POS.y), texCoord + Vec2f(1/8.f, 0) );
            hot.vertex[i*4+2].set( Vec2f(INIT_POS.x+(i+1)*ITEM_SIZE, INIT_POS.y-ITEM_SIZE)
                                                                                , texCoord+Vec2f(1/8.f, 1/8.f) );
            hot.vertex[i*4+3].set( Vec2f(INIT_POS.x+   i *ITEM_SIZE, INIT_POS.y-ITEM_SIZE)
                                                                                , texCoord+Vec2f(0, 1/8.f) );
            hot.index[i*5+0] = i*4+0;
            hot.index[i*5+1] = i*4+1;
            hot.index[i*5+2] = i*4+2;
            hot.index[i*5+3] = i*4+3;
            hot.index[i*5+4] = GLIndex::PrimitiveRestart;
        }
    });
    return inventoryPanel;
}


VertexSeq<Vec2f, Vec3f, Vec2f> PlayerView::updateHearts(){

  int activeHearts = m_player.getHearts();
  int maxHearts = m_player.getMaxHearts();
  float nextHeart = -(maxHearts / 2.0f) * HEART_SIZE;
  VertexSeq<Vec2f, Vec3f, Vec2f> heartPanel;
  heartPanel.create(4 * maxHearts, 5 * maxHearts);
  heartPanel.prime([&](HotVertexSeq<Vec2f, Vec3f, Vec2f>& hot)
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
    return heartPanel;
}

void PlayerView::drawNormalPass(Mat4f viewMat, Mat4f projMat) {

  //Show pickaxe
	m_normalPass.prime([&](HotProgram& hotPickaxe){

		hotPickaxe.uniform["u_view"] = viewMat * translationMatrix(m_player.getPosition() + 2*m_player.getDirection() ) * scalingMatrix(Vec3f(0.25,0.25,0.25));
		hotPickaxe.uniform["u_projection"] = projMat;

		TexCont cont; 
		cont.addTexture(0, m_pickaxeTexture);
		cont.prime([&](HotTexCont& hotTexCont){
				hotPickaxe.draw(hotTexCont, m_pickaxe, PrimitiveType::Triangle);	
		});
	});
}

void PlayerView::drawFinalPass(Mat4f viewMat, Mat4f projMat, Camera cam, Tex2D& lBuffer) {

  m_finalPass.prime([&](HotProgram& hotP) {
  	
  	//Vec3f cross1 = cross(m_player.getPosition(), m_player.getDirection());
    hotP.uniform["u_view"] = viewMat * translationMatrix(m_player.getPosition() + 2*m_player.getDirection() ) * scalingMatrix(Vec3f(0.25,0.25,0.25));
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

void PlayerView::initInventory()
{

    ImageBox image_box = loadJPEGImage("gfx/texture_inventory.jpg");
    m_inventoryTex.create(Vec2i(512,512), TexFormat::RGB8, image_box.data());
    m_inventoryTex.params.filterMode = TexFilterMode::Trilinear;
    m_inventoryTex.params.useMipMaps = true;

    VShader vs;
    vs.compile(loadShaderFromFile("shader/PlayerInventory.vsh"));
    FShader fs;
    fs.compile(loadShaderFromFile("shader/PlayerInventory.fsh"));
    m_inventoryProg.create(vs, fs);

}

void PlayerView::initCenterMarker()
{
    m_centerMarker.create(4, 4);
    m_centerMarker.prime([](HotVertexSeq<Vec2f, Vec3f>& hot)
    {
      /*color may change against the background...now,it is onl white cross.*/
      hot.vertex[0].set( Vec2f(-0.04, 0.0), Vec3f(1,1,1) );
      hot.vertex[1].set( Vec2f( 0.04, 0.0), Vec3f(1,1,1));
      hot.vertex[2].set( Vec2f( 0.0,-0.05), Vec3f(1,1,1));
      hot.vertex[3].set( Vec2f( 0.0, 0.05), Vec3f(1,1,1));

      hot.index[0] = 0;
      hot.index[1] = 1;
      hot.index[2] = 2;
      hot.index[3] = 3;
    });

    VShader vsCenterMarker;
    vsCenterMarker.compile(loadShaderFromFile("shader/CenterMarker.vsh"));
    FShader fsCenterMarker;
    fsCenterMarker.compile(loadShaderFromFile("shader/CenterMarker.fsh"));
    m_markerProgram.create(vsCenterMarker, fsCenterMarker);

}

void PlayerView::initHeartPanel()
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
    m_program.create(vs, fs);
    m_program.perFragProc.blendFuncRGB = BlendFunction::Add;
    m_program.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
    m_program.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;
}
