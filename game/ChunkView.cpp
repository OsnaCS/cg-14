#include "ChunkView.hpp"
#include "lumina/lumina.hpp"
#include "lumina/io/ImageJPEG.hpp"
Tex2D ChunkView::s_texture;

ChunkView::ChunkView() {

}

void ChunkView::init(Vec2i index, Map& map) {
  m_index = index;
  m_map = &map;

  updateView();
}

void ChunkView::updateView() {

  if(s_texture.nativeHandle()==0){
    ImageBox image_box = loadJPEGImage("gfx/textures_craftgame_2nd_version_better.jpg");
    s_texture.create(Vec2i(2048,2048), TexFormat::RGB8, image_box.data());
    s_texture.params.filterMode = TexFilterMode::Trilinear;
    s_texture.params.useMipMaps = true;
  }
  auto chunkOffset = m_index * 16;

  for(int i = 0; i < 8; i++) {

  	uint blockCount = 0;

  	for(Vec3i pos : Vec3i(16, 16, 16)) {

  		pos.y += i * 16;
      auto block = m_map->getChunk(m_index).getBlockType(pos);

	    if(block != BlockType::Air) {
	      blockCount++;
	    }
	  }

    if(blockCount == 0) {
      continue;
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

        auto block = m_map->getChunk(m_index).getBlockType(pos);

        if(block != BlockType::Air) {

          Vec3f cubePos(chunkOffset.x + pos.x, pos.y, chunkOffset.y + pos.z);

          if (isBoxVisible(cubePos)) {
            addBoxToSeq(hotSeq, j, k, block, cubePos);

            // Indices erhöhen
            j += 24;
            k += 30;
          }
        }
      }
    });

    m_chunkSequences[i] = std::move(sequence);
  }
}

bool ChunkView::isBoxVisible(Vec3f& cubePos) {

  Vec3i top(cubePos.x, cubePos.y + 1, cubePos.z);
  Vec3i bottom(cubePos.x, cubePos.y - 1, cubePos.z);
  Vec3i left(cubePos.x - 1, cubePos.y, cubePos.z);
  Vec3i right(cubePos.x + 1, cubePos.y, cubePos.z);
  Vec3i front(cubePos.x, cubePos.y, cubePos.z + 1);
  Vec3i back(cubePos.x, cubePos.y, cubePos.z - 1);

  if ((m_map->exists(top) && m_map->getBlockType(top) != BlockType::Air) &&
      (m_map->exists(bottom) && m_map->getBlockType(bottom) != BlockType::Air) &&
      (m_map->exists(left) && m_map->getBlockType(left) != BlockType::Air) &&
      (m_map->exists(right) && m_map->getBlockType(right) != BlockType::Air) &&
      (m_map->exists(front) && m_map->getBlockType(front) != BlockType::Air) &&
      (m_map->exists(back) && m_map->getBlockType(back) != BlockType::Air)) {
    return false;
  } else {
    return true;
  }
}

