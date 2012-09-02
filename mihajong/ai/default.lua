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
	
	-- 和了れるなら和了る
	local SeenTiles = gametbl:getseentiles() -- 見えている牌を数える
	local Shanten = gametbl:getshanten() -- 向聴
	if Shanten == -1 then -- 和了になっているなら
		local result = gametbl:evaluate(true) -- この手を評価
		if result.isvalid then --和了れるなら
			return mihajong.DiscardType.Agari, nil -- 和了る
--[[
		else -- 和了れないなら
			return mihajong.DiscardType.Normal, 14 -- ツモ切り
--]]
		end
	end
	
	-- 十三不塔なら和了る
	if gametbl:isfirstdraw() and (gametbl:isshisanbuda() or gametbl:isshisibuda()) then
		return mihajong.DiscardType.Agari, nil
	end
	
	-- 九種九牌になっているか
	if gametbl:isfirstdraw() and gametbl:iskyuushu() then
		if Shanten > 2 then return mihajong.DiscardType.Kyuushu, nil
		elseif Shanten == 2 and gametbl:getscore() < gametbl:getbasepoint() then
			return mihajong.DiscardType.Kyuushu, nil
		end
	end
	
	-- リーチの場合は、和了れなければツモ切り
	if gametbl:isriichideclared() then -- リーチしていたら
		return mihajong.DiscardType.Normal, 14
	end
	
	-- リーチをかけるかどうか
	do
		local do_not_discard = {} -- 移植時追加、捨ててはいけない牌(食い変えの牌、プンリーの待ち牌)の識別用
		local Discardability = {} -- どれを捨てるかの指標を格納する
		local MinScore = {}
		local MaxScore = {}
		local MachihaiTotalTiles = {}
		local MachihaiFuritenFlag = {}
		if Shanten == 0 and (gametbl:ismenzen() or (gametbl:getrule("riichi_shibari") == "yes")) then
			-- リーチできる条件を満たしている場合
			local Richiability = 0
			local tmpHaiHand = gametbl:gethand()
			local t1, t2 = gametbl:getpreviousdiscard()
			for k = 1, 14 do
				if not tmpHaiHand[k] then
					do_not_discard[k] = true -- 存在しない牌の場合
				elseif (k > 1) and tmpHaiHand[k - 1].tile == tmpHaiHand[k] then
					-- 同じ牌を２度調べない
					Discardability[k] = Discardability[k - 1]
					do_not_discard[k] = do_not_discard[k - 1]
				elseif (tmpHaiHand[k].tile == t1) or (tmpHaiHand[k].tile == t2) then
					do_not_discard[k] = true -- 喰い変えになる牌は飛ばす
				else
					-- 向聴数から、大まかな評価値を算出
					tmpHypothetHand = tmpHaiHand
					tmpHypothetHand[k], tmpHypothetHand[14] = tmpHypothetHand[14], nil
					local tenpaistat = gametbl:gettenpaistat(tmpHypothetHand)
					-- chkfuriten FuritenFlag, MachihaiFlag, MachihaiCount, MachihaiTotal, MachiMen, GameStat, getCurrentPlayer(GameStat, CURRENTPLAYER_ACTIVE)
					local Shanten = gametbl:getshanten(tmpHypothetHand)
					MachihaiTotalTiles[k] = tenpaistat.total
					-- ダブル立直になる時は一部の判定を省略する
					local cont = true
					if not gametbl:isfirstdraw() then
						if gametbl:getopenwait()[tmpHaiHand[k].tile] then
							-- 他家のプンリーの当たり牌になっている場合は捨てない(捨ててはならない！)
							do_not_discard[k], cont = true, false
						elseif tenpaistat.total == 0 then
							-- 空聴リーチを避ける(錯和ではないが、和了れなくなるため)
							Discardability[k], cont = -999999, false
						elseif tenpaistat.isfuriten then
							-- 振聴リーチを避ける(錯和ではないが、手変わりの可能性を残す)
							MachihaiFuritenFlag[k], Discardability[k], cont = true, -99999, false
						elseif tenpaistat.total <= 2 then
							-- 待ち牌が残り２枚以下の場合
							Discardability[k], cont = -9999, false
						end
					end
					if Shanten > 0 then -- 不聴立直防止用
						Discardability[k], cont = -999999, false
					end
					Richiability = Richiability + 1
				end
			end
		end
	end
end
