-- MiHaJong 1.7.0以降用 デフォルトAI
-- 先読みＡＩなのでたまに時間がかかる場合があります

function clone (t) -- deep-copy a table
	if type(t) ~= "table" then return t end
	local meta = getmetatable(t)
	local target = {}
	for k, v in pairs(t) do
		if type(v) == "table" then
			target[k] = clone(v)
		else
			target[k] = v
		end
	end
	setmetatable(target, meta)
	return target
end

validtiles = {
	mihajong.Tile.Character[1], mihajong.Tile.Character[5], mihajong.Tile.Character[6],
	mihajong.Tile.Character[4], mihajong.Tile.Character[5], mihajong.Tile.Character[6],
	mihajong.Tile.Character[7], mihajong.Tile.Character[8], mihajong.Tile.Character[9],
	mihajong.Tile.Circle[1], mihajong.Tile.Circle[5], mihajong.Tile.Circle[6],
	mihajong.Tile.Circle[4], mihajong.Tile.Circle[5], mihajong.Tile.Circle[6],
	mihajong.Tile.Circle[7], mihajong.Tile.Circle[8], mihajong.Tile.Circle[9],
	mihajong.Tile.Bamboo[1], mihajong.Tile.Bamboo[5], mihajong.Tile.Bamboo[6],
	mihajong.Tile.Bamboo[4], mihajong.Tile.Bamboo[5], mihajong.Tile.Bamboo[6],
	mihajong.Tile.Bamboo[7], mihajong.Tile.Bamboo[8], mihajong.Tile.Bamboo[9],
	mihajong.Tile.Wind.East, mihajong.Tile.Wind.South, mihajong.Tile.Wind.West, mihajong.Tile.Wind.North,
	mihajong.Tile.Dragon.White, mihajong.Tile.Dragon.Green, mihajong.Tile.Dragon.Red,
}

yaojiutiles = {
	mihajong.Tile.Character[1], mihajong.Tile.Character[9],
	mihajong.Tile.Circle[1], mihajong.Tile.Circle[9],
	mihajong.Tile.Bamboo[1], mihajong.Tile.Bamboo[9],
	mihajong.Tile.Wind.East, mihajong.Tile.Wind.South, mihajong.Tile.Wind.West, mihajong.Tile.Wind.North,
	mihajong.Tile.Dragon.White, mihajong.Tile.Dragon.Green, mihajong.Tile.Dragon.Red,
}

function isflower (tile) -- 花牌かどうか判定
	if tile then -- nilだったらエラーになるのでそれを防止
		for k, v in pairs(mihajong.Tile.Flower) do
			if tile.tile == v then return true end
		end
	end
	return false
end

