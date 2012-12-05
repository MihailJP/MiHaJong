#include "proto.h"

namespace mihajong_graphic {

int Scene::whichRegion(int X, int Y) {
	int regionNum = -1;
	for (int i = 0; i < regions.size(); i++)
		if ((X >= regions[i].Left) &&(Y >= regions[i].Top) && (X <= regions[i].Right) && (Y <= regions[i].Bottom))
			regionNum = i;
	return regionNum;
}

}
