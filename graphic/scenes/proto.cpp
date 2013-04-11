#include "proto.h"

namespace mihajong_graphic {

int Scene::whichRegion(int X, int Y) {
	int regionNum = -1;
	for (int i = 0; i < regions.size(); i++)
		if ((X >= regions[i].Left) &&(Y >= regions[i].Top) && (X <= regions[i].Right) && (Y <= regions[i].Bottom))
			regionNum = i;
	return regionNum;
}

const Scene::Region Scene::NullRegion = {0, 0, -1, -1};

void Scene::setRegion(unsigned regionID, int Left, int Top, int Right, int Bottom) {
	if (regions.size() <= regionID)
		regions.resize(regionID + 1, NullRegion);
	regions[regionID].Left = Left;
	regions[regionID].Top = Top;
	regions[regionID].Right = Right;
	regions[regionID].Bottom = Bottom;
}
void Scene::setRegion(unsigned regionID, const Region& region) {
	setRegion(regionID, region.Left, region.Top, region.Right, region.Bottom);
}

}