function someone_hadakatanki (gametbl)
	for k = 1, 4 do
		if gametbl:getmeld(k) and (#(gametbl:getmeld(k)) == 4) then return true end
	end
	return false
end

function isukihai (tilect_cnt) -- 浮き牌だったらtrue
	return tilect_cnt.isexistent and not (
		tilect_cnt.formstriplet or tilect_cnt.formssequence or tilect_cnt.canformquad or
				tilect_cnt.formspair or tilect_cnt.formsryanmen or
				tilect_cnt.formskanchan or tilect_cnt.formspenchan
	)
end

function ischaracter (tile) -- 萬子かどうか判定する
	for k, v in ipairs(mihajong.Tile.Character) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function iscircle (tile) -- 筒子かどうか判定する
	for k, v in ipairs(mihajong.Tile.Circle) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isbamboo (tile) -- 索子かどうか判定する
	for k, v in ipairs(mihajong.Tile.Bamboo) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isyaojiu (tile) -- 么九牌かどうか
	for k, v in ipairs(yaojiutiles) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function iswind (tile) -- 風牌かどうか
	for k, v in pairs(mihajong.Tile.Wind) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isdragon (tile) -- 三元牌かどうか
	for k, v in pairs(mihajong.Tile.Dragon) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function ishonor (tile) -- 字牌かどうか
	return iswind(tile) or isdragon(tile)
end

function issequence (meld) -- 順子かどうか
	for k, v in pairs(mihajong.MeldType.Sequence) do
		if meld.type == v then return true end
	end
	return false
end
function istriplet (meld) -- 刻子かどうか
	for k, v in pairs(mihajong.MeldType.Triplet) do
		if meld.type == v then return true end
	end
	return false
end

function gettilenumeral (tile) -- 牌の数字(１～９)を取得する
	for k, v in ipairs(mihajong.Tile.Character) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	for k, v in ipairs(mihajong.Tile.Circle) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	for k, v in ipairs(mihajong.Tile.Bamboo) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	return nil
end

local player_rel = { -- プレイヤー番号
	"shimocha", (gametbl.playerid + 0) % 4 + 1;
	"toimen",   (gametbl.playerid + 1) % 4 + 1;
	"kamicha",  (gametbl.playerid + 2) % 4 + 1;
}

function ontsumo (gametbl) -- ＡＩの打牌
	local haiHand = gametbl:gethand()
	local dahaiType, teDahai = mihajong.DiscardType.Normal, 14 -- ツモ切り
	if not (haiHand[14]) then teDahai = 1 -- 鳴いた直後（多牌しないための処理）
	else
		-- 花牌を抜く
		for cnt = 1, 14 do
			if isflower(haiHand[cnt]) then return mihajong.DiscardType.Flower, cnt end 
		end
		-- ここで時間稼ぎをしていた
		-- returnの位置はかつてここだった
	end
	if gametbl:isriichideclared() and gametbl:isankanallowed() then -- 立直後の暗槓
		-- ここで時間稼ぎをしていた
		return mihajong.DiscardType.Ankan, 14
	end
	if gametbl:getshanten() == -1 then
		local stat = gametbl:evaluate(true)
		-- ここで時間稼ぎをしていた
		if (not stat.isvalid) or ((gametbl:getrule("riichi_shibari") ~= "no") and (not gametbl:isriichideclared())) then
			-- 和了れないならツモ切り
			return mihajong.DiscardType.Normal, 14
		else
			-- 和了れるなら和了る
			return mihajong.DiscardType.Agari
		end
	end -- ツモ和了り
	-- 十三不塔なら和了る
	if gametbl:isshisanbuda() and gametbl:isfirstdraw() and (gametbl:getrule("shiisan_puutaa") ~= "no") then return mihajong.DiscardType.Agari end
	if gametbl:isshisibuda() and gametbl:isfirstdraw() and (gametbl:getrule("shiisan_uushii") ~= "no") then return mihajong.DiscardType.Agari end
	if gametbl:iskyuushu() and gametbl:isfirstdraw() then
		-- 九種九牌(浮きの2向聴、沈みの3向聴は流す)
		if (gametbl:isabovebase() and (gametbl:getshanten() > 1)) or (gametbl:getshanten() > 2) then
			-- 国士聴牌は流さない
			return mihajong.DiscardType.Kyuushu -- 流局
		end
	end
	-- リーチの場合は、和了れなければツモ切り
	if gametbl:isriichideclared() then return mihajong.DiscardType.Normal, 14 end

	do
		local flag, dahaiType, teDahai = riichi_decision(gametbl)
		if flag then return dahaiType, teDahai end
	end
	do
		local flag, dahaiType, teDahai = ankan_decision(gametbl)
		if flag then return dahaiType, teDahai end
	end
	do
		local dahaiType, teDahai = discard_decision(gametbl)
		return dahaiType, teDahai
	end
end

function riichi_decision (gametbl)
	local nowShanten = gametbl:getshanten()
	local haiCount, haiSeenCount = gametbl:gettilesinhand(), gametbl:getseentiles()
	-- リーチをかけるかどうか
	local ev = {
		"do_not_discard", {};
		"haiDiscardability", {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,};
		"MinScore", {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,};
		"MaxScore", {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,};
		"MachihaiTotalTiles", {}; "MachihaiFuritenFlag", {};
	}
	if (nowShanten == 0) and (gametbl:ismenzen() or (gametbl:getrule("riichi_shibari") ~= "no")) then
		local Richiability = 0
		local tmpHaiHand = gametbl:gethand()
		for cnt = 1, 14 do repeat -- workaround: continueの代わり
			if (cnt > 0) and tmpHaiHand[cnt-1] then
				if tmpHaiHand[cnt-1].tile == tmpHaiHand[cnt].tile then
					ev.do_not_discard[cnt] = ev.do_not_discard[cnt-1]
					ev.haiDiscardability[cnt] = ev.haiDiscardability[cnt-1] -- 同じ牌を２度調べない
					break
				end
			end
			local tilecontext = gametbl:gettilecontext()[cnt]
			local tmpHandTileCode = tmpHaiHand[cnt]
			-- 存在しない牌の場合
			if not gametbl:gethand()[cnt] then ev.do_not_discard[cnt], ev.haiDiscardability[cnt] = true, -9999999; break end
			-- 向聴数から、大まかな評価値を算出
			local haiHand = clone(tmpHaiHand)
			haiHand[cnt], haiHand[14] = clone(haiHand[14]), nil
			local tStat = gametbl:gettenpaistat(haiHand)
			local Shanten = gametbl:getshanten(haiHand)
			ev.MachihaiTotalTiles[cnt] = tStat.total
			-- ダブル立直になる時は一部の判定を省略する
			if not gametbl:isfirstdraw() then
				-- 他家のプンリーの当たり牌になっている場合は捨てない(捨ててはならない！)
				if gametbl:getopenwait()[tmpHandTileCode] then ev.do_not_discard[cnt], ev.haiDiscardability[cnt] = true, -999999999; break
				-- 空聴リーチを避ける(錯和ではないが、和了れなくなるため)
				elseif tStat.total == 0 then ev.haiDiscardability[cnt] = -999999; break
				-- 振聴リーチを避ける(錯和ではないが、手変わりの可能性を残す)
				elseif tStat.isfuriten == 1 then ev.MachihaiFuritenFlag[cnt], ev.haiDiscardability[cnt] = true, -99999; break
				-- 待ち牌が残り２枚以下の場合
				elseif tStat.total <= 2 then
					haiDiscardability[cnt] = -9999; break
				end
			end
			-- 不聴立直防止用
			if Shanten > 0 then haiDiscardability[cnt] = -999999; break end
			Richiability = Richiability + 1

			local tmpTileNum = cnt
			ev.MinScore[cnt], ev.MaxScore[cnt] = 999999999, -999999999
			for i, tilecode in ipairs(validtiles) do
				local haiHand = clone(tmpHaiHand)
				haiHand[cnt], haiHand[14] = clone(haiHand[14]), {"tile", tilecode; "red", 0}
				-- ここで時間チェックをしていた
				if (gametbl:getshanten(haiHand) == -1) then
					-- ここで計算されるのはダマ聴で自摸和のときの点数
					local stat = gametbl:evaluate(true, haiHand)
					if ev.haiDiscardability[tmpTileNum] >= 10000 then ev.haiDiscardability[tmpTileNum] = ev.haiDiscardability[tmpTileNum] - 10000 end
					ev.haiDiscardability[tmpTileNum] = ev.haiDiscardability[tmpTileNum] + stat.points*(4-haiCount[tilecode]-haiSeenCount[tilecode])+100000
					if ev.MinScore[tmpTileNum] > stat.han then ev.MinScore[tmpTileNum] = stat.han end
					if ev.MaxScore[tmpTileNum] < stat.han then ev.MaxScore[tmpTileNum] = stat.han end
				end
				if ev.MinScore[tmpTileNum] == 999999999 then ev.MinScore[tmpTileNum] = 0 end
				if ev.MaxScore[tmpTileNum] ==-999999999 then ev.MaxScore[tmpTileNum] = 0 end
			end
		until true end
		-- 時間がかかる時用の簡略版がここにあった

		local haiHand = gametbl:gethand()
		if Richiability > 0 then
			local dahaiType, teDahai = mihajong.DiscardType.Normal, 14
			for cnt = 14, 1, -1 do
				if (not ev.do_not_discard[cnt]) and (ev.haiDiscardability[teDahai] < ev.haiDiscardability[cnt]) then
					teDahai = cnt
				end
			end
			-- ここで長考してるふりをしていた
			--[[
				役満聴牌の場合、ダマ聴でも数え役満の場合、
				跳満以上が確定の場合は立直しない
				但しダブル立直になる時はリーチする
				待ちが８枚以上あるならプンリー
				振聴立直になるような場合は開き直ってプンリー
				誰かが裸単騎しているならプンリー
				但しリーチ縛りならリーチする
			]]
			if ((ev.MaxScore[teDahai] < 14) and (ev.MinScore[teDahai] < 7)) or gametbl:isfirstdraw() then
				if (ev.MachihaiTotalTiles[teDahai] >= 8) or ev.MachihaiFuritenFlag[teDahai] then
					if gametbl:ismenzen() then dahaiType = mihajong.DiscardType.OpenRiichi
					else dahaiType = mihajong.DiscardType.Riichi end
				elseif someone_hadakatanki(gametbl) then
					if (ev.MachihaiTotalTiles[teDahai] >= 2) then
						if gametbl:ismenzen() then dahaiType = mihajong.DiscardType.OpenRiichi
						else dahaiType = mihajong.DiscardType.Riichi end
					end
				else
					dahaiType = mihajong.DiscardType.Riichi
				end
			elseif gametbl:getrule("riichi_shibari") ~= "no" then
				dahaiType = mihajong.DiscardType.Riichi
			end
			if dahaiType ~= mihajong.DiscardType.Normal then
				return true, dahaiType, teDahai
			else return false end
		end
	end
	return false
end

function ankan_decision (gametbl)
	local nowShanten = gametbl:getshanten()
	local haiHand = gametbl:gethand()
	--[[
		チー、ポンした直後に暗槓/加槓できないようにするためのif処理
		チー、ポンした直後はツモ牌がない状態なので、それを利用する
		カンをした直後はツモ牌が嶺上牌になっている
		カンをしたあと、続けて暗槓をすることはかまわない
	]]
	if haiHand[14] then
		-- 暗槓するかどうか
		local tmpHaiHand = gametbl:gethand()
		local tilect = gametbl:gettilecontext()
		for cnt = 1, 14 do repeat
			if not haiHand[cnt] then break end -- continueの代わり
			local tmpnum = haiHand[cnt].tile
			for k = 1, 14 do
				if haiHand[cnt] and (haiHand[cnt].tile == tmpnum) then
					haiHand[cnt] = nil
				end
			end
			local Shanten = gametbl:getshanten(haiHand)
			haiHand = clone(tmpHaiHand)
			if tilect[cnt].canformquad then
				if (Shanten == (nowShanten+2)) then
					-- ここで長考してるふりをしていた
					return true, mihajong.DiscardType.Ankan, cnt
				end
			end
		until true end
		-- 加槓するかどうか
		for cnt = 1, 14 do repeat
			local Kakanability = false
			if not haiHand[cnt] then break end -- continueの代わり
			local tmpnum = haiHand[cnt].tile
			haiHand[cnt] = nil
			local Shanten = gametbl:getshanten(haiHand)
			haiHand = clone(tmpHaiHand)
			local haiNakiMianziDat = gametbl:getmeld()
			for k = 1, #haiNakiMianziDat do
				if haiNakiMianziDat[k].tile == tmpnum then
					if istriplet(haiNakiMianziDat[k]) then Kakanability = true end
				end
			end
			if Kakanability then
				if Shanten == nowShanten then
					-- ここで長考してるふりをしていた
					return true, mihajong.DiscardType.Kakan, cnt
				end
			end
		until true end
		return false
	end
end

function evaluate_hand (gametbl, cnt, tp, hand, haiDiscardability, tmpde) -- 再帰処理
	local origShanten = gametbl:getshanten()
	local tmpDiscardability = {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,}
	local nowShanten = gametbl:getshanten(hand)
	local haiHand = clone(hand)
	local tmpTileNum = cnt
	local haiCount, haiSeenCount = gametbl:gettilesinhand(), gametbl:getseentiles()
	for k, v in validtiles do repeat
		-- 時間がかかったらここで切り上げていた
		if origShanten > 1 then
			if ishonor({"tile", v}) and (haiCount[v] + haiSeenCount[v] == 3) then break end -- 字牌のラス牌はとりあえず無視
		end
		if origShanten > 0 then -- 本来は既にテンパイしているときには行なっていなかった処理
			if haiCount[v] + haiSeenCount[v] == 4 then break end -- ４枚切れた牌
			local tmpHe, haiSutehai = false, gametbl:getdiscard()
			for idx = 1, #haiSutehai do -- 振聴になる牌を避ける
				if haiSutehai[idx].tile == v then tmpHe = true; break end
			end
			if tmpHe then tmpDiscardability[tp] = 0; break end
		end
		haiHand[tp], haiHand[14] = clone(haiHand[14]), {"tile", v; "red", 0}
		local Shanten = gametbl:getshanten(haiHand)
		if Shanten == -1 then
			local stat = gametbl:evaluate(true, haiHand)
			if haiDiscardability[tmpTileNum] >= 10000 then
				tmpDiscardability[tp] = tmpDiscardability[tp] - 10000
			end
			tmpDiscardability[tp] = tmpDiscardability[tp] +
				stat.points * (4 - haiCount[v] - haiSeenCount[v]) + tmpde
		elseif Shanten == (nowShanten - 1) then
			local tmpde = (4 - haiCount[v] - haiSeenCount[v])
			-- 時間がかかったらここで切り上げていた
			for i = 1, 14 do repeat
				-- 時間がかかったらここで切り上げていた
				if i > 1 then
					if haiHand[i] and (haiHand[i].tile == haiHand[i - 1].tile) then
						tmpDiscardability[i] = tmpDiscardability[i - 1]
						break -- 同じ牌を２度調べない
					end
				end
				local td = evaluate_hand(gametbl, cnt, tp, haiHand, haiDiscardability, tmpde)
				for j = 1, 14 do tmpDiscardability[i] = tmpDiscardability[i] + td[j] end
			until true end
			-- 処理が遅い時はここで別処理をしていた
		end
		haiHand = clone(hand)
		-- 時間がかかったらここで切り上げていた
	until true end
	return tmpDiscardability
end

function discard_decision (gametbl)
	local do_not_discard = {}
	local haiDiscardability = {0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,}
	local haiHand = gametbl:gethand()
	local haiNakiMianziDat = gametbl:getmeld()
	local tilect = gametbl:gettilecontext()
	local haiSeenCount = gametbl:getseentiles()
	local nowShanten = gametbl:getshanten()

	-- それぞれを捨てたときの手の評価を計算する
	local WanzCount, PinzCount, SouzCount, YaojiuCount, KeziCount = 0, 0, 0, 0, 0
	for cnt = 1, 14 do
		if ischaracter(haiHand[cnt]) then WanzCount = WanzCount + 1 end
		if iscircle(haiHand[cnt]) then PinzCount = PinzCount + 1 end
		if isbamboo(haiHand[cnt]) then SouzCount = SouzCount + 1 end
		if isyaojiu(haiHand[cnt]) then YaojiuCount = YaojiuCount + 1 end
		if tilect[cnt].formstriplet or tilect[cnt].canformquad then KeziCount = KeziCount + 1 end
	end
	for cnt = 1, #haiNakiMianziDat do
		if ischaracter(haiNakiMianziDat[cnt]) then WanzCount = WanzCount + 3 end
		if iscircle(haiNakiMianziDat[cnt]) then PinzCount = PinzCount + 3 end
		if isbamboo(haiNakiMianziDat[cnt]) then SouzCount = SouzCount + 3 end
		if not issequence(haiNakiMianziDat[cnt]) then KeziCount = KeziCount + 3 end
	end
	--[=[ 1.3.1の時点ですでに没になってたらしい……
	if (WanzCount >= 8) then ephemeral.haiAimingYise = 1 end -- 一色手を狙ってみよう！
	if (PinzCount >= 8) then ephemeral.haiAimingYise = 2 end
	if (SouzCount >= 8) then ephemeral.haiAimingYise = 3 end
	if (KeziCount >= 6) then
		ephemeral.haiAimingDuidui = true -- どうやら対子場のようだ
	end
	--]=]

	for cnt = 1, 14 do repeat
		local tmpHand = clone(haiHand[cnt])
		local numeral = gettilenumeral(tmpHand)
		-- 存在しない牌の場合
		if not haiHand[cnt] then
			do_not_discard[cnt], haiDiscardability[cnt] = true, -9999999
			break
		end
		-- 向聴数から、大まかな評価値を算出
		haiHand[cnt] = nil
		local Shanten = gametbl:getshanten(haiHand)
		haiDiscardability[cnt] = (6-Shanten)*1000
		haiHand[cnt] = clone(tmpHand)
		if isukihai(tilect[cnt]) then
			if ishonor(tmpHand) then
				local yakuhailist = gametbl:getyakuhaiwind()
				haiDiscardability[cnt] = haiDiscardability[cnt] + 900 -- 字牌の浮き牌
				if isdragon(tmpHand) then
					haiDiscardability[cnt] = haiDiscardability[cnt] - 100 -- 三元牌のとき
				end
				for nom, tile in pairs(mihajong.Tile.Wind) do
					if (tmpHand.tile == tile) and yakuhailist[nom] then
						haiDiscardability[cnt] = haiDiscardability[cnt] - 100 -- 風牌が役牌のとき
					end
				end
			else
				haiDiscardability[cnt] = haiDiscardability[cnt] + 700 -- 数牌の浮き牌
			end
		else
			if     tilect[cnt].formspenchan then haiDiscardability[cnt] = haiDiscardability[cnt] + 500 -- 辺張
			elseif tilect[cnt].formskanchan then haiDiscardability[cnt] = haiDiscardability[cnt] + 300 -- 嵌張
			elseif ephemeral.haiAimingDuidui and tilect[cnt].formsryanmen then haiDiscardability[cnt] = haiDiscardability[cnt] + 100 -- 対子場の時は両面
			elseif tilect[cnt].formspair then haiDiscardability[cnt] = haiDiscardability[cnt] + 100 -- そうでなければ対子
			end
		end
		if haiHand[cnt].red ~= 0 then
			haiDiscardability[cnt] = haiDiscardability[cnt] - 50 -- 赤ドラの時
		end
		if tilect[cnt].formstriplet then
			-- 役牌の暗刻を切らないようにする
			if ishonor(tmpHand) then
				if isdragon(tmpHand) then
					haiDiscardability[cnt] = haiDiscardability[cnt] - 99999 -- 三元牌のとき
				end
				for nom, tile in pairs(mihajong.Tile.Wind) do
					if (tmpHand.tile == tile) and yakuhailist[nom] then
						haiDiscardability[cnt] = haiDiscardability[cnt] - 99999 -- 風牌が役牌のとき
					end
				end
			end
		end
		haiDiscardability[cnt] = haiDiscardability[cnt] + haiSeenCount[tmpHand.tile] * 10 -- 場に見えてる個数
		if ishonor(tmpHand) then haiDiscardability[cnt] = haiDiscardability[cnt] + 50
		elseif (numeral == 1) or (numeral == 9) then haiDiscardability[cnt] = haiDiscardability[cnt] + 40
		elseif (numeral == 2) or (numeral == 8) then haiDiscardability[cnt] = haiDiscardability[cnt] + 30
		elseif (numeral == 3) or (numeral == 7) then haiDiscardability[cnt] = haiDiscardability[cnt] + 20
		elseif (numeral == 4) or (numeral == 6) then haiDiscardability[cnt] = haiDiscardability[cnt] + 10
		end
		haiHand[cnt] = clone(tmpHand)
--[=[ いろいろあったんで、やめた
		if (nowShanten >= 2) then -- 染めてみようかな？？
			if iscircle(tmpHand) or isbamboo(tmpHand) then
				if WanzCount >= 11 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if WanzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if WanzCount >=  8 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
			if ischaracter(tmpHand) or isbamboo(tmpHand) then
				if PinzCount >= 11 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if PinzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if PinzCount >=  8 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
			if ischaracter(tmpHand) or iscircle(tmpHand) then
				if SouzCount >= 11 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if SouzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
				if SouzCount >=  8 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
		elseif (nowShanten == 1) then
			if iscircle(tmpHand) or isbamboo(tmpHand) then
				if WanzCount >= 12 then haiDiscardability[cnt] = haiDiscardability[cnt] + 20000 end
				if WanzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
			if ischaracter(tmpHand) or isbamboo(tmpHand) then
				if PinzCount >= 12 then haiDiscardability[cnt] = haiDiscardability[cnt] + 20000 end
				if PinzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
			if ischaracter(tmpHand) or iscircle(tmpHand) then
				if SouzCount >= 12 then haiDiscardability[cnt] = haiDiscardability[cnt] + 20000 end
				if SouzCount >= 10 then haiDiscardability[cnt] = haiDiscardability[cnt] + 10000 end
			end
		end
		if (nowShanten >= 1) then -- やっぱタンヤオ狙いでしょっ！
			if (YaojiuCount <= 3) and isyaojiu(tmpHand) then
				haiDiscardability[cnt] = haiDiscardability[cnt] + 30000
			end
		end
--]=]
		if nowShanten == 2 then -- 二向聴でも先読みさせてみる
			local tmpDiscardability = evaluate_hand(gametbl, cnt, cnt, haiHand, haiDiscardability, 0) -- 先読み判定
			for i = 1, 14 do haiDiscardability[i] = haiDiscardability[i] + tmpDiscardability[i] + 100000 end
		elseif nowShanten == 1 then -- 一向聴のとき
			local tmpDiscardability = evaluate_hand(gametbl, cnt, cnt, haiHand, haiDiscardability, 0) -- 先読み判定
			for i = 1, 14 do haiDiscardability[i] = haiDiscardability[i] + tmpDiscardability[i] + 100000 end
		elseif nowShanten == 0 then -- 聴牌したとき
			local tmpDiscardability = evaluate_hand(gametbl, cnt, cnt, haiHand, haiDiscardability, 100000) -- 先読み判定
			for i = 1, 14 do haiDiscardability[i] = haiDiscardability[i] + tmpDiscardability[i] end
		end
	until true end

	do -- オープン立直の待ち牌を捨てない(捨ててはならない！)ようにする
		haiHand[cnt] = clone(tmpHand)
		local haiOpenRichiMachihai = gametbl:getopenwait()
		for cnt = 1, 14 do
			if haiHand[cnt] and not isflower(haiHand[cnt]) then
				if haiOpenRichiMachihai[haiHand[cnt].tile] then
					do_not_discard[cnt], haiDiscardability[cnt] = true, -999999999
				end
			end
		end
	end

	if nowShanten > 1 then -- 大三元の包になるような牌を捨てないようにする
		-- ノーチャンスか・振聴となるか・白を鳴いてるか・発を鳴いてるか・中を鳴いてるか
		local tmpDiscardabilityChkFlag = 0
		for tmpchkcnt = 1, 4 do repeat
			if tmpchkcnt == gametbl.playerid then break end
			local haiNakiMianziDat = gametbl:getmeld(tmpchkcnt)
			for cnt = 1, #haiNakiMianziDat do
				if haiNakiMianziDat[cnt].tile == mihajong.Tile.Dragon.White then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 4
				elseif haiNakiMianziDat[cnt].tile == mihajong.Tile.Dragon.Green then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 2
				elseif haiNakiMianziDat[cnt].tile == mihajong.Tile.Dragon.Red then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 1
				end
			end
			local haiSutehai = gametbl:getdiscard(tmpchkcnt)
			for cnt = 1, #haiSutehai do
				if ((haiSutehai[cnt] == mihajong.Tile.Dragon.White) and (tmpDiscardabilityChkFlag == 3)) or
					((haiSutehai[cnt] == mihajong.Tile.Dragon.Green) and (tmpDiscardabilityChkFlag == 5)) or
					((haiSutehai[cnt] == mihajong.Tile.Dragon.Red)   and (tmpDiscardabilityChkFlag == 6)) then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 8; break
				end
			end
			if ((haiSeenCount[mihajong.Tile.Dragon.White] >= 2) and (tmpDiscardabilityChkFlag == 3)) or
				((haiSeenCount[mihajong.Tile.Dragon.Green] >= 2) and (tmpDiscardabilityChkFlag == 5)) or
				((haiSeenCount[mihajong.Tile.Dragon.Red  ] >= 2) and (tmpDiscardabilityChkFlag == 6)) then
				tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 16
			end
		until true end
		for cnt = 1, 14 do
			if haiHand[cnt] then
				if ((tmpDiscardabilityChkFlag == 3) and (haiHand[cnt].tile == mihajong.Tile.Dragon.White)) or
					((tmpDiscardabilityChkFlag == 5) and (haiHand[cnt].tile == mihajong.Tile.Dragon.Green)) or
					((tmpDiscardabilityChkFlag == 6) and (haiHand[cnt].tile == mihajong.Tile.Dragon.Red)) then
					haiDiscardability[cnt] = -799999999
				end
			end
		end
	end

	if nowShanten > 1 then -- 大四喜の包になるような牌を捨てないようにする
		-- ノーチャンスか・振聴となるか・東を鳴いてるか
		-- ・南を鳴いてるか・西を鳴いてるか・北を鳴いてるか
		local tmpDiscardabilityChkFlag = 0
		for tmpchkcnt = 1, 4 do repeat
			if tmpchkcnt == gametbl.playerid then break end
			local haiNakiMianziDat = gametbl:getmeld(tmpchkcnt)
			for cnt = 1, #haiNakiMianziDat do
				if haiNakiMianziDat[cnt].tile == mihajong.Tile.Wind.East then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 8
				elseif haiNakiMianziDat[cnt].tile == mihajong.Tile.Wind.South then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 4
				elseif haiNakiMianziDat[cnt].tile == mihajong.Tile.Wind.West then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 2
				elseif haiNakiMianziDat[cnt].tile == mihajong.Tile.Wind.North then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 1
				end
			end
			local haiSutehai = gametbl:getdiscard(tmpchkcnt)
			for cnt = 1, #haiSutehai do
				if ((haiSutehai[cnt] == mihajong.Tile.Wind.East ) and (tmpDiscardabilityChkFlag ==  7)) or
					((haiSutehai[cnt] == mihajong.Tile.Wind.South) and (tmpDiscardabilityChkFlag == 11)) or
					((haiSutehai[cnt] == mihajong.Tile.Wind.West ) and (tmpDiscardabilityChkFlag == 13)) or
					((haiSutehai[cnt] == mihajong.Tile.Wind.North) and (tmpDiscardabilityChkFlag == 14)) then
					tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 16; break
				end
			end
			if ((haiSeenCount[mihajong.Tile.Wind.East ] >= 3) and (tmpDiscardabilityChkFlag ==  7)) or
				((haiSeenCount[mihajong.Tile.Wind.South] >= 3) and (tmpDiscardabilityChkFlag == 11)) or
				((haiSeenCount[mihajong.Tile.Wind.West ] >= 3) and (tmpDiscardabilityChkFlag == 13)) or
				((haiSeenCount[mihajong.Tile.Wind.North] >= 3) and (tmpDiscardabilityChkFlag == 14)) then
				tmpDiscardabilityChkFlag = tmpDiscardabilityChkFlag + 32
			end
		until true end
		for cnt = 1, 14 do
			if haiHand[cnt] then
				if ((tmpDiscardabilityChkFlag == 7) and (haiHand[cnt].tile == mihajong.Tile.Wind.East)) or
					((tmpDiscardabilityChkFlag == 11) and (haiHand[cnt].tile == mihajong.Tile.Wind.South)) or
					((tmpDiscardabilityChkFlag == 13) and (haiHand[cnt].tile == mihajong.Tile.Wind.West)) or
					((tmpDiscardabilityChkFlag == 14) and (haiHand[cnt].tile == mihajong.Tile.Wind.North)) then
					haiDiscardability[cnt] = -799999999
				end
			end
		end
	end

	if nowShanten > 1 then -- 一色手を警戒する
		for tmpchkcnt = 1, 4 do repeat
			if tmpchkcnt == gametbl.playerid then break end
			local tmpDiscardabilityChkCount = {0, 0, 0}
			local haiSutehai = gametbl:getdiscard(tmpchkcnt)
			for cnt = 1, #haiSutehai do
				if ischaracter(haiSutehai[cnt]) then tmpDiscardabilityChkCount[1] = tmpDiscardabilityChkCount[1] + 1
				elseif iscircle(haiSutehai[cnt]) then tmpDiscardabilityChkCount[2] = tmpDiscardabilityChkCount[2] + 1
				elseif isbamboo(haiSutehai[cnt]) then tmpDiscardabilityChkCount[3] = tmpDiscardabilityChkCount[3] + 1
				end
			end
			for cnt = 1, 14 do
				if (tmpDiscardabilityChkCount[2] > 0) and (tmpDiscardabilityChkCount[3] > 0) then
					if (tmpDiscardabilityChkCount[1] / (tmpDiscardabilityChkCount[2] + tmpDiscardabilityChkCount[3])) < 0.2 then
						if haiHand[cnt] and ischaracter(haiHand[cnt]) then haiDiscardability[cnt] = 0 end
					end
				end
				if (tmpDiscardabilityChkCount[1] > 0) and (tmpDiscardabilityChkCount[3] > 0) then
					if (tmpDiscardabilityChkCount[2] / (tmpDiscardabilityChkCount[1] + tmpDiscardabilityChkCount[3])) < 0.2 then
						if haiHand[cnt] and iscircle(haiHand[cnt]) then haiDiscardability[cnt] = 0 end
					end
				end
				if (tmpDiscardabilityChkCount[1] > 0) and (tmpDiscardabilityChkCount[2] > 0) then
					if (tmpDiscardabilityChkCount[3] / (tmpDiscardabilityChkCount[1] + tmpDiscardabilityChkCount[2])) < 0.2 then
						if haiHand[cnt] and isbamboo(haiHand[cnt]) then haiDiscardability[cnt] = 0 end
					end
				end
			end
		until true end
	end

	if nowShanten > 1 then -- 国士無双を警戒する
		for tmpchkcnt = 1, 4 do repeat
			if tmpchkcnt == gametbl.playerid then break end
			local tmpDiscardabilityChkCount = {0, 0}
			local haiSutehai = gametbl:getdiscard(tmpchkcnt)
			for cnt = 1, #haiSutehai do
				if not isflower(haiSutehai[cnt]) then
					if not isyaojiu(haiSutehai[cnt]) then
						tmpDiscardabilityChkCount[1] = tmpDiscardabilityChkCount[1] + 1
					end
					tmpDiscardabilityChkCount[2] = tmpDiscardabilityChkCount[2] + 1
				end
			end
			for cnt = 1, 14 do
				if tmpDiscardabilityChkCount[2] > 0 then
					if (tmpDiscardabilityChkCount[1] / tmpDiscardabilityChkCount[2]) > 0.84 then
						if haiHand[cnt] and isyaojiu(haiHand[cnt]) then
							haiDiscardability[cnt] = 0
						end
					end
				end
			end
		until true end
	end

	-- 警戒するプレーヤーを決定する
	local haiMarkingPlayer = {false, false, false, false}
	for cnt = 1, 4 do repeat
		if tmpchkcnt == gametbl.playerid then break end -- 自分自身の場合は飛ばす
		local haiNakiMianziDat = gametbl:getmeld(cnt)
		if #haiNakiMianziDat >= 2 then
			haiMarkingPlayer[cnt] = true -- 二面子以上鳴いていたら降りる
		end
		if gametbl:isriichideclared(cnt) then
			haiMarkingPlayer[cnt] = true -- 誰かが立直していたら降りる！
		end
		if #haiNakiMianziDat == 1 and gametbl:getdeckleft() < 30 then -- 残り３０枚未満で１副露の者がいた場合は降りる
			haiMarkingPlayer[cnt] = true
		end
	until true end

	-- オリるかどうかの判定…和了り手の得点を調べる
	local TmpMinScore, TmpMaxScore, MinScore, MaxScore = 999999999, -999999999, {}, {}
	local haiCount, haiSeenCount = gametbl:gettilesinhand(), gametbl:getseentiles()
	for tmpTileNum = 1, 14 do repeat
		MinScore[cnt], MaxScore[cnt] = 0, 0
		if haiHand[tmpTileNum] then
			MinScore[cnt], MaxScore[cnt] = 0, 0; break
		end
		tmpTileCodeNum = clone(haiHand[tmpTileNum])
		MinScore[cnt], MaxScore[cnt] = 999999999, -999999999
		for k, cnt in ipairs(validtiles) do
			haiHand[tmpTileNum], haiHand[14] = clone(haiHand[14]), {"tile", cnt; "red", 0}
			if (gametbl:getshanten(haiHand) == -1) then
				-- ここで計算されるのはダマ聴で自摸和のときの点数
				local stat = gametbl:evaluate(true, haiHand)
				if haiDiscardability[tmpTileNum] >= 10000 then
					haiDiscardability[tmpTileNum] = haiDiscardability[tmpTileNum] - 10000
				end
				haiDiscardability[tmpTileNum] = haiDiscardability[tmpTileNum] +
					gametbl.points * (4 - haiCount[cnt] - haiSeenCount[cnt]) + 100000
				if MinScore[tmpTileNum] > stat.han then MinScore[tmpTileNum] = stat.han end
				if MaxScore[tmpTileNum] < stat.han then MaxScore[tmpTileNum] = stat.han end
			end
			haiHand[14], haiHand[tmpTileNum] = clone(haiHand[tmpTileNum]), clone(tmpTileCodeNum)
			if MinScore[tmpTileNum] == 999999999 then MinScore[tmpTileNum] = 0 end
			if MaxScore[tmpTileNum] ==-999999999 then MaxScore[tmpTileNum] = 0 end
		end
		haiHand[tmpTileNum] = clone(tmpTileCodeNum)
		if MinScore[tmpTileNum] < TmpMinScore then TmpMinScore = MinScore[tmpTileNum] end
		if MaxScore[tmpTileNum] > TmpMinScore then TmpMaxScore = MaxScore[tmpTileNum] end
	until true end
	if TmpMaxScore >= 6 then -- とりあえず、跳満以上聴牌なら全ツッパする
		haiMarkingPlayer = {false, false, false, false}
	end

	do
		local riskinfo = gametbl:gettilerisk()
		for tmphaiindex = 1, 14 do -- 降り打ちモードのための処理
			for plkey, tmpchkcnt in pairs(player_rel) do
				local done = false
				repeat
					if haiMarkingPlayer[tmpchkcnt] then break end
					if not riskinfo[tmphaiindex] then break end
					local tmpflag = false
					-- 合わせ打ちができるなら合わせ打ち
					if riskinfo[tmphaiindex].issameasprevious then
						haiDiscardability[tmphaiindex] = haiDiscardability[tmphaiindex] + 7000000
						tmpflag = true
					end
					if riskinfo[tmphaiindex][plkey].isgembutsu then
						-- 警戒しているプレーヤーの現物があれば優先的にそれを捨てる
						if not ishonor(haiHand[tmphaiindex]) then
							haiDiscardability[tmphaiindex] = haiDiscardability[tmphaiindex] + 10000000
						else
							haiDiscardability[tmphaiindex] = haiDiscardability[tmphaiindex] + 5000000
						end
						tmpflag = true
						done = true; break
					end
					if not tmpflag then -- 合わせ打ちも無理なら筋牌
						if riskinfo[tmphaiindex][plkey].issuji then
							haiDiscardability[tmphaiindex] = haiDiscardability[tmphaiindex] + 1200000
							done = true; break
						end
					end
				until true
				if done then break end
			end
		end
	end

	-- 捨牌を決定する
	local dahaiType, teDahai = mihajong.DiscardType.Normal, 14
	for cnt = 14, 1, -1 do
		if not do_not_discard[cnt] then
			if haiDiscardability[teDahai] < haiDiscardability[cnt] then
				teDahai = cnt
			end
		end
	end

	-- 決定された捨牌が北だった場合、捨てる代わりに抜く(三麻)
	if (gametbl:getrule("flower_tiles") == "four_norths") and haiHand[14] then
		if haiHand[teDahai].tile == mihajong.Tile.Wind.North then
			dahaiType = mihajong.DiscardType.Flower
		end
	end

	-- 長考してるふりをここでしていた
	return dahaiType, teDahai
end

function ondiscard (gametbl)
	return decide_call(gametbl, 0)
end
function onkakan (gametbl)
	return decide_call(gametbl, 1)
end
function onankan (gametbl)
	return decide_call(gametbl, 2)
end

function decide_call (gametbl, ChanKanFlag) -- ＡＩの鳴き・栄和
	local haiHand = gametbl:gethand()
	local haiCurrentSutehai = gametbl:getcurrentdiscard()

	if isflower(haiCurrentSutehai) then -- 花牌の場合は戻る
		return mihajong.Call.None
	end

	haiHand[14] = nil
	local currentShanten = gametbl:getshanten(haiHand)
	haiHand[14] = clone(haiCurrentSutehai)

	local Shanten = gametbl:getshanten(haiHand)
	local yakustat = gametbl:evaluate(false, haiHand)
	local tenpaistat = gametbl:gettenpaistat(haiHand)
	-- 暗槓に対する搶槓の判定で、国士聴牌でない場合は戻る
	if (ChanKanFlag == 2) and (gametbl:getshanten(haiHand, mihajong.AgariType.Orphans) ~= 0) then
		return mihajong.Call.None
	end

	if Shanten == -1 then
		local haiRon = true -- 出和了り
		if (not yakustat.isvalid) or -- 縛りを満たしていないか
				tenpaistat.isfuriten or -- 現物フリテンか
				gametbl:isdoujunfuriten() or -- 同巡内フリテンか
				((gametbl:getrule("riichi_shibari") ~= "no") and gametbl:isriichideclared()) then -- 立直縛りの場合でリーチしてないなら
					haiRon = false -- チョンボになるから和がらないようにする
		end
		if haiRon then
			return mihajong.Call.Ron
		else
			return mihajong.Call.None
		end
	end
	if ChanKanFlag == 1 then return mihajong.Call.None end -- 搶槓の判定中なら判定打ち切り
	if gametbl:getdeckleft() == 0 then return mihajong.Call.None end -- 河底牌なら判定打ち切り
	if gametbl:isriichideclared() then return mihajong.Call.None end -- リーチしているなら判定打ち切り

	--[=======[ 書き換え完了ここまで
	await 0: gosub *countseentiles
	dim haiCount, 80 -- 計算する牌
	repeat 14
		if (haiHand(cnt, PassivePlayer) != 0) then
			-- 手牌の中の、それぞれの牌の枚数を数える
			haiCount(haiHand(cnt, PassivePlayer))++
		end
	loop
	-- 捨てられた牌が役牌だった場合
	YakuhaiPon = 0
	if (haiCurrentSutehai == 35) then YakuhaiPon = 1 end
	if (haiCurrentSutehai == 36) then YakuhaiPon = 1 end
	if (haiCurrentSutehai == 37) then YakuhaiPon = 1 end
	if ((haiCurrentSutehai == 31)&&((hncnTurn/4 == 0)||((targetPlayer+32-hncnTurn)\4 == 0))) then YakuhaiPon = 1 end
	if ((haiCurrentSutehai == 32)&&((hncnTurn/4 == 1)||((targetPlayer+32-hncnTurn)\4 == 1))) then YakuhaiPon = 1 end
	if ((haiCurrentSutehai == 33)&&((hncnTurn/4 == 2)||((targetPlayer+32-hncnTurn)\4 == 2))) then YakuhaiPon = 1 end
	if ((haiCurrentSutehai == 34)&&((hncnTurn/4 == 3)||((targetPlayer+32-hncnTurn)\4 == 3))) then YakuhaiPon = 1 end
	-- 役牌を鳴いたらフラグを立てる
	-- ポンが失敗する場合は他家のロンがあるか河底牌の場合
	-- なのでこの時点でフラグを立てても特に問題はない
	if (YakuhaiPon) then haiYakuhaiNaki(PassivePlayer) = 1 end
	await 0: gosub *countseentiles
	if (haiCount(haiCurrentSutehai) >= 3) then
		-- すでに槓子を２つ持っているなら大明槓させる
		if (haiNumOfKangs(PassivePlayer) >= 2) then
			haiMinkan(PassivePlayer) = 1
			return
		end
	end
	if (currentShanten > 0) then -- すでにテンパってたら鳴かない
		if (currentShanten > Shanten) then -- 有効牌でなければ鳴かない
			if (haiCount(haiCurrentSutehai) >= 2) then -- ポンできるかどうかの判定
				if ((haiMenzen(PassivePlayer) == 0)||(YakuhaiPon == 1)) then
					-- すでに鳴いているか、役牌の場合はポン 対々狙いか一色狙いなら二鳴き
					haiPon(PassivePlayer) = 1
					return
				end else
					tmpYaojiuTilesCount = 0
					repeat 13
						switch haiHand(cnt, PassivePlayer)
							case 1: case 9: case 11: case 19: case 21: case 29
							case 31: case 32: case 33: case 34: case 35: case 36: case 37
								tmpYaojiuTilesCount++
						swend
					loop
					switch CurrentSutehai
						case 1: case 9: case 11: case 19: case 21: case 29
						case 31: case 32: case 33: case 34: case 35: case 36: case 37
							tmpYaojiuTilesCount++
					swend
					repeat haiNakiMianziDat(0, PassivePlayer)
						if (haiNakiMianziDat(0, PassivePlayer) == 0) then break end
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 100) > 30) then
							tmpYaojiuTilesCount++: continue
						end
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 1) then
							tmpYaojiuTilesCount++: continue
						end
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 9) then
							tmpYaojiuTilesCount++: continue
						end
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 7) then
							if ((haiNakiMianziDat(cnt, PassivePlayer) / 1000) < 4) then
								tmpYaojiuTilesCount++: continue
							end
						end
					loop
					if (tmpYaojiuTilesCount == 0) then
						if ((currentShanten == 1)&&(strmid(RuleConf, 19, 1) == "0")) then
							-- 喰い断をポン聴する
							haiPon(PassivePlayer) = 1
							return
						end
					end
				end
			end
		end
	end
