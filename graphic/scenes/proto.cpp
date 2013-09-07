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

#ifdef _WIN32
void Scene::PadInput(LPDIDEVICEOBJECTDATA od) {
	DIDEVICEOBJECTDATA tmpObjDat;
	tmpObjDat.dwData = od->dwData;
	tmpObjDat.dwOfs = od->dwOfs;
	tmpObjDat.dwSequence = od->dwSequence;
	tmpObjDat.dwTimeStamp = od->dwTimeStamp;
	tmpObjDat.uAppData = od->uAppData;
	signed short axisVal = tmpObjDat.dwData;
	switch (od->dwOfs) {
	case DIJOFS_X:
		if (axisVal < 0)
			tmpObjDat.dwOfs = DIK_LEFT;
		else if (axisVal > 0)
			tmpObjDat.dwOfs = DIK_RIGHT;
		KeyboardInput(&tmpObjDat);
		break;
	case DIJOFS_Y:
		if (axisVal < 0)
			tmpObjDat.dwOfs = DIK_UP;
		else if (axisVal > 0)
			tmpObjDat.dwOfs = DIK_DOWN;
		KeyboardInput(&tmpObjDat);
		break;
	case DIJOFS_BUTTON0:  case DIJOFS_BUTTON2:  case DIJOFS_BUTTON4:  case DIJOFS_BUTTON6:  case DIJOFS_BUTTON8:
	case DIJOFS_BUTTON10: case DIJOFS_BUTTON12: case DIJOFS_BUTTON14: case DIJOFS_BUTTON16: case DIJOFS_BUTTON18:
	case DIJOFS_BUTTON20: case DIJOFS_BUTTON22: case DIJOFS_BUTTON24: case DIJOFS_BUTTON26: case DIJOFS_BUTTON28:
	case DIJOFS_BUTTON30:
		tmpObjDat.dwOfs = DIK_Z;
		KeyboardInput(&tmpObjDat);
		break;
	case DIJOFS_BUTTON1:  case DIJOFS_BUTTON3:  case DIJOFS_BUTTON5:  case DIJOFS_BUTTON7:  case DIJOFS_BUTTON9:
	case DIJOFS_BUTTON11: case DIJOFS_BUTTON13: case DIJOFS_BUTTON15: case DIJOFS_BUTTON17: case DIJOFS_BUTTON19:
	case DIJOFS_BUTTON21: case DIJOFS_BUTTON23: case DIJOFS_BUTTON25: case DIJOFS_BUTTON27: case DIJOFS_BUTTON29:
	case DIJOFS_BUTTON31:
		tmpObjDat.dwOfs = DIK_X;
		KeyboardInput(&tmpObjDat);
		break;
	}
}
#endif /* _WIN32 */

}
