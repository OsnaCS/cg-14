#pragma once

#include "lumina/lumina.hpp"

#include <string>

using namespace lumina;
using namespace std;

VertexSeq<Vec3f, Vec3f, Vec2f> loadOBJ(string file);