#ifndef SANMAX
	-- チーするかどうかを判定する
	haiHand(13, PassivePlayer) = haiCurrentSutehai
	dim tmpHaiHand, 14
	repeat 14
		tmpHaiHand(cnt) = haiHand(cnt, PassivePlayer)
	loop
	tmp2Shanten = Shanten
	chosenNaki = 0
	repeat 3, 1: chiType = cnt
		targetPlayer = PassivePlayer: targetTile = cnt: gosub *getpaiinfo: await 0
		nakiCount1 = 0: nakiCount2 = 0: nakiCount3 = 0
		if ((haiCurrentSutehai-chiType) < 0) then
			continue -- バグ防止用
		end
		repeat 14
			await 0
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+1-chiType)&&(nakiCount1 < 1)) then
				haiHand(cnt, PassivePlayer) = 0
				nakiCount1++
			end
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+2-chiType)&&(nakiCount2 < 1)) then
				haiHand(cnt, PassivePlayer) = 0
				nakiCount2++
			end
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+3-chiType)&&(nakiCount3 < 1)) then
				haiHand(cnt, PassivePlayer) = 0
				nakiCount3++
			end
		loop
		targetPlayer = PassivePlayer: await 0: gosub *countshanten
		repeat 14
			haiHand(cnt, PassivePlayer) = tmpHaiHand(cnt)
		loop
		if (haiCurrentSutehai >= 30) then
			continue -- 字牌に順子なし
		end
		if ((nakiCount1 == 0)||(nakiCount2 == 0)||(nakiCount3 == 0)) then
			continue -- 不可能な鳴きの場合
		end
		if (currentShanten > (Shanten - 2)) then chosenNaki == cnt end
	loop
	Shanten = tmp2Shanten
	haiHand(13, PassivePlayer) = 0
	if (currentShanten > 0) then -- すでにテンパってたら鳴かない
