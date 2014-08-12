#pragma once

#include "Chunk.hpp"
#include "lumina/lumina.hpp"

/**
 * @brief Graphical representation of Chunk
 * 
 */
class ChunkView {
public:
	ChunkView(Chunk& chunk, Vec2i index);
	void draw(HotProgram& hotProg);

private:
	Chunk& m_chunk;
	Vec2i m_index;
	
	static VertexSeq m_box;
};