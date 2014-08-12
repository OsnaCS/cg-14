#include "Diamond.hpp"

using namespace lumina;

VertexSeq createDiamond(float length, float height) {
    VertexSeq diamond;
    diamond.create(6, 5, 18);

    diamond.prime<Vec3f, Color32f>([=](HotVertexSeq<Vec3f, Color32f>& hot){
	    hot.vertex[0].set(Vec3f(-length, 0.f, -length), Color32f(0, 1, 0));
	    hot.vertex[1].set(Vec3f(length, 0.f, -length),  Color32f(0, 0, 1));
	    hot.vertex[2].set(Vec3f(length,  0.f, length),  Color32f(0, 1, 0));
	    hot.vertex[3].set(Vec3f(-length, 0.f, length),  Color32f(0, 0, 1));
			hot.vertex[4].set(Vec3f(0.f, height, 0.f),  Color32f(1, 0, 0));
			hot.vertex[5].set(Vec3f(0.f, -height, 0.f),  Color32f(1, 1, 1));


			// top-back
			hot.index[0] = 4;
			hot.index[1] = 1;
			hot.index[2] = 0;

			// top-left
			hot.index[3] = 4;
			hot.index[4] = 2;
			hot.index[5] = 1;

			// top-front
			hot.index[6] = 4;
			hot.index[7] = 3;
			hot.index[8] = 2;

			// top-right
			hot.index[9] = 4;
			hot.index[10] = 0;
			hot.index[11] = 3;

			// bot-back
			hot.index[12] = 5;
			hot.index[13] = 0;
			hot.index[14] = 1;

			// bot-left
			hot.index[15] = 5;
			hot.index[16] = 1;
			hot.index[17] = 2;

			// bot-front
			hot.index[18] = 5;
			hot.index[19] = 2;
			hot.index[20] = 3;

			// bot-right
			hot.index[21] = 5;
			hot.index[22] = 3;
			hot.index[23] = 0;

  	});

  	return diamond;
};