#pragma once

#include "Chunk.hpp"
#include "lumina/lumina.hpp"

#include <array>

/**
 * @brief Graphical representation of Chunk
 */
class ChunkView {
public:
	ChunkView(Chunk& chunk, Vec2i index);
	void draw(HotProgram& hotProg);
	void updateView();

private:
	Chunk& m_chunk;
	Vec2i m_index;
	std::array<VertexSeq, 8> m_chunkSequences;
};