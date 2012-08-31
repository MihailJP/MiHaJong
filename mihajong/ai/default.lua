-- MiHaJong 1.7.0以降用 デフォルトAI
-- 先読みＡＩなのでたまに時間がかかる場合があります

function isflower(tilenum) -- 花牌かどうか判定
	if     tilenum == mihajong.Tile.Flower.Spring then return true
	elseif tilenum == mihajong.Tile.Flower.Summer then return true
	elseif tilenum == mihajong.Tile.Flower.Autumn then return true
	elseif tilenum == mihajong.Tile.Flower.Winter then return true
	elseif tilenum == mihajong.Tile.Flower.Plum   then return true
	elseif tilenum == mihajong.Tile.Flower.Orchid then return true
	elseif tilenum == mihajong.Tile.Flower.Chrys  then return true
	elseif tilenum == mihajong.Tile.Flower.Bamboo then return true
	else                                               return false
	end
end

function ontsumo (gametbl) -- AIの打牌処理
	local TileCount = gametbl:gettilesinhand() -- 牌を数える
	local MyHand = gametbl:getHand() -- 自分の手牌を取得
	local DiscardTileType = mihajong.DiscardType.Normal
	local DiscardTileIndex = 14 -- 捨牌の番号
	if not MyHand[14] then DiscardTileIndex = 1 end -- 鳴いた直後（多牌しないための処理）
	
	-- 花牌を抜く
	if MyHand[14] then -- 鳴きの直後には花牌を晒せない
		for k = 1, 14 do
			if MyHand[k] then -- attempt to index (a nil value) 防止用
				if isflower(MyHand[k].tile) then -- 花牌が見つかったら
					return mihajong.DiscardType.Flower, k -- 花牌を抜く
				end
			end
		end
	end
	
	-- リーチ後の暗槓判定
	if gametbl:isriichideclared() then -- リーチしていたら
		if gametbl:isankanallowed() then -- 暗槓がルール違反にならないなら
			return mihajong.DiscardType.Ankan, 14 -- 暗槓する
		end
	end
end
