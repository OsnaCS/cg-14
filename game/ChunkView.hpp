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
	void draw(HotProgram& hotProg, HotTexCont& hotCont);
	void updateView();

private:
	Vec2i m_index;
	Map* m_map;
	std::array<VertexSeq<Vec3f, Vec3f, float, uint8_t>, 8> m_chunkSequences;

	void addBoxToSeq(HotVertexSeq<Vec3f, Vec3f, float, uint8_t>& hotSeq, uint& vertexIndex, uint& indexIndex, BlockType blockType, Vec3i blockWorldPos);
	int countVisibleFaces(Vec3i blockWorldPos);
	bool isFaceVisible(Vec3i blockWorldPos, BlockSide side);
	Vec4f getLightForFace(Vec3i blockWorldPos, BlockSide side);
};