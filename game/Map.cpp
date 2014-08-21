#include "Map.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

Map::Map() { m_name = "untitled"; }

Map::Map(string name) { m_name = name; }

void Map::setName(string name) { m_name = name; }

void Map::addChunk(Vec2i pos) {

  Chunk c;

  if(m_map.count(pos) > 0)
    slogWarning("[Map] Overwriting old Chunk with new empty Chunk");

  m_map[pos] = c;
}

Chunk& Map::getChunk(Vec2i pos) {

  if(m_map.count(pos) == 0)
    throw OutOfRangeEx("[Map] The Map contains no Chunk at this Position");

  return m_map[pos];
}

Vec2i Map::getChunkPos(Vec3i pos) {

  Vec2i pos2d;
  pos2d.x = pos.x / 16;
  pos2d.y = pos.z / 16;

  // C++ rounds negative number to 0, but we need it to be rounded down
  if(pos.x < 0 && (pos.x % 16 != 0))
    pos2d.x = pos2d.x - 1;
  if(pos.z < 0 && (pos.z % 16 != 0))
    pos2d.y = pos2d.y - 1;

  return pos2d;
}


void Map::setBlockType(Vec3i pos, BlockType type) {

  Vec2i pos2d = getChunkPos(pos);

  pos.x = pos.x % 16;
  if(pos.x < 0)
    pos.x = pos.x + 16;

  pos.z = pos.z % 16;
  if(pos.z < 0)
    pos.z = pos.z + 16;

  m_map.at(pos2d).setBlockType(pos, type);
}

BlockType Map::getBlockType(Vec3i pos) {

  Vec2i pos2d = getChunkPos(pos);

  pos.x = pos.x % 16;
  if(pos.x < 0)
    pos.x = pos.x + 16;

  pos.z = pos.z % 16;
  if(pos.z < 0)
    pos.z = pos.z + 16;

  return m_map.at(pos2d).getBlockType(pos);
}

bool Map::exists(Vec3i pos) {

  if (pos.y < 0 || pos.y > 127) {
    return false;
  }

  return m_map.count(getChunkPos(pos)) > 0;
}

void Map::saveWorld() {

  string saveFolder = "save/";
  string newFolder = "save/" + m_name + "/";
  mkdir(saveFolder.c_str(), 0777);
  mkdir(newFolder.c_str(), 0777);
  std::ofstream mapfile;
  string mapname = "save/" + m_name + "/mapfile.txt";
  mapfile.open(mapname.c_str());

  // Für jeden Vector in der Map eine Datei erstellen
  // zusätzlich die Map abspeichern
  for(map<Vec2i, Chunk>::iterator it = m_map.begin(); it != m_map.end(); it++) {

    stringstream s;                         // string Stream erzeugen
    s << it->first.x << "_" << it->first.y; // Key hinzufügen

    // Variablen Dateinamen erzeugen

    string a = newFolder + s.str() + ".txt";
    string mapcontent = s.str();

    it++;
    if(it == m_map.end()) {
      mapfile << mapcontent;
    } else {
      mapfile << mapcontent << endl;
    }
    it--;

    std::ofstream outfile;
    outfile.open(a.c_str());

    int j = 0;

    for(int i = 0; i < m_map[it->first].m_blocks.size(); i++) {

      outfile << static_cast<int>(m_map[it->first].m_blocks[i]) << " ";
      j++;

      if(j == 256 && (i != 16 * 16 * 128 - 1)) {

        outfile << endl;
        j = 0;
      }
    }

    outfile.close();
  }

  mapfile.close();
}

void Map::loadWorld(string name) {

  string mappath = "save/" + name + "/";

  // Für den Fall, dass der MapOrdner nicht existiert, wird eine Exception geworfen
  if(!mkdir(mappath.c_str(), 0777)) {

    remove(mappath.c_str());
    throw OutOfRangeEx("[Map] Map does not exist");
  }

  map<Vec2i, Chunk> maptoload;
  ifstream f; // Datei-Handle
  string s;
  f.open("save/" + name + "/mapfile.txt"); // Öffne Datei aus Parameter

  // Öffne das MapFile und gehe alle Einträge durch
  while(!f.eof()) {

    size_t pos;
    string subs1, subs2;

    getline(f, s); // Lese eine Zeile

    if(s.length() == 0)
      break;
    ifstream e;
    string s2;
    e.open("save/" + name + "/" + s + ".txt");

    pos = s.find("_");
    subs1 = s.substr(0, pos);
    subs2 = s.substr(pos + 1);

    Chunk c = Chunk();
    int i = 0;
    while(!e.eof()) {

      getline(e, s2);

      while(s2.length() > 0) {

        pos = s2.find(" ");
        string blt = s2.substr(0, pos);
        c.m_blocks[i] = static_cast<BlockType>(atoi(blt.c_str()));
        s2 = s2.substr(pos + 1);
        i++;
      }

      maptoload[Vec2i(atoi(subs1.c_str()), atoi(subs2.c_str()))] = c;
    }

    e.close();
  }

  // Datei wieder schließen
  f.close();

  m_map = maptoload;
}

