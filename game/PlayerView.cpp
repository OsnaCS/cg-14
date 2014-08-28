#include "PlayerView.hpp"
#include "lumina/io/ImageJPEG.hpp"
#include "ObjectLoader.hpp"
#include "Camera.hpp"
#include "lumina/util/Transformation.hpp"
#include "BlockType.hpp"

#include <map>

using namespace std;
using namespace lumina;



// Initial position of the inventory panel. Now, it is set at the bottom-left of the screen.
const Vec2f INIT_INVENT_POS(-0.8f, -0.85f);

// Size of each inventory item in the panel.
const Vec2f INVENT_ITEM_SIZE(0.05f, 0.08f);


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


  if (  m_player.getInventoryItems().empty() )
     return;

  m_inventoryProg.prime([&,this](HotProgram& hotprog)
    {
    m_inventoryTex.prime(0, [&,this](HotTex2D& hotTex)
    {
      hotprog.draw(hotTex, updateInventory(), PrimitiveType::TriangleStrip);
    });
  });
  m_numberProg.prime([&](HotProgram& numHotProg)
  {
    m_numberTex.prime(0, [&](HotTex2D& numHotTex)
    {
      numHotProg.draw(numHotTex, updateInventoryNumbers(), PrimitiveType::TriangleStrip);
    });
  });
}


void PlayerView::init()
{
    // Pickaxe
	//Initialize of the texture
	ImageBox image_box2 = loadJPEGImage("gfx/pickaxe_texture.jpg");
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

    // To display hearts panel
    initHeartPanel();

    // For center-marker at the center of the screen
    initCenterMarker();

    // To display player's inventory
    initInventory();

    // To display number of inventory for each object's type
    initInventoryNumber();

    

}

