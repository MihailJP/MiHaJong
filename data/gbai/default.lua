-- MiHaJong中国麻雀 デフォルトAI

function isflower (tile) -- 花牌かどうか判定
	if tile then -- nilだったらエラーになるのでそれを防止
		for k, v in getiter(mihajong.Tile.Flower) do
			if tile.tile == v then return true end
		end
	end
	return false
end

function ontsumo (gametbl) -- ＡＩの打牌
	local haiHand = gametbl:gethand()

	-- 花牌を抜く
	for cnt = 1, 14 do
		if isflower(haiHand[cnt]) then return mihajong.DiscardType.Flower, cnt end 
	end

	return mihajong.DiscardType.Normal, 14
end

function ondiscard (gametbl) -- 鳴き
	return mihajong.Call.None -- 仮実装
end

function onkakan (gametbl)
	return mihajong.Call.None -- 仮実装
end

function onankan (gametbl)
	return mihajong.Call.None -- 仮実装
end
