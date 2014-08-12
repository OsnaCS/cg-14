#include "ChunkView.hpp"

VertexSeq ChunkView::m_box;

ChunkView::ChunkView(Chunk& chunk, Vec2i index) 
	: m_chunk(chunk), m_index(index){

		if(m_box.nativeVertexHandle() == 0){

			m_box = createBox<VAttr::Position, VAttr::Normal>(Vec3f(.45f,.45f,.45f));

		}

}

void ChunkView::draw(HotProgram& hotProg){

	for(Vec3i pos : Vec3i(16,128,16)){

		auto block = m_chunk.getBlockType(pos);

		if(block != BlockType::Air){

			auto hPos = m_index * 16;
			auto zPos = Vec3i(pos.x + hPos.x, pos.y, pos.z + hPos.y);

			Color8A c = getColor(block);

			hotProg.uniform["u_world"] = translationMatrix(vector_cast<float>(zPos));
			hotProg.uniform["u_color"] = Vec3f(c.r/255.f,c.g/255.f,c.b/255.f);
      hotProg.draw(m_box, PrimitiveType::TriangleStrip);
		}
	}
}
