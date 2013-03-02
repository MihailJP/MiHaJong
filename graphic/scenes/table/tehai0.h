#pragma once

#include <functional>
#include <tuple>
#include <d3dx9.h>
#include <dxerr.h>
#include "../../showtile.h"
#include "../../../common/seatrank.h"

namespace mihajong_graphic {

using mihajong_structs::seatRelative;

class ShowTehai {
protected:
	ShowTile* TileTexture;
	static const unsigned int HandLength = 13;
	static const int tileCursorOff = -3; // ��v�J�[�\��������
	void Reconstruct(const GameTable* gameStat, PlayerID targetPlayer,
		std::function<std::tuple<int, int> (seatRelative)> coordFunc,
		seatRelative direction = (seatRelative)-1,
		std::function<D3DCOLOR (int)> colorFunc = [](int){return (D3DCOLOR)0xffffffff;},
		std::function<void (const int*, const int*, int)> regionFunc = [](const int*, const int*, int){}); // ��v�̍č\�z
private:
	LPDIRECT3DDEVICE9 myDevice;
public:
	virtual void Render() = 0;
	explicit ShowTehai(LPDIRECT3DDEVICE9 device);
	virtual ~ShowTehai();
};

}
