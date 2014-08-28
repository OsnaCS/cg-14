#include "ChunkView.hpp"

#include <cstdint>

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
  //wenn es existiert und nicht luft oder birchleaves ist (denn letzteres ist partiell durchsichtig)
  return !(m_map->exists(checkPos) && (m_map->getBlockType(checkPos) != BlockType::SpruceLeaves && m_map->getBlockType(checkPos) != BlockType::Air 
    && m_map->getBlockType(checkPos) != BlockType::BirchLeaves));
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

    VertexSeq<Vec3f, Vec3f, float, uint8_t> sequence;
    sequence.create(4 * faceCount, 5 * faceCount);

    sequence.prime([&](
      HotVertexSeq<Vec3f, Vec3f, float, uint8_t>& hotSeq) {

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

/**
 * @brief returns the light for the vertices of the face (x: top left, y: top right, z: bottom right, w: bottom left)
 */
Vec4f ChunkView::getLightForFace(Vec3i blockWorldPos, BlockSide side) {

  // prepare lookup
  std::array<bool, 8> lookup;

  // get check positions depending on the BlockSide
  std::array<Vec3i, 8> checkPositions;
  switch (side) {
    case BlockSide::Top:
      checkPositions = {
        blockWorldPos + Vec3i(-1, 1, -1),
        blockWorldPos + Vec3i(0, 1, -1),
        blockWorldPos + Vec3i(1, 1, -1),
        blockWorldPos + Vec3i(1, 1, 0),
        blockWorldPos + Vec3i(1, 1, 1),
        blockWorldPos + Vec3i(0, 1, 1),
        blockWorldPos + Vec3i(-1, 1, 1),
        blockWorldPos + Vec3i(-1, 1, 0)
      };
      break;
    case BlockSide::Bottom:
      checkPositions = {
        blockWorldPos + Vec3i(-1, -1, 1),
        blockWorldPos + Vec3i(0, -1, 1),
        blockWorldPos + Vec3i(1, -1, 1),
        blockWorldPos + Vec3i(1, -1, 0),
        blockWorldPos + Vec3i(1, -1, -1),
        blockWorldPos + Vec3i(0, -1, -1),
        blockWorldPos + Vec3i(-1, -1, -1),
        blockWorldPos + Vec3i(-1, -1, 0)
      };
      break;

    // back side
    case BlockSide::North:
      checkPositions = {
        blockWorldPos + Vec3i(1, 1, -1),
        blockWorldPos + Vec3i(0, 1, -1),
        blockWorldPos + Vec3i(-1, 1, -1),
        blockWorldPos + Vec3i(-1, 0, -1),
        blockWorldPos + Vec3i(-1, -1, -1),
        blockWorldPos + Vec3i(0, -1, -1),
        blockWorldPos + Vec3i(1, -1, -1),
        blockWorldPos + Vec3i(1, 0, -1)
      };
      break;

    // front side
    case BlockSide::South:
      checkPositions = {
        blockWorldPos + Vec3i(-1, 1, 1),
        blockWorldPos + Vec3i(0, 1, 1),
        blockWorldPos + Vec3i(1, 1, 1),
        blockWorldPos + Vec3i(1, 0, 1),
        blockWorldPos + Vec3i(1, -1, 1),
        blockWorldPos + Vec3i(0, -1, 1),
        blockWorldPos + Vec3i(-1, -1, 1),
        blockWorldPos + Vec3i(-1, 0, 1)
      };
      break;
    case BlockSide::West:
      checkPositions = {
        blockWorldPos + Vec3i(-1, 1, -1),
        blockWorldPos + Vec3i(-1, 1, 0),
        blockWorldPos + Vec3i(-1, 1, 1),
        blockWorldPos + Vec3i(-1, 0, 1),
        blockWorldPos + Vec3i(-1, -1, 1),
        blockWorldPos + Vec3i(-1, -1, 0),
        blockWorldPos + Vec3i(-1, -1, -1),
        blockWorldPos + Vec3i(-1, 0, -1)
      };
      break;
    case BlockSide::East:
      checkPositions = {
        blockWorldPos + Vec3i(1, 1, 1),
        blockWorldPos + Vec3i(1, 1, 0),
        blockWorldPos + Vec3i(1, 1, -1),
        blockWorldPos + Vec3i(1, 0, -1),
        blockWorldPos + Vec3i(1, -1, -1),
        blockWorldPos + Vec3i(1, -1, 0),
        blockWorldPos + Vec3i(1, -1, 1),
        blockWorldPos + Vec3i(1, 0, 1)
      };
      break;
  }

  // fill bit lookup
  int i = 0;
  for (Vec3i pos : checkPositions) {
    lookup[i] = m_map->exists(pos) && m_map->getBlockType(pos) != BlockType::Air;
    i++;
  }

  // loop over the edges of the quad (+= 2 to use the index in the bitset)
  std::array<float, 4> colors;
  for (int i = 0; i < colors.size(); i++) {

    int a = i == 0 ? (lookup.size() - 1) : ((i * 2) - 1);
    int b = i * 2;
    int c = (i * 2) + 1;

    // darkest
    if (lookup[a] && lookup[c]) {
      colors[i] = 0.3f;
    // half dark
    } else if ((!lookup[a] && lookup[b] && lookup[c]) || (lookup[a] && lookup[b] && !lookup[c])) {
      colors[i] = 0.6f;
    // half light
    } else if ((lookup[a] && !lookup[b] && !lookup[c]) || (!lookup[a] && !lookup[b] && lookup[c]) || (!lookup[a] && lookup[b] && !lookup[c])) {
      colors[i] = 0.8f;
    } else {
    // light
      colors[i] = 1.0f;
    }
  }

  return Vec4f(colors[0], colors[1], colors[2], colors[3]);
}

void ChunkView::addBoxToSeq(HotVertexSeq<Vec3f, Vec3f, float, uint8_t>& hotSeq, uint& vertexIndex, uint& indexIndex, BlockType blockType, Vec3i blockWorldPos) {

  // texture coords
  int south = getTexCoords(blockType,BlockSide::South);
  int east = getTexCoords(blockType, BlockSide::East);
  int north = getTexCoords(blockType, BlockSide::North);
  int west = getTexCoords(blockType, BlockSide::West);
  int top = getTexCoords(blockType, BlockSide::Top);
  int bottom = getTexCoords(blockType, BlockSide::Bottom);

  // block size
  float s = 0.5f;

  // index offsets
  int vertexIndexOffset = 0;
  int indexIndexOffset = 0;

  // -- positive z (front)
  if (isFaceVisible(blockWorldPos, BlockSide::South)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::South);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(0,0,south), light.x, 3);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(0,1,south), light.w, 3);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(1,0,south), light.y, 3);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(1,1,south), light.z, 3);

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- positive x (right)
  if (isFaceVisible(blockWorldPos, BlockSide::East)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::East);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(0,0,east), light.x, 5);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(0,1,east), light.w, 5);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(1,0,east), light.y, 5);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(1,1,east), light.z, 5);

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative z (back)
  if (isFaceVisible(blockWorldPos, BlockSide::North)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::North);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(0,0,north), light.x, 2);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(0,1,north), light.w, 2);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(1,0,north), light.y, 2);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(1,1,north), light.z, 2);

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative x (left)
  if (isFaceVisible(blockWorldPos, BlockSide::West)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::West);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(0,0,west), light.x, 4);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(0,1,west), light.w, 4);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(1,0,west), light.y, 4);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(1,1,west), light.z, 4);

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- positive y (top)
  if (isFaceVisible(blockWorldPos, BlockSide::Top)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::Top);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, -s), Vec3f(0,0,top), light.x, 0);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, s, s), Vec3f(0,1,top), light.w, 0);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, s, -s), Vec3f(1,0,top), light.y, 0);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(s, s, s), Vec3f(1,1,top), light.z, 0);

    hotSeq.index[indexIndexOffset + 0 + indexIndex] = vertexIndexOffset + 0 + vertexIndex;
    hotSeq.index[indexIndexOffset + 1 + indexIndex] = vertexIndexOffset + 1 + vertexIndex;
    hotSeq.index[indexIndexOffset + 2 + indexIndex] = vertexIndexOffset + 2 + vertexIndex;
    hotSeq.index[indexIndexOffset + 3 + indexIndex] = vertexIndexOffset + 3 + vertexIndex;
    hotSeq.index[indexIndexOffset + 4 + indexIndex] = GLIndex::PrimitiveRestart;

    vertexIndexOffset += 4;
    indexIndexOffset += 5;
  }

  // -- negative y (bottom)
  if (isFaceVisible(blockWorldPos, BlockSide::Bottom)) {
    Vec4f light = getLightForFace(blockWorldPos, BlockSide::Bottom);
    hotSeq.vertex[vertexIndexOffset + 0 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, s), Vec3f(0,0,bottom), light.y, 1);
    hotSeq.vertex[vertexIndexOffset + 1 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, s), Vec3f(0,1,bottom), light.x, 1);
    hotSeq.vertex[vertexIndexOffset + 2 + vertexIndex].set(blockWorldPos + Vec3f(s, -s, -s), Vec3f(1,0,bottom), light.z, 1);
    hotSeq.vertex[vertexIndexOffset + 3 + vertexIndex].set(blockWorldPos + Vec3f(-s, -s, -s), Vec3f(1,1,bottom), light.w, 1);

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
  for(VertexSeq<Vec3f, Vec3f, float, uint8_t>& sequence : m_chunkSequences) {
    if(sequence) {
      hotProg.draw(hotCont, sequence, PrimitiveType::TriangleStrip);
    }
  }
}
