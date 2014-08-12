#pragma once

#include "Chunk.hpp"
#include "lumina/lumina.hpp"

class ChunkView{

public:

	ChunkView(Chunk& chunk, Vec2i index);
	void draw(HotProgram& hotProg);

private:

	static VertexSeq m_box;
	Chunk& m_chunk;
	Vec2i m_index;



};