--		if ((YakuhaiPon == 1)||(((haiCurrentSutehai/10) == (haiAimingYise(PassivePlayer)-1)))) then
		if (YakuhaiPon == 1) then
			-- すでに役牌を鳴いているならチー
			haiChi(PassivePlayer) = chosenNaki
		end else
			tmpYaojiuTilesCount = 0
			repeat 13
				await 0
				switch haiHand(cnt, PassivePlayer)
					case 1: case 9: case 11: case 19: case 21: case 29
					case 31: case 32: case 33: case 34: case 35: case 36: case 37
						tmpYaojiuTilesCount++
				swend
			loop
			switch haiCurrentSutehai
				case 1: case 9: case 11: case 19: case 21: case 29
				case 31: case 32: case 33: case 34: case 35: case 36: case 37
					tmpYaojiuTilesCount++
			swend
			repeat haiNakiMianziDat(0, PassivePlayer), 1
				await 0
				if (haiNakiMianziDat(0, PassivePlayer) == 0) then break end
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 100) > 30) then
					tmpYaojiuTilesCount++: continue
				end
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 1) then
					tmpYaojiuTilesCount++: continue
				end
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 9) then
					tmpYaojiuTilesCount++: continue
				end
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 7) then
					if ((haiNakiMianziDat(cnt, PassivePlayer) / 1000) < 4) then
						tmpYaojiuTilesCount++: continue
					end
				end
			loop
			if (tmpYaojiuTilesCount == 0) then
				if ((currentShanten == 1)&&(strmid(RuleConf, 19, 1) == "0")) then
					-- 喰い断を吃聴する
					haiChi(PassivePlayer) = chosenNaki
				end
			end
		end
	end
#endif
return
]=======]
end
