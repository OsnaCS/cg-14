#include "Environment.hpp"

#include <math.h>


Environment::Environment(Camera& camera)
: m_camera(camera), m_dayLength(200), m_time(m_dayLength/5), m_day(0), m_orbitAngle(0.0), m_phase(0), m_sunAxis(0.5), m_moonAxis(1.0), m_pulse(0.0),  
m_cloudPosition1(10.0f,130.0f,10.0f), m_cloudPosition2(10.0f,130.0f,0.0f), m_cloudPosition3(0.0f,130.0f,10.0f), m_cloudPosition4(0.0f,130.0f,0.0f)  {

}

void Environment::draw(Mat4f viewMat, Mat4f projMat){


	
	m_programSphere.prime([&](HotProgram& hotprog){

		viewMat.setColumn(3, Vec4f(0,0,0,1));

		hotprog.uniform["u_transform"] = projMat * viewMat;
		hotprog.uniform["u_colorUp"] = getcUp();
		hotprog.uniform["u_colorWest"] = getcWest();
		hotprog.uniform["u_colorNorth"] = getcNorth();
		hotprog.uniform["u_colorEast"] = getcEast();
		hotprog.uniform["u_colorSouth"] = getcSouth();
		hotprog.uniform["u_colorDown"] = getcDown();

		hotprog.draw(m_sphere, PrimitiveType::TriangleStrip);

	});

  m_programSun.prime([&](HotProgram& hotprog){

    viewMat.setColumn(3, Vec4f(0,0,0,1));

    Mat4f rotMat = rotationMatrix(quaternionFromAxisAngle(Vec3f(0, -sin(m_sunAxis), cos(m_sunAxis)), m_orbitAngle));
    
    float pulse = m_pulse;
    if(pulse > 1){
    	pulse = 2 - pulse;
    }

    hotprog.uniform["u_pulse"] = pulse;
    hotprog.uniform["u_color"] = getSunColor();
    hotprog.uniform["u_transform"] = projMat * (viewMat * rotMat);
    hotprog.draw(m_sun, PrimitiveType::TriangleStrip);

  });

  m_programMoon.prime([&](HotProgram& hotprog){
  	
    viewMat.setColumn(3, Vec4f(0,0,0,1));

    Mat4f rotMat = rotationMatrix(quaternionFromAxisAngle(Vec3f(0, sin(m_moonAxis), cos(m_moonAxis)), m_orbitAngle));

    Mat4f transMat = translationMatrix(Vec3f(0, -10 * cos(m_moonAxis), 10 * sin(m_moonAxis)));
    Mat4f transbackMat = translationMatrix(Vec3f(0, 10 * cos(m_moonAxis), -10 * sin(m_moonAxis)));

    float angle = m_orbitAngle;
    if(angle > 3.14159265359){
    	angle = -(2 * 3.14159265359 - angle);
    }
    if(angle > 3.14159265359/2){
    	angle = (3.14159265359 - angle);
    }
    if(angle < -3.14159265359/2){
    	angle = (-3.14159265359 - angle);
    }
    angle /= m_moonAxis * (3.14159265359);

    Mat4f rotMoonMat = rotationMatrix(quaternionFromAxisAngle(Vec3f(0, cos(m_moonAxis), -sin(m_moonAxis)), angle));
    Mat4f correct = transbackMat * (rotMoonMat * transMat);

   	hotprog.uniform["u_color"] = getMoonColor();
   	hotprog.uniform["u_phase"] =  m_phase;
    hotprog.uniform["u_transform"] = projMat * (viewMat * (rotMat * correct));
    hotprog.draw(m_moon, PrimitiveType::TriangleStrip);

  });
}

