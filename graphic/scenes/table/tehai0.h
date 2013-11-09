#pragma once

#include <functional>
#include <tuple>
#include "../../showtile.h"
#include "../../../common/seatrank.h"
#include "../../directx.h"

namespace mihajong_graphic {

using mihajong_structs::seatRelative;

class ShowTehai {
protected:
	ShowTile* TileTexture;
	static const unsigned int HandLength = 13;
	static const int tileCursorOff = -3; // éËîvÉJÅ[É\Éãñ≥å¯éû
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
		std::function<std::tuple<int, int> (seatRelative)> coordFunc,
		seatRelative direction = (seatRelative)-1,
		std::function<ArgbColor (int)> colorFunc = [](int){return (ArgbColor)0xffffffff;},
		std::function<void (const int*, const int*, int)> regionFunc = [](const int*, const int*, int){}); // éËîvÇÃçƒç\íz
private:
	DevicePtr myDevice;
public:
	virtual void Render() = 0;
	explicit ShowTehai(DevicePtr device);
	ShowTehai(const ShowTehai&) = delete; // Delete unexpected copy constructor
	ShowTehai& operator= (const ShowTehai&) = delete; // Delete unexpected assign operator
	virtual ~ShowTehai();
};

}
