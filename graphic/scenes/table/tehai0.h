#pragma once

#include <functional>
#include <tuple>
#include "../../directx.h"
#include "../../showtile.h"
#include "../../../common/seatrank.h"

namespace mihajong_graphic {

using mihajong_structs::seatRelative;

class ShowTehai {
protected:
	ShowTile* TileTexture;
	static const unsigned int HandLength = 13;
	static const int tileCursorOff = -3; // è”vƒJ[ƒ\ƒ‹–³Œø
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
		std::function<std::tuple<int, int> (seatRelative)> coordFunc,
		seatRelative direction = (seatRelative)-1,
		std::function<ArgbColor (int)> colorFunc = [](int){return (ArgbColor)0xffffffff;},
		std::function<void (const int*, const int*, int)> regionFunc = [](const int*, const int*, int){}); // è”v‚ÌÄ\’z
private:
	DevicePtr myDevice;
public:
	virtual void Render() = 0;
	explicit ShowTehai(DevicePtr device);
	virtual ~ShowTehai();
};

}
