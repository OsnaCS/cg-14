#pragma once

#include "Chunk.hpp"
#include "lumina/lumina.hpp"
#include "Map.hpp"

#include <array>

/**
 * @brief Graphical representation of Chunk
 */
class ChunkView {
public:
	ChunkView();
	void init(Vec2i index, Map& map);
	void draw(HotProgram& hotProg, HotTex2D& hotTex);
	void updateView();

private:
	Vec2i m_index;
	Map* m_map;
	std::array<VertexSeq, 8> m_chunkSequences;

	void addBoxToSeq(HotVertexSeq<Vec3f, Vec3f, Vec2f>& hotSeq, uint& vertexIndex, uint& indexIndex, BlockType& block, Vec3f& cubePos);
	bool isBoxVisible(Vec3f& cubePos);
};