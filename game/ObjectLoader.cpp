#include "ObjectLoader.hpp"
#include "lumina/lumina.hpp"

#include <fstream>


using namespace std;

VertexSeq loadOBJ(string obj_name){
  // Liste der verschiedenen Vertices/Normalen/Texturkoordinaten
  vector<Vec3f> vertices;
  vector<Vec3f> normals;
  vector<Vec2f> texture;
  vector<Vec3f> tangents;
  // Liste der Indizes der verschiedenen Vertices/Normalen/Texturkoordinaten
  vector<int> i_vertices;
  vector<int> i_normals;
  vector<int> i_texture;


  ifstream file(obj_name);

  if(!file) {
    slog("No Object-file!");
  }

  string line;
  int index_count = 0;

  while(getline(file, line)) {

    stringstream ss(line);
    string begin;
    ss >> begin; // liest bis zu einem Leerzeichen
    Vec3f tmp;
    Vec2f tmp2;

    if(begin == "v") {
      ss >> tmp.x;
      ss >> tmp.y;
      ss >> tmp.z;
      vertices.push_back(tmp);
    }

    if(begin == "vt") {
      ss >> tmp2.x;
      ss >> tmp2.y;
      texture.push_back(tmp2);
    }

    if(begin == "vn") {
      ss >> tmp.x;
      ss >> tmp.y;
      ss >> tmp.z;
      normals.push_back(tmp);
    }

    if(begin == "f") {
      index_count += 3;
      int index;
      char c;

      for(int three = 0; three < 3; three++) {
        // Positionsindex speichern
        ss >> index;
        i_vertices.push_back(index);
        // '/' überspringen
        ss >> c;
        // Texturindex speichern
        ss >> index;
        i_texture.push_back(index);
        //'/' überspringen
        ss >> c;
        // Normalenindex speichern
        ss >> index;
        i_normals.push_back(index);
      }
    }
  }

  for(int i = 0; i < index_count; i = i + 3) {
    tangents.push_back(vertices[i_vertices[i + 1]] - vertices[i_vertices[i]]);
    tangents.push_back(vertices[i_vertices[i + 1]] - vertices[i_vertices[i]]);
    tangents.push_back(vertices[i_vertices[i + 1]] - vertices[i_vertices[i]]);
  }

  VertexSeq object;
  object.create(3 + 3 + 2, index_count, index_count);
//object.create(3 + 3 + 3 + 2, index_count, index_count);
  // Position, Normale, Tangente, Textur 
  object.prime<Vec3f, Vec3f, Vec2f>([&](HotVertexSeq<Vec3f, Vec3f, Vec2f>& hotObject) {
    for(int j = 0; j < index_count; j++) {
      hotObject.vertex[j].set(vertices[i_vertices[j] - 1], normals[i_normals[j] - 1], texture[i_texture[j] - 1]);
      hotObject.index[j] = j;

    }
  });

  return object;
}