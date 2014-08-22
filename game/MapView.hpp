#pragma once

#include <map>

#include "ChunkView.hpp"
#include "Map.hpp"
#include "Camera.hpp"

class MapView
{

public:

	MapView(Map& map, Camera& cam);
	void drawNormalPass(Mat4f viewMat, Mat4f projMat);
	void drawFinalPass(Mat4f viewMat, Mat4f projMat, Tex2D& lBuffer);
	void init();
	void deleteChunkView(Vec2i chunkPos);

	/**
	* Checks if a chunk is visible
	*/
	bool isChunkVisible(Vec2i& chunkPos);
	bool exists(Vec2i pos);
	void clearMapView(Vec2i position);
	int size(){return m_mapView.size();}
	

private:

	void drawChunks(HotProgram& hotP, HotTexCont& hotTexCont);

	map<Vec2i, ChunkView> m_mapView;
	Map& m_map;
	Camera& m_cam;
	Tex2D m_colorTexture;
	Program m_program;
	Program m_normalPass;
	Program m_finalPass;
	Tex2D m_normalTexture;
};