-- MiHaJong中国麻雀 デフォルトAI

---------------------------------------------------------------------------

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
function getiter (proxy) -- プロキシテーブルになっている場合、正しいイテレータを返すための処理
	if getmetatable(proxy) then
		return pairs(getmetatable(proxy).__index)
	else
		return pairs(proxy)
	end
end
function igetiter (proxy) -- プロキシテーブルになっている場合、正しいイテレータを返すための処理
	if getmetatable(proxy) then
		return ipairs(getmetatable(proxy).__index)
	else
		return ipairs(proxy)
	end
end

validtiles = {
	mihajong.Tile.Character[1], mihajong.Tile.Character[2], mihajong.Tile.Character[3],
	mihajong.Tile.Character[4], mihajong.Tile.Character[5], mihajong.Tile.Character[6],
	mihajong.Tile.Character[7], mihajong.Tile.Character[8], mihajong.Tile.Character[9],
	mihajong.Tile.Circle[1], mihajong.Tile.Circle[2], mihajong.Tile.Circle[3],
	mihajong.Tile.Circle[4], mihajong.Tile.Circle[5], mihajong.Tile.Circle[6],
	mihajong.Tile.Circle[7], mihajong.Tile.Circle[8], mihajong.Tile.Circle[9],
	mihajong.Tile.Bamboo[1], mihajong.Tile.Bamboo[2], mihajong.Tile.Bamboo[3],
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
		for k, v in getiter(mihajong.Tile.Flower) do
			if tile.tile == v then return true end
		end
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
	for k, v in igetiter(mihajong.Tile.Character) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function iscircle (tile) -- 筒子かどうか判定する
	for k, v in igetiter(mihajong.Tile.Circle) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isbamboo (tile) -- 索子かどうか判定する
	for k, v in igetiter(mihajong.Tile.Bamboo) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isyaojiu (tile) -- 么九牌かどうか
	for k, v in igetiter(yaojiutiles) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function iswind (tile) -- 風牌かどうか
	for k, v in getiter(mihajong.Tile.Wind) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function isdragon (tile) -- 三元牌かどうか
	for k, v in getiter(mihajong.Tile.Dragon) do
		if tile and (tile.tile == v) then return true end
	end
	return false
end
function ishonor (tile) -- 字牌かどうか
	return iswind(tile) or isdragon(tile)
end

function issequence (meld) -- 順子かどうか
	for k, v in getiter(mihajong.MeldType.Sequence) do
		if meld.type == v then return true end
	end
	return false
end
function istriplet (meld) -- 刻子かどうか
	for k, v in getiter(mihajong.MeldType.Triplet) do
		if meld.type == v then return true end
	end
	return false
end

function gettilenumeral (tile) -- 牌の数字(１～９)を取得する
	for k, v in igetiter(mihajong.Tile.Character) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	for k, v in igetiter(mihajong.Tile.Circle) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	for k, v in igetiter(mihajong.Tile.Bamboo) do
		if tile and (tile.tile == v) then return tonumber(k) end
	end
	return nil
end
function gettilesuit (tile) -- 牌の色を取得する
	if ischaracter(tile) then return 1
	elseif iscircle(tile) then return 2
	elseif isbamboo(tile) then return 3
	elseif ishonor(tile) then return 4
	else return nil end
end

function numofquads (gametbl) -- 槓子の数を数える
	local melds = gametbl:getmeld() -- 鳴き面子
	local count = 0 -- 返り値
	for i = 1, #melds do -- 各々の面子について
		for k, v in getiter(mihajong.MeldType.Quad.Exposed) do
			if melds[i].type == v then count = count + 1 end
		end
		for k, v in getiter(mihajong.MeldType.Quad.Added) do
			if melds[i].type == v then count = count + 1 end
		end
		if melds[i].type == mihajong.MeldType.Quad.Concealed then count = count + 1 end
	end
	return count
end

---------------------------------------------------------------------------

suit_table = {
	{mihajong.Tile.Character, mihajong.Tile.Circle, mihajong.Tile.Bamboo},
	{mihajong.Tile.Character, mihajong.Tile.Bamboo, mihajong.Tile.Circle},
	{mihajong.Tile.Circle, mihajong.Tile.Character, mihajong.Tile.Bamboo},
	{mihajong.Tile.Circle, mihajong.Tile.Bamboo, mihajong.Tile.Character},
	{mihajong.Tile.Bamboo, mihajong.Tile.Character, mihajong.Tile.Circle},
	{mihajong.Tile.Bamboo, mihajong.Tile.Circle, mihajong.Tile.Character}
}

function tiles_seq (gametbl)
	local tiles = gametbl:gettilesinhand()
	local melds = gametbl:getmeld()
	for k, meld in igetiter(melds) do
		if issequence(meld) then
			tiles[meld.tile    ] = tiles[meld.tile    ] + 1
			tiles[meld.tile + 1] = tiles[meld.tile + 1] + 1
			tiles[meld.tile + 2] = tiles[meld.tile + 2] + 1
		end
	end
	return tiles
end

function cnt_sanse_sanbugao (gametbl) -- 三色三歩高
	local tiles = tiles_seq(gametbl)
	local maxval, tile1, tile2, tile3 = 0, 0, 0, 0
	for i = 1, 5 do
		for j = 1, 6 do
			local count = 0
			for k = 1, 3 do
				for l = 1, 3 do
					if tiles[suit_table[j][k][i + k + l - 2]] > 0 then
						count = count + 1
					end
				end
			end
			if count > maxval then
				maxval = count
				tile1, tile2, tile3 = suit_table[j][1][i], suit_table[j][2][i + 1], suit_table[j][3][i + 2]
			end
		end
	end
	return maxval, tile1, tile2, tile3
end
function cnt_sanse_santongshun (gametbl) -- 三色三同順
	local tiles = tiles_seq(gametbl)
	local maxval, tile1, tile2, tile3 = 0, 0, 0, 0
	for i = 1, 7 do
		local count = 0
		for k = 1, 3 do
			for l = 1, 3 do
				if tiles[suit_table[1][k][i + l - 1]] > 0 then
					count = count + 1
				end
			end
		end
		if count > maxval then
			maxval = count
			tile1, tile2, tile3 = suit_table[1][1][i], suit_table[1][2][i], suit_table[1][3][i]
		end
	end
	return maxval, tile1, tile2, tile3
end
function cnt_hualong (gametbl) -- 花龍
	local tiles = tiles_seq(gametbl)
	local maxval, tile1, tile2, tile3 = 0, 0, 0, 0
	for i = 1, 6 do
		local count = 0
		for k = 1, 3 do
			if tiles[suit_table[i][k][k * 3 - 2]] > 0 then
				count = count + 1
			end
		end
		if count > maxval then
			maxval = count
			tile1, tile2, tile3 = suit_table[i][1][1], suit_table[i][2][4], suit_table[i][3][7]
		end
	end
	return maxval, tile1, tile2, tile3
end
function cnt_qinglong (gametbl) -- 清龍
	local tiles = tiles_seq(gametbl)
	local maxval, tile1, tile2, tile3 = 0, 0, 0, 0
	for i = 1, 5, 2 do
		local count = 0
		for k = 1, 3 do
			if tiles[suit_table[i][1][k * 3 - 2]] > 0 then
				count = count + 1
			end
		end
		if count > maxval then
			maxval = count
			tile1, tile2, tile3 = suit_table[i][1][1], suit_table[i][1][4], suit_table[i][1][7]
		end
	end
	return maxval, tile1, tile2, tile3
end

---------------------------------------------------------------------------

function ontsumo (gametbl) -- ＡＩの打牌
	local haiHand = gametbl:gethand()

	-- 花牌を抜く
	for cnt = 1, 14 do
		if isflower(haiHand[cnt]) then return mihajong.DiscardType.Flower, cnt end 
	end

	do -- 暗槓・加槓するか
		local flag, dahaiType, teDahai = ankan_decision(gametbl)
		if flag then return dahaiType, teDahai end
	end
	do -- 捨て牌を選択する
		local dahaiType, teDahai = discard_decision(gametbl)
		return dahaiType, teDahai
	end
	return mihajong.DiscardType.Normal, 14
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
	local furitenFlag = false
	for k, v in getiter(validtiles) do repeat
		if origShanten > 1 then
			if ishonor({["tile"] = v}) and (haiCount[v] + haiSeenCount[v] == 3) then break end -- 字牌のラス牌はとりあえず無視
		end
		if origShanten > 0 then -- 本来は既にテンパイしているときには行なっていなかった処理
			if haiCount[v] + haiSeenCount[v] == 4 then break end -- ４枚切れた牌
		end
		haiHand[tp], haiHand[14] = clone(haiHand[14]), {["tile"] = v, ["red"] = mihajong.DoraColor.Normal}
		local Shanten = gametbl:getshanten(haiHand)
		if Shanten == -1 then
			do
				local tmpHe, haiSutehai = false, gametbl:getdiscard()
				for idx = 1, #haiSutehai do -- 振聴になる牌を避ける
					if haiSutehai[idx].tile == v then tmpHe = true; break end
				end
				if tmpHe then furitenFlag = true end
			end
			local stat = gametbl:evaluate(true, haiHand)
			if stat and stat.points then
				tmpDiscardability[tp] = tmpDiscardability[tp] +
					stat.points * (4 - haiCount[v] - haiSeenCount[v]) + tmpde
			end
		elseif Shanten == (nowShanten - 1) then
			local tmpde2 = (4 - haiCount[v] - haiSeenCount[v])
			-- 時間がかかったらここで切り上げていた
			for i = 1, 14 do repeat
				if i > 1 then
					if haiHand[i] and haiHand[i - 1] and (haiHand[i].tile == haiHand[i - 1].tile) then
						tmpDiscardability[i] = tmpDiscardability[i - 1]
						break -- 同じ牌を２度調べない
					end
				end
				local xHand = clone(haiHand); xHand[i] = nil
				if gametbl:getshanten(xHand) == Shanten then
					local td = evaluate_hand(gametbl, cnt, i, clone(haiHand), haiDiscardability, tmpde2)
					for j = 1, 14 do tmpDiscardability[i] = tmpDiscardability[i] + td[j] end
				end
			until true end
		end
		haiHand = clone(hand)
	until true end
	if furitenFlag then tmpDiscardability[tp] = tmpDiscardability[tp] / 3 end -- フリテンだった場合
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
				for nom, tile in getiter(mihajong.Tile.Wind) do
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
				local yakuhailist = gametbl:getyakuhaiwind()
				if isdragon(tmpHand) then
					haiDiscardability[cnt] = haiDiscardability[cnt] - 99999 -- 三元牌のとき
				end
				for nom, tile in getiter(mihajong.Tile.Wind) do
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
		if
---[[
			false and
--]]
			nowShanten == 1 then -- 一向聴のとき
				local tmpDiscardability = evaluate_hand(gametbl, cnt, cnt, haiHand, haiDiscardability, 100000) -- 先読み判定
				for i = 1, 14 do haiDiscardability[cnt] = haiDiscardability[cnt] + tmpDiscardability[i] end
		elseif nowShanten == 0 then -- 聴牌したとき
			local tmpDiscardability = evaluate_hand(gametbl, cnt, cnt, haiHand, haiDiscardability, 100000) -- 先読み判定
			for i = 1, 14 do haiDiscardability[cnt] = haiDiscardability[cnt] + tmpDiscardability[i] end
		end
	until true end

	-- 捨牌を決定する
	local dahaiType, teDahai = mihajong.DiscardType.Normal, 14
	for cnt = 14, 1, -1 do
		if not do_not_discard[cnt] then
			if haiDiscardability[teDahai] < haiDiscardability[cnt] then
				teDahai = cnt
			end
		end
	end

	-- 長考してるふりをここでしていた
	return dahaiType, teDahai
end

---------------------------------------------------------------------------

function ondiscard (gametbl) -- 鳴き
	return decide_call(gametbl, 0)
end

function onkakan (gametbl)
	return decide_call(gametbl, 1)
end

function onankan (gametbl)
	return mihajong.Call.None
end

function decide_call (gametbl, ChanKanFlag) -- ＡＩの鳴き・栄和
	local haiHand = gametbl:gethand()
	local haiCurrentSutehai = gametbl:getcurrentdiscard()
	local haiNakiMianziDat = gametbl:getmeld()

	if isflower(haiCurrentSutehai) then -- 花牌の場合は戻る
		return mihajong.Call.None
	end

	haiHand[14] = nil
	local currentShanten = gametbl:getshanten(haiHand)
	haiHand[14] = clone(haiCurrentSutehai)

	local Shanten = gametbl:getshanten(haiHand)

	if Shanten == -1 then
		local yakustat = gametbl:evaluate(false, haiHand)
		local tenpaistat = gametbl:gettenpaistat(haiHand)
		local haiRon = true -- 出和了り
		if not yakustat.isvalid then -- 縛りを満たしていないなら
			haiRon = false -- チョンボになるから和がらないようにする
		end
		if haiRon then
			return mihajong.Call.Ron
		else
			return mihajong.Call.None
		end
	end
	if ChanKanFlag ~= 0 then return mihajong.Call.None end -- 搶槓の判定中なら判定打ち切り
	if gametbl:getdeckleft() == 0 then return mihajong.Call.None end -- 河底牌なら判定打ち切り

	local haiCount = gametbl:gettilesinhand() -- 手牌の中の、それぞれの牌の枚数を数える

	local haiSeenCount = gametbl:getseentiles()
	if haiCount[haiCurrentSutehai.tile] and (haiCount[haiCurrentSutehai.tile] >= 3) then
		-- すでに槓子を２つ持っているなら大明槓させる
		if numofquads(gametbl) >= 2 then
			return mihajong.Call.Kan
		end
	end

--[[
	if (currentShanten > 0) and -- すでにテンパってたら鳴かない
		(currentShanten > Shanten) and -- 有効牌でなければ鳴かない
		haiCount[haiCurrentSutehai.tile] and (haiCount[haiCurrentSutehai.tile] >= 2) then -- ポンできるかどうかの判定
			if (not gametbl:ismenzen()) or YakuhaiPon then
				-- すでに鳴いているか、役牌の場合はポン 対々狙いか一色狙いなら二鳴き
				return mihajong.Call.Pon
			else
				local tmpYaojiuTilesCount = 0
				for cnt = 1, 13 do
					if isyaojiu(haiHand[cnt]) then
						tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
					end
				end
				if isyaojiu(CurrentSutehai) then
					tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
				end
				for cnt = 1, #haiNakiMianziDat do
					if ishonor(haiNakiMianziDat[cnt]) then
						tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
					elseif gettilenumeral(haiNakiMianziDat[cnt]) == 1 then
						tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
					elseif gettilenumeral(haiNakiMianziDat[cnt]) == 9 then
						tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
					elseif gettilenumeral(haiNakiMianziDat[cnt]) == 7 then
						if issequence(haiNakiMianziDat[cnt]) then
							tmpYaojiuTilesCount = tmpYaojiuTilesCount + 1
						end
					end
				end
				if (tmpYaojiuTilesCount == 0) and (currentShanten == 1) and (gametbl:getrule("kuitan") == "yes") then
					return mihajong.Call.Pon -- 喰い断をポン聴する
				end
			end
	end
]]

		-- チーするかどうかを判定する
		haiHand[14] = clone(haiCurrentSutehai)
		local tmpHaiHand = clone(haiHand)
		local tmp2Shanten = Shanten
		local chosenNaki, haiChi = 0, 0
		for chiType = 1, 3 do repeat
			local nakiCount = {0, 0, 0}
			if (haiCurrentSutehai.tile - chiType) < 0 then break end -- バグ防止用
			for cnt = 1, 14 do
				if haiHand[cnt] then
					if (haiHand[cnt].tile == haiCurrentSutehai.tile + 1 - chiType) and (nakiCount[1] < 1) then
						haiHand[cnt] = nil
						nakiCount[1] = nakiCount[1] + 1
					elseif (haiHand[cnt].tile == haiCurrentSutehai.tile + 2 - chiType) and (nakiCount[2] < 1) then
						haiHand[cnt] = nil
						nakiCount[2] = nakiCount[2] + 1
					elseif (haiHand[cnt].tile == haiCurrentSutehai.tile + 3 - chiType) and (nakiCount[3] < 1) then
						haiHand[cnt] = nil
						nakiCount[3] = nakiCount[3] + 1
					end
				end
			end
			local Shanten = gametbl:getshanten(haiHand)
			haiHand = clone(tmpHaiHand)
			if ishonor(haiCurrentSutehai) or isflower(haiCurrentSutehai) then
				break -- 字牌に順子なし
			end
			if (nakiCount[1] == 0) or (nakiCount[2] == 0) or (nakiCount[3] == 0) then
				break -- 不可能な鳴きの場合
			end
			--if currentShanten > (Shanten - 2) then chosenNaki = chiType end
			do -- 三色三歩高とかの判定
				local func = {cnt_sanse_sanbugao, cnt_sanse_santongshun, cnt_hualong, cnt_qinglong}
				for k, f in pairs(func) do
					local val, tile1, tile2, tile3 = f(gametbl)
					if val > 7 then
						if     haiCurrentSutehai.tile == tile1 + chiType - 1 then haiChi = chiType
						elseif haiCurrentSutehai.tile == tile2 + chiType - 1 then haiChi = chiType
						elseif haiCurrentSutehai.tile == tile3 + chiType - 1 then haiChi = chiType
						end
					end
				end
			end
		until true end
		Shanten = tmp2Shanten
		haiHand[14] = nil

		if     haiChi == 1 then return mihajong.Call.Chii.Lower
		elseif haiChi == 2 then return mihajong.Call.Chii.Middle
		elseif haiChi == 3 then return mihajong.Call.Chii.Upper
		end
	--end
	return mihajong.Call.None
end
