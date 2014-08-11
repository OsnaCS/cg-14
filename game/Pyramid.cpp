#include "Pyramid.hpp"

using namespace lumina;

VertexSeq createPyramid(float length, float height) {
    VertexSeq pyramid;
    pyramid.create(6, 5);

    pyramid.prime<Vec3f, Color32f>([=](HotVertexSeq<Vec3f, Color32f>& hot){
	    hot.vertex[0].set(Vec3f(-length, 0.f, -length), Color32f(0, 1, 0));
	    hot.vertex[1].set(Vec3f(length, 0.f, -length),  Color32f(0, 0, 1));
	    hot.vertex[2].set(Vec3f(length,  0.f, length),  Color32f(0, 1, 0));
	    hot.vertex[3].set(Vec3f(-length, 0.f, length),  Color32f(0, 0, 1));
			hot.vertex[4].set(Vec3f(0.f, height, 0.f),  Color32f(1, 0, 0));

			// base
			hot.index[0] = 0;
			hot.index[1] = 1;
			hot.index[2] = 2;
			hot.index[3] = 0;
			hot.index[4] = 2;
			hot.index[5] = 3;

			// back
			hot.index[6] = 4;
			hot.index[7] = 1;
			hot.index[8] = 0;

			// left
			hot.index[9] = 4;
			hot.index[10] = 2;
			hot.index[11] = 1;

			// front
			hot.index[12] = 4;
			hot.index[13] = 3;
			hot.index[14] = 2;

			// right
			hot.index[15] = 4;
			hot.index[16] = 0;
			hot.index[17] = 3;
  	});

  	return pyramid;
};