void Environment::drawCloudNormalPass(Mat4f viewMat, Mat4f projMat){
  m_programCloud.prime([&](HotProgram& hotprog){
    hotprog.uniform["u_projection"] = projMat;
    hotprog.uniform["u_view"] = viewMat; 
    hotprog.uniform["u_backPlaneDistance"] = m_camera.getBackPlaneDistance();
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition1;
    hotprog.draw(m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition2;
    hotprog.draw(m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition3;
    hotprog.draw(m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition4;
    hotprog.draw(m_cloud, PrimitiveType::TriangleStrip);
  }); 

}

void Environment::drawCloudFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBufferTex, Tex2D& gBufferDepth){
  m_programFinalCloud.prime([&](HotProgram& hotprog){
    //hotprog.uniform["u_projection"] = projMat;
    //hotprog.uniform["u_view"] = viewMat; 
    

    hotprog.uniform["u_view"] = viewMat;
    hotprog.uniform["u_projection"] = projMat;
    hotprog.uniform["s_lightTexture"] = 0;
    hotprog.uniform["s_depthTexture"] = 1;
    hotprog.uniform["u_winSize"] = m_camera.getWindow().getSize();
    hotprog.uniform["u_backPlaneDistance"] = m_camera.getBackPlaneDistance();
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition1;
    TexCont cont;
    cont.addTexture(0, lBufferTex);
    cont.addTexture(1, gBufferDepth);
    

    cont.prime([&](HotTexCont& hotCont){
      hotprog.draw(hotCont, m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition2;
      hotprog.draw(hotCont, m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition3;
      hotprog.draw(hotCont, m_cloud, PrimitiveType::TriangleStrip);
    hotprog.uniform["u_cloudPosition"] = m_cloudPosition4;
      hotprog.draw(hotCont, m_cloud, PrimitiveType::TriangleStrip);
    });
  });  

}


void Environment::drawLightingPass(Mat4f viewMat, Mat4f projMat, TexCont& gBuffer) {

  m_lightingPass.prime([&](HotProgram& hotProg) {

    hotProg.uniform["normalTexture"] = 0;
    // hotProg.uniform["depthTexture"] = 1;
    // hotProg.uniform["u_cameraPos"] = m_camera.get_position();
    hotProg.uniform["u_lightRay"] = getSkyLightDir();
    hotProg.uniform["u_lightIntens"] = getSkyLightIntensity();
    hotProg.uniform["u_daytime"] = m_time / m_dayLength;

    Vec3f direction = m_camera.get_direction();
    float backPlaneDistance = m_camera.getBackPlaneDistance();
    float viewAngle = m_camera.getViewAngle();
    float screenRatio = m_camera.getScreenRatio();
    Vec3f viewUp = m_camera.getViewUp();

    // vector in the backplane
    Vec3f vd = cross(direction, viewUp).normalize();
    viewUp = cross(vd, direction).normalize();
    float halfBackPlaneHeight = tan(viewAngle / 2) * backPlaneDistance;
    float halfBackPlaneWidth = screenRatio * halfBackPlaneHeight;
    Vec3f backPlaneCenter = direction.normalize() * backPlaneDistance;

    VertexSeq<Vec2f, Vec3f> backPlane;
    backPlane.create(4);

    backPlane.prime([&](HotVertexSeq<Vec2f, Vec3f>& hotV) {
      // oben rechts
      hotV.vertex[0].set(Vec2f(1, 1), backPlaneCenter + (viewUp * halfBackPlaneHeight) + (vd * halfBackPlaneWidth));
      // unten rechts
      hotV.vertex[1].set(Vec2f(1, -1), backPlaneCenter - (viewUp * halfBackPlaneHeight) + (vd * halfBackPlaneWidth));
      // oben links
      hotV.vertex[2].set(Vec2f(-1, 1), backPlaneCenter + (viewUp * halfBackPlaneHeight) - (vd * halfBackPlaneWidth));
      // unten links
      hotV.vertex[3].set(Vec2f(-1, -1), backPlaneCenter - (viewUp * halfBackPlaneHeight) - (vd * halfBackPlaneWidth));
    });

    gBuffer.prime([&](HotTexCont& hotTexCont) {
      hotProg.draw(hotTexCont, backPlane, PrimitiveType::TriangleStrip);
    });
  });
}

void Environment::init()
{

	VShader vsSphere;
	vsSphere.compile(loadShaderFromFile("shader/Environment.vsh"));
	FShader fsSphere;
	fsSphere.compile(loadShaderFromFile("shader/Environment.fsh"));

  // create program and link the two shaders
	m_programSphere.create(vsSphere, fsSphere);

  m_sphere.create(6, 13);
  m_sphere.prime([](HotVertexSeq<Vec3f>& hot)
  {

  	hot.vertex[0].set(Vec3f(0, 1, 0));
  	hot.vertex[1].set(Vec3f(1, 0, 0));
  	hot.vertex[2].set(Vec3f(0, 0, 1));
  	hot.vertex[3].set(Vec3f(-1, 0, 0));
  	hot.vertex[4].set(Vec3f(0, 0, -1));
  	hot.vertex[5].set(Vec3f(0, -1, 0));

  	hot.index[0] = 0;
  	hot.index[1] = 2;
  	hot.index[2] = 1;
  	hot.index[3] = 5;
  	hot.index[4] = 4;
    hot.index[5] = 3;
    hot.index[6] = GLIndex::PrimitiveRestart;
  	hot.index[7] = 5;
  	hot.index[8] = 2;
  	hot.index[9] = 3;
  	hot.index[10] = 0;
  	hot.index[11] = 4;
  	hot.index[12] = 1;

  });

  VShader vsSun;
  vsSun.compile(loadShaderFromFile("shader/Sun.vsh"));
  FShader fsSun;
  fsSun.compile(loadShaderFromFile("shader/Sun.fsh"));

  VShader vsMoon;
  FShader fsMoon;
  vsMoon.compile(loadShaderFromFile("shader/Moon.vsh"));
  fsMoon.compile(loadShaderFromFile("shader/Moon.fsh"));

  //Cloudshader 
  VShader vsCloud;
  FShader fsCloud;
  vsCloud.compile(loadShaderFromFile("shader/CloudNormalPass.vsh"));
  fsCloud.compile(loadShaderFromFile("shader/CloudNormalPass.fsh"));

  VShader vsFinalCloud;
  FShader fsFinalCloud;
  vsFinalCloud.compile(loadShaderFromFile("shader/CloudFinalPass.vsh"));
  fsFinalCloud.compile(loadShaderFromFile("shader/CloudFinalPass.fsh"));

  // create program and link the two shaders
  m_programSun.create(vsSun, fsSun);
  m_programSun.perFragProc.blendFuncRGB = BlendFunction::Add;
  m_programSun.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
  m_programSun.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;

  m_sun.create(4);
  m_sun.prime([&](HotVertexSeq<Vec3f, Vec2f>& hot) {
    
    hot.vertex[0].set(Vec3f( 1, -5*cos(m_sunAxis)-sin(m_sunAxis),-5*sin(m_sunAxis)+cos(m_sunAxis)), Vec2f( 3,  3));
    hot.vertex[1].set(Vec3f(-1, -5*cos(m_sunAxis)-sin(m_sunAxis),-5*sin(m_sunAxis)+cos(m_sunAxis)), Vec2f(-3,  3));
    hot.vertex[2].set(Vec3f( 1, -5*cos(m_sunAxis)+sin(m_sunAxis),-5*sin(m_sunAxis)-cos(m_sunAxis)), Vec2f( 3, -3));
    hot.vertex[3].set(Vec3f(-1, -5*cos(m_sunAxis)+sin(m_sunAxis),-5*sin(m_sunAxis)-cos(m_sunAxis)), Vec2f(-3, -3));

  });

  m_programMoon.create(vsMoon, fsMoon);
  m_programMoon.perFragProc.blendFuncRGB = BlendFunction::Add;
  m_programMoon.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
  m_programMoon.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;



  m_moon.create(4);
  m_moon.prime([&](HotVertexSeq<Vec3f, Vec2f>& hot) {

    hot.vertex[0].set(Vec3f( 1, 10*cos(m_moonAxis)+sin(m_moonAxis),-10*sin(m_moonAxis)+cos(m_moonAxis)), Vec2f( 2,  2));
    hot.vertex[1].set(Vec3f(-1, 10*cos(m_moonAxis)+sin(m_moonAxis),-10*sin(m_moonAxis)+cos(m_moonAxis)), Vec2f(-2,  2));
    hot.vertex[2].set(Vec3f( 1, 10*cos(m_moonAxis)-sin(m_moonAxis),-10*sin(m_moonAxis)-cos(m_moonAxis)), Vec2f( 2, -2));
    hot.vertex[3].set(Vec3f(-1, 10*cos(m_moonAxis)-sin(m_moonAxis),-10*sin(m_moonAxis)-cos(m_moonAxis)), Vec2f(-2, -2));

  });


//Create Cloud for testing
  // create program and link the two shaders
  m_programCloud.create(vsCloud, fsCloud);
  m_programCloud.perFragProc.enableDepthTest();
  m_programCloud.perFragProc.setDepthFunction(DepthFunction::Less);
  m_programCloud.primitiveProc.enableCulling();
  //m_programCloud.primitiveProc.setCullFace(CullFace::Front);
  
  m_programFinalCloud.create(vsFinalCloud, fsFinalCloud);
  m_programFinalCloud.perFragProc.enableDepthTest();
  m_programFinalCloud.perFragProc.enableDepthWrite();
  m_programFinalCloud.perFragProc.setDepthFunction(DepthFunction::Less);
  m_programFinalCloud.primitiveProc.enableCulling();
  m_programFinalCloud.primitiveProc.setCullFace(CullFace::Front);
  m_programFinalCloud.perFragProc.blendFuncRGB = BlendFunction::Add;
  m_programFinalCloud.perFragProc.srcRGBParam = BlendParam::SrcAlpha;
  m_programFinalCloud.perFragProc.dstRGBParam = BlendParam::OneMinusSrcAlpha;
  
  m_cloud = createBox<VAttr::Position, VAttr::Normal, VAttr:: TexUV>(Vec3f(8.0f,2.0f,4.0f));



  // m_programSphere.perFragProc.enableDepthTest();
  // m_programSphere.perFragProc.setDepthFunction(DepthFunction::Less);
  // m_programSun.perFragProc.enableDepthTest();
  // m_programSun.perFragProc.setDepthFunction(DepthFunction::Less);

  VShader lightingVS;
  lightingVS.compile(loadShaderFromFile("shader/EnvironmentLightingPass.vsh"));
  FShader lightingFS;
  lightingFS.compile(loadShaderFromFile("shader/EnvironmentLightingPass.fsh"));

  m_lightingPass.create(lightingVS, lightingFS);
  m_lightingPass.perFragProc.enableDepthTest();
  m_lightingPass.perFragProc.setDepthFunction(DepthFunction::Greater);
}

void Environment::update(float delta)
{

	m_time += delta;

	if(m_time > m_dayLength)
	{
		m_time -= m_dayLength;
		m_day++;
	}

  m_orbitAngle = m_time * 3.14159265359 * 2 / (m_dayLength);

	m_phase += 4 * (delta / m_dayLength) / 29.575;
	if(m_phase > 4){
		m_phase -= 4;
	}

	m_pulse += delta;
	if(m_pulse > 2){
		m_pulse -= 2;
	}

  float help = m_time;
  if(help > m_dayLength / 2){

    help = m_dayLength - help;
  }

  m_cloudPosition1.x = help;
  m_cloudPosition1.z = help;

  m_cloudPosition2.x = -help;
  m_cloudPosition2.z = help;

  m_cloudPosition3.x = help;
  m_cloudPosition3.z = -help;

  m_cloudPosition4.x = -help;
  m_cloudPosition4.z = -help;
}

void Environment::setDayLength(float sec) 
{ 

	m_dayLength = sec; 
}

Vec3f Environment::getSkyLightColor(){

	if(m_time <= 0.75 * m_dayLength && m_time > 0.25 * m_dayLength){

		return getSunColor();
	} else{
		return getMoonColor();
	}

}
float Environment::getSkyLightIntensity(){

	if(m_time <= 0.75 * m_dayLength && m_time > 0.25 * m_dayLength){

		return getSunIntensity();
	} else{
		return getMoonIntensity();
	}
}
Vec3f Environment::getSkyLightDir(){

	if(m_time <= 0.75 * m_dayLength && m_time > 0.25 * m_dayLength){

		return -getSunPos();
	} else{
		return -getMoonPos();
	}
}





Vec3f Environment::getSunColor()
{

	float r, g, b;
	float help;

	r=0;
	g=0;
	b=0;

	if(m_time > 0.75 * m_dayLength) 
	{

		help = m_time - 0.75 * m_dayLength;
		help = help /(m_dayLength/4);
		r = 1 - help;
		g = 0.3 - 0.3 * help;

	} 
	else if(m_time > 0.5 * m_dayLength) 
	{

		help = m_time - 0.5 * m_dayLength;
		help = help /(m_dayLength/4);
		help *= help;
		help *= help;
		r = 1;
		g = 1 - 0.7 * help;
		b = 0.75 * (1 - help);

	} 
	else if(m_time > 0.25 * m_dayLength) 
	{

		help = m_time - 0.25 * m_dayLength;
		help = help /(m_dayLength/4);
		help = 1 - help;
		help *= help;
		help *= help;
		help = 1- help;

		r = 1;
		g = 0.3 + 0.7 * help;
		b= 0.75 * help;

	} 
	else 
	{

		help = m_time / (m_dayLength/4);
		r = help;
		g = 0.3 * help;

	}

	return Vec3f(r,g,b);

}

float Environment::getSunIntensity(){

	float help = m_time / m_dayLength;
	float maxIntens = 0.9;

	if(help >= 0.35 && help < 0.65){

		return maxIntens;

	} else if(help >= 0.25 && help < 0.35){

		help = (help - 0.25) * 10;
		return maxIntens * help;

	} else if(help >= 0.65 && help < 0.75){	

		help = (0.75 - help) * 10;
		return maxIntens * help;

	} else {
		return 0.0;
	}
}

Vec3f Environment::getSunPos(){

	return Vec3f(sin(m_orbitAngle), -cos(m_orbitAngle) * cos(m_sunAxis), -cos(m_orbitAngle) * sin(m_sunAxis));
}

Vec3f Environment::getMoonColor(){
	return Vec3f(0.75, 0.75, 0.75);
}
float Environment::getMoonIntensity(){
	
	float help = m_time / m_dayLength;
	float maxIntens = m_phase;

	if(maxIntens > 2){
		maxIntens = 4 - maxIntens;
	}
	maxIntens = (1 - (maxIntens / 2)) * 0.3;

	if(help < 0.15 || help > 0.85){

		return maxIntens;

	} else if(help >= 0.15 && help < 0.25){

		help = (0.25 - help) * 10;
		return maxIntens * help;

	} else if(help > 0.75 && help <= 0.85){	

		help = (help - 0.75) * 10;
		return maxIntens * help;

	} else {
		return 0.0;
	}
}
Vec3f Environment::getMoonPos() { 

	return Vec3f(-sin(m_orbitAngle), cos(m_orbitAngle) * cos(m_moonAxis), -cos(m_orbitAngle) * sin(m_moonAxis));
}

Vec3f Environment::getcUp()
{

	float help = 4 * m_time / m_dayLength;

	if(help > 3)
	{

		help = help - 3;
		help = 1 - help;
		help *= help /2; 

	} else if(help > 2)
	{
		help = help -2;
		help *= help / 2;
		help = 1 - help; 

	} else if(help > 1)
	{

		help = help - 1;
		help = 1 - help;
		help *= help / 2;
		help = 1 - help;

	} else {

    help *= help / 2;
  }


	return Vec3f(0, 0, 0.8 * help);

}

Vec3f Environment::getcWest()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *= help;

    return Vec3f(0.15 * help, 0.15 * help, 0.4 * help);

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);
    help *= help;

    return Vec3f(0.75 - (0.6 * help), 0.75 - (0.6 * help), 1 - 0.6 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *= help;
    help = 1 - help;

    return Vec3f(0.5 + (0.25 * help), 0.2 + 0.55 * help, help);

  } 
  else 
  {
    help = m_time;
    help = help / (m_dayLength / 4);
    help *= help;
    return Vec3f(0.5 * help, 0.2 * help, 0);
  }
}

Vec3f Environment::getcNorth()
{
	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *=help;

    return Vec3f(0.15 * help, 0.15 * help, 0.4 * help);

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);
    help *=help;

    return Vec3f(0.75 - (0.6 * help), 0.75 - (0.6 * help), 1 - 0.6 * help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *=help;
    help = 1 - help;

    return Vec3f(0.15 + (0.6 * help), 0.15 + (0.6 * help), 0.4 + (0.6 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);
    help *= help;

    return Vec3f(0.25 * help, 0.25 * help, 0.5 * help);

  }

}

Vec3f Environment::getcEast()
{

	float help;

  if(m_time > 0.75 * m_dayLength) 
  {

    help = m_time - 0.75 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *=help;

    return Vec3f(0.5 * help, 0.2 * help, 0);

  } 
  else if(m_time > 0.5 * m_dayLength) 
  {

    help = m_time - 0.5 * m_dayLength;
    help = help / (m_dayLength / 4);
    help *= help;

    return Vec3f(0.75 - (0.25 * help), 0.75 - (0.55 * help), 1 - help);

  } 
  else if(m_time > 0.25 * m_dayLength) 
  {

    help = m_time - 0.25 * m_dayLength;
    help = help / (m_dayLength / 4);
    help = 1 - help;
    help *= help;
    help = 1- help;

    return Vec3f(0.15 + (0.6 * help), 0.15 + (0.6 * help), 0.4 + (0.6 * help));

  } 
  else 
  {

    help = m_time;
    help = help / (m_dayLength / 4);
    help *=help;

    return Vec3f(0.15 * help, 0.15 * help, 0.4 * help);

  }
}

Vec3f Environment::getcSouth()
{

	return getcNorth();

}

Vec3f Environment::getcDown()
{

	return Vec3f(0,0,0);

}
