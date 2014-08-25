#include "ChunkView.hpp"

ChunkView::ChunkView() {

}

void ChunkView::init(Vec2i index, Map& map) {
  m_index = index;
  m_map = &map;

  updateView();
}

/**
 * @brief Checks if the given BlockSide of a given block is visible
 */
bool ChunkView::isFaceVisible(Vec3i blockWorldPos, BlockSide blockSide) {

  Vec3i positionOffset;
  switch (blockSide) {

    case BlockSide::Top:
      positionOffset = Vec3i(0, 1, 0);
      break;
    case BlockSide::Bottom:
      positionOffset = Vec3i(0, -1, 0);
      break;

    // back side
    case BlockSide::North:
      positionOffset = Vec3i(0, 0, -1);
      break;

    // front side
    case BlockSide::South:
      positionOffset = Vec3i(0, 0, 1);
      break;
    case BlockSide::West:
      positionOffset = Vec3i(-1, 0, 0);
      break;
    case BlockSide::East:
      positionOffset = Vec3i(1, 0, 0);
      break;
  }

  Vec3i checkPos = blockWorldPos + positionOffset;
  return !(m_map->exists(checkPos) && m_map->getBlockType(checkPos) != BlockType::Air);
}

/**
 * @brief Counts the visible faces of the given block
 */
int ChunkView::countVisibleFaces(Vec3i blockWorldPos) {
  
  // increase faceCount for every visible side of the block
  int faceCount = 0;
  for (int i = 0; i < 6; i++) {
    if(isFaceVisible(blockWorldPos, static_cast<BlockSide>(i))) {
      faceCount++;
    }
  }

  return faceCount;
}

void ChunkView::updateView() {

  Vec2i chunkOffset = m_index * 16;

  // for each subchunk
  for(int i = 0; i < 8; i++) {

    // count faces to draw
  	int faceCount = 0;

    // for each block in the current subchunk
  	for(Vec3i blockPos : Vec3i(16, 16, 16)) {

      // correct the y-pos of the block
  		blockPos.y += i * 16;

      // get the pos of the block in world-coords and the blocktype
      Vec3i blockWorldPos(chunkOffset.x + blockPos.x, blockPos.y, chunkOffset.y + blockPos.z);
      BlockType blockType = m_map->getBlockType(blockWorldPos);

      // if the block is not air count the visible faces
	    if(blockType != BlockType::Air) {
        faceCount += countVisibleFaces(blockWorldPos);
	    }
	  }

    // skip blocks with no visible faces
    if(faceCount == 0) {
      continue;
    }

    VertexSeq sequence;
    sequence.create(3 + 3 + 3 + 2, 4 * faceCount, 5 * faceCount);

    sequence.prime<Vec3f, Vec3f, Vec3f, Vec2f>([&](
      HotVertexSeq<Vec3f, Vec3f, Vec3f, Vec2f>& hotSeq) {

    	// vertex and index indices
    	uint j = 0;
    	uint k = 0;

      // for each block in the current subchunk
      for(Vec3i blockPos : Vec3i(16, 16, 16)) {

        // correct the y-pos of the block
        blockPos.y += i * 16;

        // get the pos of the block in world-coords and the blocktype
        Vec3i blockWorldPos(chunkOffset.x + blockPos.x, blockPos.y, chunkOffset.y + blockPos.z);
        BlockType blockType = m_map->getBlockType(blockWorldPos);

        // if the block is not air add the visible faces to the sequence
        if(blockType != BlockType::Air) {
          addBoxToSeq(hotSeq, j, k, blockType, blockWorldPos);
        }
      }
    });

    m_chunkSequences[i] = std::move(sequence);
  }
}

void ChunkView::addBoxToSeq(HotVertexSeq<Vec3f, Vec3f, Vec3f, Vec2f>& hotSeq, uint& vertexIndex, uint& indexIndex, BlockType blockType, Vec3i blockWorldPos) {

  // texture coords
  Vec2f south = getTexCoords(blockType,BlockSide::South);
  Vec2f east = getTexCoords(blockType, BlockSide::East);
  Vec2f north = getTexCoords(blockType, BlockSide::North);
  Vec2f west = getTexCoords(blockType, BlockSide::West);
  Vec2f top = getTexCoords(blockType, BlockSide::Top);
  Vec2f bottom = getTexCoords(blockType, BlockSide::Bottom);

  // block size
  float s = 0.5f;

  // index offsets
  int vertexIndexOffset = 0;
  int indexIndexOffset = 0;

  // -- positive z
  if (isFaceVisible(blockWorldPos, BlockSide::South)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(0,0,1),Vec3f(1,0,0), south);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(0,0,1), Vec3f(1,0,0), south + Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(0,0,1), Vec3f(1,0,0), south + Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(0,0,1), Vec3f(1,0,0), south + Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- positive x
  if (isFaceVisible(blockWorldPos, BlockSide::East)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(1,0,0), Vec3f (0,0,1), east);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(1,0,0), Vec3f (0,0,1), east+Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(1,0,0), Vec3f (0,0,1), east+Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(1,0,0), Vec3f (0,0,1), east+Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative z
  if (isFaceVisible(blockWorldPos, BlockSide::North)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(0,0,-1), Vec3f(1,0,0), north);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(0,0,-1), Vec3f(1,0,0), north+Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(0,0,-1), Vec3f(1,0,0), north+Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(0,0,-1), Vec3f(1,0,0), north+Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative x
  if (isFaceVisible(blockWorldPos, BlockSide::West)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(-1,0,0), Vec3f(0,0,1), west);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(-1,0,0), Vec3f(0,0,1), west+Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(-1,0,0), Vec3f(0,0,1), west+Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(-1,0,0), Vec3f(0,0,1), west+Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- positive y
  if (isFaceVisible(blockWorldPos, BlockSide::Top)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(0,1,0), Vec3f(1,0,0), top);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(0,1,0), Vec3f(1,0,0), top+Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(0,1,0), Vec3f(1,0,0), top+Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(0,1,0), Vec3f(1,0,0), top+Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative y
  if (isFaceVisible(blockWorldPos, BlockSide::Bottom)) {
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(0,-1,0), Vec3f(0,0,1), bottom);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(0,-1,0), Vec3f(0,0,1), bottom+Vec2f(0, 1/8.f));
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(0,-1,0), Vec3f(0,0,1), bottom+Vec2f(1/8.f, 0));
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(0,-1,0), Vec3f(0,0,1), bottom+Vec2f(1/8.f, 1/8.f));

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  vertexIndex += vertexIndexOffset;
  indexIndex += indexIndexOffset;
}


void ChunkView::draw(HotProgram& hotProg, HotTexCont& hotCont) {
  for(VertexSeq& sequence : m_chunkSequences) {
    if(sequence) {
      hotProg.draw(hotCont, sequence, PrimitiveType::TriangleStrip);
    }
  }
}
