#include "ChunkView.hpp"

ChunkView::ChunkView(Chunk& chunk, Vec2i index)
  : m_chunk(chunk), m_index(index) {
  updateView();
}

void ChunkView::updateView() {

  auto chunkOffset = m_index * 16;

  for(int i = 0; i < 8; i++) {

  	uint blockCount = 0;

  	for(Vec3i pos : Vec3i(16, 16, 16)) {

  		pos.y += i * 16;
      auto block = m_chunk.getBlockType(pos);

	    if(block != BlockType::Air) {
	      blockCount++;
	    }
	  }

    VertexSeq sequence;
    sequence.create(3 + 3 + 2, 6 * 4 * blockCount, 6 * 5 * blockCount);

    sequence.prime<Vec3f, Vec3f, Vec2f>([&](
      HotVertexSeq<Vec3f, Vec3f, Vec2f>& hotSeq) {

    	// Indices
    	uint j = 0;
    	uint k = 0;

      for(Vec3i pos : Vec3i(16, 16, 16)) {

        pos.y += i * 16;

        auto block = m_chunk.getBlockType(pos);

        if(block != BlockType::Air) {

          Color8A color = getColor(block);
          Vec3f c(color.r / 255.f, color.g / 255.f, color.b / 255.f);

          Vec3f cubePos(chunkOffset.x + pos.x, pos.y, chunkOffset.y + pos.z);

          float s = 0.5f;

          // -- positive z
          hotSeq.vertex[0 + j].set(cubePos + Vec3f(-s, s, s), c, Vec2f(0, 0));
          hotSeq.vertex[1 + j].set(cubePos + Vec3f(-s, -s, s), c, Vec2f(0, 1));
          hotSeq.vertex[2 + j].set(cubePos + Vec3f(s, s, s), c, Vec2f(1, 0));
          hotSeq.vertex[3 + j].set(cubePos + Vec3f(s, -s, s), c, Vec2f(1, 1));

          hotSeq.index[0 + k] = 0 + j;
          hotSeq.index[1 + k] = 1 + j;
          hotSeq.index[2 + k] = 2 + j;
          hotSeq.index[3 + k] = 3 + j;
          hotSeq.index[4 + k] = GLIndex::PrimitiveRestart;

          // -- positive x
          hotSeq.vertex[4 + j].set(cubePos + Vec3f(s, s, s), c, Vec2f(0, 0));
          hotSeq.vertex[5 + j].set(cubePos + Vec3f(s, -s, s), c, Vec2f(0, 1));
          hotSeq.vertex[6 + j].set(cubePos + Vec3f(s, s, -s), c, Vec2f(1, 0));
          hotSeq.vertex[7 + j].set(cubePos + Vec3f(s, -s, -s), c, Vec2f(1, 1));

          hotSeq.index[5 + k] = 4 + j;
          hotSeq.index[6 + k] = 5 + j;
          hotSeq.index[7 + k] = 6 + j;
          hotSeq.index[8 + k] = 7 + j;
          hotSeq.index[9 + k] = GLIndex::PrimitiveRestart;

          // -- negative z
          hotSeq.vertex[8 + j].set(cubePos + Vec3f(s, s, -s), c, Vec2f(0, 0));
          hotSeq.vertex[9 + j].set(cubePos + Vec3f(s, -s, -s), c, Vec2f(0, 1));
          hotSeq.vertex[10 + j].set(cubePos + Vec3f(-s, s, -s), c, Vec2f(1, 0));
          hotSeq.vertex[11 + j].set(cubePos + Vec3f(-s, -s, -s), c, Vec2f(1, 1));

          hotSeq.index[10 + k] = 8 + j;
          hotSeq.index[11 + k] = 9 + j;
          hotSeq.index[12 + k] = 10 + j;
          hotSeq.index[13 + k] = 11 + j;
          hotSeq.index[14 + k] = GLIndex::PrimitiveRestart;

          // -- negative x
          hotSeq.vertex[12 + j].set(cubePos + Vec3f(-s, s, -s), c, Vec2f(0, 0));
          hotSeq.vertex[13 + j].set(cubePos + Vec3f(-s, -s, -s), c, Vec2f(0, 1));
          hotSeq.vertex[14 + j].set(cubePos + Vec3f(-s, s, s), c, Vec2f(1, 0));
          hotSeq.vertex[15 + j].set(cubePos + Vec3f(-s, -s, s), c, Vec2f(1, 1));

          hotSeq.index[15 + k] = 12 + j;
          hotSeq.index[16 + k] = 13 + j;
          hotSeq.index[17 + k] = 14 + j;
          hotSeq.index[18 + k] = 15 + j;
          hotSeq.index[19 + k] = GLIndex::PrimitiveRestart;

          // -- positive y
          hotSeq.vertex[16 + j].set(cubePos + Vec3f(-s, s, -s), c, Vec2f(0, 0));
          hotSeq.vertex[17 + j].set(cubePos + Vec3f(-s, s, s), c, Vec2f(0, 1));
          hotSeq.vertex[18 + j].set(cubePos + Vec3f(s, s, -s), c, Vec2f(1, 0));
          hotSeq.vertex[19 + j].set(cubePos + Vec3f(s, s, s), c, Vec2f(1, 1));

          hotSeq.index[20 + k] = 16 + j;
          hotSeq.index[21 + k] = 17 + j;
          hotSeq.index[22 + k] = 18 + j;
          hotSeq.index[23 + k] = 19 + j;
          hotSeq.index[24 + k] = GLIndex::PrimitiveRestart;

          // -- negative y
          hotSeq.vertex[20 + j].set(cubePos + Vec3f(s, -s, s), c, Vec2f(0, 0));
          hotSeq.vertex[21 + j].set(cubePos + Vec3f(-s, -s, s), c, Vec2f(0, 1));
          hotSeq.vertex[22 + j].set(cubePos + Vec3f(s, -s, -s), c, Vec2f(1, 0));
          hotSeq.vertex[23 + j].set(cubePos + Vec3f(-s, -s, -s), c, Vec2f(1, 1));

          hotSeq.index[25 + k] = 20 + j;
          hotSeq.index[26 + k] = 21 + j;
          hotSeq.index[27 + k] = 22 + j;
          hotSeq.index[28 + k] = 23 + j;
          hotSeq.index[29 + k] = GLIndex::PrimitiveRestart;

          // Indices erhöhen
		      j += 24;
		      k += 30;
        }
      }
    });

    m_chunkSequences[i] = std::move(sequence);
  }
}

void ChunkView::draw(HotProgram& hotProg) {
  for(VertexSeq& sequence : m_chunkSequences) {
    hotProg.draw(sequence, PrimitiveType::TriangleStrip);
 	}
}
