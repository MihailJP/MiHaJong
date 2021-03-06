﻿#pragma once

#include <functional>
#include <tuple>
#include "../../showtile.h"
#include "../../../common/seatrank.h"
#include "../../directx.h"

namespace mihajong_graphic {

using mihajong_structs::SeatRelative;

class ShowTehai {
protected:
	ShowTile* TileTexture;
	static constexpr unsigned int HandLength = 13;
	static constexpr int tileCursorOff = -3; // 手牌カーソル無効時
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
		std::function<std::tuple<int, int> (SeatRelative)> coordFunc,
		SeatRelative direction = (SeatRelative)-1,
		std::function<ArgbColor (int)> colorFunc = [](int){return static_cast<ArgbColor>(0xffffffff);},
		std::function<void (const int*, const int*, int)> regionFunc = [](const int*, const int*, int){}); // 手牌の再構築
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
