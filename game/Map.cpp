#include "Map.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

Map::Map()
{

	m_name = "untitled";

}

Map::Map(string name)
{

	m_name = name;

}

void Map::setName(string name)
{

	m_name = name;

}

void Map::addChunk(Vec2i pos)
{

	Chunk c = Chunk();

  if (m_map.count(pos)>0)
    cout << " is already an element of m_map.\n";

	m_map[pos] = c;

}

void Map::setBlockType(Vec3i pos, BlockType type)
{

	Vec2i pos2d = getChunkPos(pos);

	pos.x = pos.x % 16;
	if(pos.x < 0)
		pos.x = pos.x + 16;

	pos.z = pos.z % 16; 
	if(pos.z < 0)
		pos.z = pos.z + 16;

	m_map.at(pos2d).setBlockType(pos, type);

}

Vec2i Map::getChunkPos(Vec3i pos)
{

	Vec2i pos2d;
	pos2d.x = pos.x / 16;
	pos2d.y = pos.z / 16;

	//C++ rounds negative number to 0, but we need it to be rounded down
	if(pos.x < 0 && (pos.x % 16 != 0))
		pos2d.x = pos2d.x - 1;
	if(pos.z < 0 && (pos.z % 16 != 0))
		pos2d.y = pos2d.y - 1;

	return pos2d;

}

BlockType Map::getBlockType(Vec3i pos)
{

	Vec2i pos2d = getChunkPos(pos);

	pos.x = pos.x % 16;
	if(pos.x < 0)
		pos.x = pos.x + 16;

	pos.z = pos.z % 16; 
	if(pos.z < 0)
		pos.z = pos.z + 16;

	return m_map.at(pos2d).getBlockType(pos);

}

bool Map::exists(Vec3i pos)
{

	return m_map.count(getChunkPos(pos)) > 0;

}

void Map::saveWorld()
{

	std::ofstream mapfile;
	string mapname =  "save/" + m_name + "/mapfile.txt";
	mapfile.open(mapname.c_str());

	// Für jeden Vector in der Map eine Datei erstellen
	// zusätzlich die Map abspeichern
	for(map<Vec2i, Chunk>::iterator it=m_map.begin(); it!=m_map.end(); it++)
	{

		stringstream s;// string Stream erzeugen
   	s << it->first.x << "_" << it->first.y;// Key hinzufügen
   	
		// Variablen Dateinamen erzeugen
		// vorsicht, Pfad angabe, d.h. account abhängig
		string newFolder = "save/" + m_name + "/";
   	string a =  newFolder + s.str() + ".txt";
   	string mapcontent = s.str();

   	mkdir(newFolder.c_str(), 0777);

   	cout << a << endl;
   	it++;
   	if(it==m_map.end())
   		{mapfile << mapcontent;}
   	else
   	{
   		mapfile << mapcontent << endl;
   	}
		it--;

		std::ofstream outfile;
  	outfile.open(a.c_str());

  	int j = 0;

  	for(int i = 0; i < m_map[it->first].m_blocks.size(); i++)
  	{

  		outfile << static_cast<int>(m_map[it->first].m_blocks[i]);
  		j++;

  		if(j == 256)
  		{
  			outfile << std::endl;
  			j = 0;
  		}

  	}
  	
  	outfile.close();
  
	}

	mapfile.close();

}

map<Vec2i, Chunk> Map::loadWorld(string chunkname)
{
	BlockType bl = static_cast<BlockType>(3);
	cout << static_cast<int>(getColor(bl).r) << endl;
	// jede Datei im Ordner muss aufgerufen und eingelesen werden

	ifstream f;  // Datei-Handle
  string s;
  f.open("save/" + m_name + "/mapfile.txt"); // Öffne Datei aus Parameter

  while (!f.eof())          // Solange noch Daten vorliegen
  {

    getline(f, s);        // Lese eine Zeile
    cout << s << endl;    // Zeige sie auf dem Bildschirm
    ifstream e;
    string s2;
    e.open("save/" + m_name + "/" + s + ".txt");
  	while(!e.eof())
  	{
  		getline(e, s2);
  	//	cout << s2 << endl;
  	}

  	e.close();

  }
  f.close();                // Datei wieder schließen

  return m_map;

}