void ChunkView::addBoxToSeq(HotVertexSeq<Vec3f, Vec3f, Vec2f>& hotSeq, uint& vertexIndex, uint& indexIndex, BlockType& block, Vec3f& cubePos) {
  Color8A color = getColor(block);
  Vec3f c(color.r / 255.f, color.g / 255.f, color.b / 255.f);

  Vec2f south = getTexCoords(block,BlockSide::South);
  Vec2f east = getTexCoords(block, BlockSide::East);
  Vec2f north = getTexCoords(block, BlockSide::North);
  Vec2f west = getTexCoords(block, BlockSide::West);
  Vec2f top = getTexCoords(block, BlockSide::Top);
  Vec2f bottom = getTexCoords(block, BlockSide::Bottom);

  float s = 0.5f;

  // -- positive z
  hotSeq.vertex[0 + vertexIndex].set(cubePos + Vec3f(-s, s, s), Vec3f(0,0,1), south);
  hotSeq.vertex[1 + vertexIndex].set(cubePos + Vec3f(-s, -s, s), Vec3f(0,0,1), south + Vec2f(0, 1/8.f));
  hotSeq.vertex[2 + vertexIndex].set(cubePos + Vec3f(s, s, s), Vec3f(0,0,1), south + Vec2f(1/8.f, 0));
  hotSeq.vertex[3 + vertexIndex].set(cubePos + Vec3f(s, -s, s), Vec3f(0,0,1), south + Vec2f(1/8.f, 1/8.f));

  hotSeq.index[0 + indexIndex] = 0 + vertexIndex;
  hotSeq.index[1 + indexIndex] = 1 + vertexIndex;
  hotSeq.index[2 + indexIndex] = 2 + vertexIndex;
  hotSeq.index[3 + indexIndex] = 3 + vertexIndex;
  hotSeq.index[4 + indexIndex] = GLIndex::PrimitiveRestart;

  // -- positive x
  hotSeq.vertex[4 + vertexIndex].set(cubePos + Vec3f(s, s, s), Vec3f(1,0,0),east);
  hotSeq.vertex[5 + vertexIndex].set(cubePos + Vec3f(s, -s, s), Vec3f(1,0,0), east+Vec2f(0, 1/8.f));
  hotSeq.vertex[6 + vertexIndex].set(cubePos + Vec3f(s, s, -s), Vec3f(1,0,0), east+Vec2f(1/8.f, 0));
  hotSeq.vertex[7 + vertexIndex].set(cubePos + Vec3f(s, -s, -s), Vec3f(1,0,0), east+Vec2f(1/8.f, 1/8.f));

  hotSeq.index[5 + indexIndex] = 4 + vertexIndex;
  hotSeq.index[6 + indexIndex] = 5 + vertexIndex;
  hotSeq.index[7 + indexIndex] = 6 + vertexIndex;
  hotSeq.index[8 + indexIndex] = 7 + vertexIndex;
  hotSeq.index[9 + indexIndex] = GLIndex::PrimitiveRestart;

  // -- negative z
  hotSeq.vertex[8 + vertexIndex].set(cubePos + Vec3f(s, s, -s), Vec3f(0,0,-1), north);
  hotSeq.vertex[9 + vertexIndex].set(cubePos + Vec3f(s, -s, -s), Vec3f(0,0,-1), north+Vec2f(0, 1/8.f));
  hotSeq.vertex[10 + vertexIndex].set(cubePos + Vec3f(-s, s, -s), Vec3f(0,0,-1), north+Vec2f(1/8.f, 0));
  hotSeq.vertex[11 + vertexIndex].set(cubePos + Vec3f(-s, -s, -s), Vec3f(0,0,-1), north+Vec2f(1/8.f, 1/8.f));

  hotSeq.index[10 + indexIndex] = 8 + vertexIndex;
  hotSeq.index[11 + indexIndex] = 9 + vertexIndex;
  hotSeq.index[12 + indexIndex] = 10 + vertexIndex;
  hotSeq.index[13 + indexIndex] = 11 + vertexIndex;
  hotSeq.index[14 + indexIndex] = GLIndex::PrimitiveRestart;

  // -- negative x
  hotSeq.vertex[12 + vertexIndex].set(cubePos + Vec3f(-s, s, -s), Vec3f(-1,0,0), west);
  hotSeq.vertex[13 + vertexIndex].set(cubePos + Vec3f(-s, -s, -s), Vec3f(-1,0,0), west+Vec2f(0, 1/8.f));
  hotSeq.vertex[14 + vertexIndex].set(cubePos + Vec3f(-s, s, s), Vec3f(-1,0,0), west+Vec2f(1/8.f, 0));
  hotSeq.vertex[15 + vertexIndex].set(cubePos + Vec3f(-s, -s, s), Vec3f(-1,0,0), west+Vec2f(1/8.f, 1/8.f));

  hotSeq.index[15 + indexIndex] = 12 + vertexIndex;
  hotSeq.index[16 + indexIndex] = 13 + vertexIndex;
  hotSeq.index[17 + indexIndex] = 14 + vertexIndex;
  hotSeq.index[18 + indexIndex] = 15 + vertexIndex;
  hotSeq.index[19 + indexIndex] = GLIndex::PrimitiveRestart;

  // -- positive y
  hotSeq.vertex[16 + vertexIndex].set(cubePos + Vec3f(-s, s, -s), Vec3f(0,1,0), top);
  hotSeq.vertex[17 + vertexIndex].set(cubePos + Vec3f(-s, s, s), Vec3f(0,1,0), top+Vec2f(0, 1/8.f));
  hotSeq.vertex[18 + vertexIndex].set(cubePos + Vec3f(s, s, -s), Vec3f(0,1,0), top+Vec2f(1/8.f, 0));
  hotSeq.vertex[19 + vertexIndex].set(cubePos + Vec3f(s, s, s), Vec3f(0,1,0), top+Vec2f(1/8.f, 1/8.f));

  hotSeq.index[20 + indexIndex] = 16 + vertexIndex;
  hotSeq.index[21 + indexIndex] = 17 + vertexIndex;
  hotSeq.index[22 + indexIndex] = 18 + vertexIndex;
  hotSeq.index[23 + indexIndex] = 19 + vertexIndex;
  hotSeq.index[24 + indexIndex] = GLIndex::PrimitiveRestart;

  // -- negative y
  hotSeq.vertex[20 + vertexIndex].set(cubePos + Vec3f(s, -s, s), Vec3f(0,-1,0), bottom);
  hotSeq.vertex[21 + vertexIndex].set(cubePos + Vec3f(-s, -s, s), Vec3f(0,-1,0), bottom+Vec2f(0, 1/8.f));
  hotSeq.vertex[22 + vertexIndex].set(cubePos + Vec3f(s, -s, -s), Vec3f(0,-1,0), bottom+Vec2f(1/8.f, 0));
  hotSeq.vertex[23 + vertexIndex].set(cubePos + Vec3f(-s, -s, -s), Vec3f(0,-1,0), bottom+Vec2f(1/8.f, 1/8.f));

  hotSeq.index[25 + indexIndex] = 20 + vertexIndex;
  hotSeq.index[26 + indexIndex] = 21 + vertexIndex;
  hotSeq.index[27 + indexIndex] = 22 + vertexIndex;
  hotSeq.index[28 + indexIndex] = 23 + vertexIndex;
  hotSeq.index[29 + indexIndex] = GLIndex::PrimitiveRestart;
}

void ChunkView::draw(HotProgram& hotProg) {
  s_texture.prime(0, [&](HotTex2D& hotTex) {
    for(VertexSeq& sequence : m_chunkSequences) {
      if(sequence) {
        hotProg.draw(hotTex, sequence, PrimitiveType::TriangleStrip);
      }
   	}
 });
}