VertexSeq<Vec2f, Vec2f> PlayerView::updateInventory()
{
    const map<BlockType, int> items = m_player.getInventoryItems();

    VertexSeq<Vec2f, Vec2f> inventoryPanel;
    inventoryPanel.create(4*items.size(), 5*items.size() );
    inventoryPanel.prime([&](HotVertexSeq<Vec2f, Vec2f>& hot)
    {
        auto it=items.begin();
        for ( int i=0;  it != items.end(); ++it, ++i )
        {
            Vec2f texCoord = getInventoryCoords(it->first, BlockSide::North);
            hot.vertex[i*4+0].set( Vec2f(INIT_INVENT_POS.x+  i  *INVENT_ITEM_SIZE.x, INIT_INVENT_POS.y), texCoord );
            hot.vertex[i*4+2].set( Vec2f(INIT_INVENT_POS.x+(i+1)*INVENT_ITEM_SIZE.x, INIT_INVENT_POS.y), texCoord + Vec2f(1/8.f, 0) );
            hot.vertex[i*4+3].set( Vec2f(INIT_INVENT_POS.x+(i+1)*INVENT_ITEM_SIZE.x, INIT_INVENT_POS.y-INVENT_ITEM_SIZE.y)
                                                                                , texCoord+Vec2f(1/8.f, 1/8.f) );
            hot.vertex[i*4+1].set( Vec2f(INIT_INVENT_POS.x+   i *INVENT_ITEM_SIZE.x, INIT_INVENT_POS.y-INVENT_ITEM_SIZE.y)
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

VertexSeq<Vec2f, Vec3f, Vec2f> PlayerView::updateInventoryNumbers()
{

    // Initial position of the "number" is on the bottom-right of th+e inventory texture.
    Vec2f init_pos = Vec2f(INIT_INVENT_POS.x + INVENT_ITEM_SIZE.x/2, INIT_INVENT_POS.y-INVENT_ITEM_SIZE.y/2);
    // Item size of number texture
    const Vec2f item_size = Vec2f(INVENT_ITEM_SIZE.x/2, INVENT_ITEM_SIZE.y/2);
    // Gap for placing second digit(left). Otherwise the space between 2 digits of the same number will be too wide.
    const Vec2f gap = Vec2f(INVENT_ITEM_SIZE.x/4, INVENT_ITEM_SIZE.y/4);

    const map<BlockType, int> items = m_player.getInventoryItems();
    VertexSeq<Vec2f, Vec3f, Vec2f>  numInventory;
    int seqSize = calcSeqSize(items);
    numInventory.create(4*seqSize, 5*seqSize );
    numInventory.prime([&](HotVertexSeq<Vec2f, Vec3f, Vec2f>& hot)
    {
        auto it=items.begin();
        int cnt = 0; int cntIdx = 0;
        for ( int i=0;  it != items.end(); ++i, ++it )
        {
            vector<Vec2f> positions = positionOfNumber( it->second );

            auto pos_it = positions.begin();
            for ( int j=0; pos_it != positions.end(); ++pos_it, ++j )
            {

                cnt += j*4;
                cntIdx += j*5;
                float offsetX = -2*j*item_size.x+j*gap.x; // if j=1, shift position[1] to the left as second digit number           
                hot.vertex[i*4+0+cnt].set( Vec2f(init_pos.x+    i*item_size.x + offsetX, init_pos.y), Vec3f(1,1,1)
                                       , positions[j] );
                hot.vertex[i*4+2+cnt].set( Vec2f(init_pos.x+(i+1)*item_size.x + offsetX, init_pos.y), Vec3f(1,1,1)
                                       , positions[j] + Vec2f(1/10.f, 0) );
                hot.vertex[i*4+3+cnt].set( Vec2f(init_pos.x+(i+1)*item_size.x + offsetX, init_pos.y-item_size.y), Vec3f(1,1,1)
                                        , positions[j]+Vec2f(1/10.f, 1.0f) );
                hot.vertex[i*4+1+cnt].set( Vec2f(init_pos.x+    i*item_size.x + offsetX, init_pos.y-item_size.y), Vec3f(1,1,1)
                                        , positions[j]+Vec2f(0, 1.0f) );       
                init_pos.x += (j==1)?0:INVENT_ITEM_SIZE.x/2; // update  initial position of number block.
                hot.index[i*5+0+cntIdx] = i*4+0+cnt;
                hot.index[i*5+1+cntIdx] = i*4+1+cnt;
                hot.index[i*5+2+cntIdx] = i*4+2+cnt;
                hot.index[i*5+3+cntIdx] = i*4+3+cnt;
                hot.index[i*5+4+cntIdx] = GLIndex::PrimitiveRestart;

            }

        }
    });
    return numInventory;

}

VertexSeq<Vec2f, Vec3f, Vec2f> PlayerView::updateHearts()
{

   const float HEART_SIZE = 0.05f;
   const float HEART_POSY = -0.7f;

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
      /*color may change against the background...now,it is only white cross.*/
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

void PlayerView::initInventoryNumber()
{
    // Initialize numbers to draw on top of the texture inventory for counting
    ImageBox image_box = loadPNGImage("gfx/number.png");
    m_numberTex.create(Vec2i(320,32), TexFormat::RGBA8, image_box.data());
    m_numberTex.params.filterMode = TexFilterMode::Trilinear;
    m_numberTex.params.useMipMaps = true;

    VShader vsNum;
    vsNum.compile(loadShaderFromFile("shader/InventoryNumbers.vsh"));
    FShader fsNum;
    fsNum.compile(loadShaderFromFile("shader/InventoryNumbers.fsh"));
    m_numberProg.create(vsNum, fsNum);
    m_numberProg.perFragProc.blendFuncRGB = BlendFunction::Add;
    m_numberProg.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
    m_numberProg.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;

}


std::vector<Vec2f> PlayerView::positionOfNumber(int num)
{
    // We have 10 numbers listed in the texture and we want the position of the
    // corresponding number from the listed number. [0 1 2 .. 9].
    // The postion of the texture will be given from the "top-left" referred at (0,0) of the image.
    // Subsequenced image will be at (1/10)
    // If number is greater than 9, e.g. 10, 0 will be placed at vector[0] and 1 is placed at vector[1].

     std::vector<Vec2f> positions;
     Vec2f pos(0.0, 0.0);
     if ( num < 10 )
     {
          positions.push_back( Vec2f(pos.x+ num*1.0f/10, pos.y) );
     }
     else if ( num>=10 && num<=99)
     {
          string text = to_string(num);
          // text[0] and text[1] are swapped oder after conversion. 10 yields text[1]=0 and text[0]=1
          positions.push_back( Vec2f(pos.x+ (int)(toDigit(text[1]))*1.0f/10, pos.y) );
          positions.push_back( Vec2f(pos.x+ (int)(toDigit(text[0]))*1.0f/10, pos.y) );
     }
     else {
         // The inventory panel will not display the number of the corresponding object.
         // slog("Not implemented. Because number of inventory of this particular object > 99");
     }

  return positions;
}

int PlayerView::calcSeqSize(const std::map<BlockType, int>& items)
{
    int size = 0;
    for(auto it = items.begin(); it != items.end(); ++it)
    {
        size += ( it->second > 9 )? 2: 1; // 1 or 2 digit place
    }
    return size;
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

