#pragma once

#include <map>
#include <set>

#include "ChunkView.hpp"
#include "Map.hpp"
#include "Camera.hpp"
#include "Environment.hpp"

class MapView
{

public:

	MapView(Map& map, Camera& cam, Environment& envir);
	void drawNormalPass(Mat4f viewMat, Mat4f projMat);
	void drawLightingPass(Mat4f viewMat, Mat4f projMat, TexCont& gBuffer);
	void drawFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBuffer, Tex2D& dBuffer);
	void init();
	void notifyBlockUpdate(Vec3i blockPos);
	void deleteChunkView(Vec2i chunkPos);
	void resetMapView();

	/**
	* Checks if a chunk is visible
	*/
	bool isChunkVisible(Vec2i& chunkPos);
	bool exists(Vec2i pos);
	void clearMapView(Vec2i position);
	int size(){return m_mapView.size();}
	

private:

	void drawChunks(HotProgram& hotP, HotTexCont& hotTexCont);
	set<Vec3f> getVisibleTorches();
	Mat4f getTorchTransformationMatrix();

	map<Vec2i, ChunkView> m_mapView;
	Map& m_map;
	Camera& m_cam;
	Environment& m_envir;
	Tex2D m_colorTexture;
	Program m_program;
	Program m_normalPass;
	Program m_normalPassTorches;
	Program m_finalPass;
	Program m_finalPassTorches;
	Tex2D m_normalTexture;
	Tex2D m_pickaxeTexture;
	Program m_lightingPass;
	int m_visibleChunkRange;
	VertexSeq<Vec3f, Vec3f, Vec2f> m_torch;
	Tex2D m_torchTexture;
};