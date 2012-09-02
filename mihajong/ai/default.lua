-- MiHaJong 1.7.0以降用 デフォルトAI
-- 先読みＡＩなのでたまに時間がかかる場合があります

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

function isflower (tile) -- 花牌かどうか判定
	if tile then -- nilだったらエラーになるのでそれを防止
		if     tile.tile == mihajong.Tile.Flower.Spring then return true
		elseif tile.tile == mihajong.Tile.Flower.Summer then return true
		elseif tile.tile == mihajong.Tile.Flower.Autumn then return true
		elseif tile.tile == mihajong.Tile.Flower.Winter then return true
		elseif tile.tile == mihajong.Tile.Flower.Plum   then return true
		elseif tile.tile == mihajong.Tile.Flower.Orchid then return true
		elseif tile.tile == mihajong.Tile.Flower.Chrys  then return true
		elseif tile.tile == mihajong.Tile.Flower.Bamboo then return true
		else return false end
	else return false
	end
end

function someone_hadakatanki (gametbl)
	for k = 1, 4 do
		if gametbl:getmeld(k) and (#(gametbl:getmeld(k)) == 4) then return true end
	end
	return false
end

---[=======[
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
	if gametbl:isshisanbuda() and gametbl:isfirstdraw() and (gametbl:getrule("shiisan_puutaa") ~= "no") return mihajong.DiscardType.Agari end
	if gametbl:isshisibuda() and gametbl:isfirstdraw() and (gametbl:getrule("shiisan_uushii") ~= "no") return mihajong.DiscardType.Agari end
	if gametbl:iskyuushu() and gametbl:isfirstdraw() then
		-- 九種九牌(浮きの2向聴、沈みの3向聴は流す)
		if (gametbl:isabovebase() and (gametbl:getshanten() > 1)) or (gametbl:getshanten() > 2) then
			-- 国士聴牌は流さない
			return mihajong.DiscardType.Kyuushu -- 流局
		end
	end
	-- リーチの場合は、和了れなければツモ切り
	if gametbl:isriichideclared() then return mihajong.DiscardType.Normal, 14 end

	local evaluation = {
		"do_not_discard", {};
		"haiDiscardability", {}; "MinScore", {}; "MaxScore", {};
		"MachihaiTotalTiles", {}; "MachihaiFuritenFlag", {};
	}
	
	do
		local flag, ev, dahaiType, teDahai = riichi_decision(gametbl, evaluation)
		if flag then return dahaiType, teDahai end
		evaluation = ev
	end
end

function riichi_decision (gametbl, evaluation)
	local nowShanten = gametbl:getshanten()
	local haiCount, haiSeenCount = gametbl:gettilesinhand(), gametbl:getseentiles()
	-- リーチをかけるかどうか
	local ev = evaluation
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
			local haiHand = tmpHaiHand
			haiHand[cnt], haiHand[14] = haiHand[14], nil
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
				local haiHand = tmpHaiHand
				haiHand[cnt], haiHand[14] = haiHand[14], tilecode
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
				if ev.MaxScore[tmpTileNum] ==-999999999 then Mev.axScore[tmpTileNum] = 0 end
			end
		until true end
		-- 時間がかかる時用の簡略版がここにあった

		local haiHand = gametbl:gethand()
		if Richiability > 0 then
			local dahaiType, teDahai = mihajong.DiscardType.Normal, 14
			for cnt = 14, 1, -1 do
				if ev.haiDiscardability[teDahai] < ev.haiDiscardability[cnt] then
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
				return true, ev, dahaiType, teDahai
			else return false, ev end
		end
	end
	return false, ev

--[====[ 書き換え完了ここまで
	-- チー、ポンした直後に暗槓/加槓できないようにするためのif処理
	-- チー、ポンした直後はツモ牌がない状態なので、それを利用する
	-- カンをした直後はツモ牌が嶺上牌になっている
	-- カンをしたあと、続けて暗槓をすることはかまわない
	if (haiHand(13, ActivePlayer) != 0) then
		await 0
		--[[ 暗槓するかどうか ]]
		dim tmpHaiHand, 14
		repeat 14
			tmpHaiHand(cnt) = haiHand(cnt, ActivePlayer)
		loop
		repeat 14
			if (haiHand(cnt, ActivePlayer) == 0) then continue end
			targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
			tmpnum = haiHand(cnt, ActivePlayer)
			repeat 14
				if (haiHand(cnt, ActivePlayer) == tmpnum) then
					haiHand(cnt, ActivePlayer) = 0
				end
			loop
			targetPlayer = ActivePlayer: await 0: gosub *countshanten
			repeat 14
				haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
			loop
			if ((haiTileInfo\128 / 64) == 1) then
				if (Shanten == (nowShanten+2)) then
					-- 長考してるふり
					repeat
						await 0
						nowTime = gettime(6)*1000+gettime(7)
						if (nowTime < startTime) then nowTime += 60000 end
						if ((nowTime-startTime) >= COMTHINKTIME) then break end
					loop
					teDahai = cnt+20
					break
				end
			end
		loop
		--[[ 加槓するかどうか ]]
		repeat 14
			Kakanability = 0
			if (haiHand(cnt, ActivePlayer) == 0) then continue end
			targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
			tmpnum = haiHand(cnt, ActivePlayer)
			haiHand(cnt, ActivePlayer) = 0
			targetPlayer = ActivePlayer: await 0: gosub *countshanten
			repeat 14
				haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
			loop
			repeat haiNakiMianziDat(0, ActivePlayer), 1
				if (haiNakiMianziDat(cnt, ActivePlayer)\100 == tmpnum) then
					if ((haiNakiMianziDat(cnt, ActivePlayer)/1000 >= 5)&&(haiNakiMianziDat(cnt, ActivePlayer)/1000 <= 7)) then Kakanability = 1 end
				end
			loop
			if (Kakanability == 1) then
				if (Shanten == nowShanten) then
					-- 長考してるふり
					repeat
						await 0
						nowTime = gettime(6)*1000+gettime(7)
						if (nowTime < startTime) then nowTime += 60000 end
						if ((nowTime-startTime) >= COMTHINKTIME) then break end
					loop
					teDahai = cnt+40
					break
				end
			end
		loop
		if (teDahai >= 20) then return end
	end
	--[[ それぞれを捨てたときの手の評価を計算する ]]
	dim haiDiscardability, 14
	startTime = gettime(6)*1000+gettime(7)
	Yishanten = 0
	dim tmpDiscardability, 14

	WanzCount = 0: PinzCount = 0: SouzCount = 0
	YaojiuCount = 0: KeziCount = 0
	repeat 14
		if (haiHand(cnt, ActivePlayer)/10 == 0) then WanzCount++ end
		if (haiHand(cnt, ActivePlayer)/10 == 1) then PinzCount++ end
		if (haiHand(cnt, ActivePlayer)/10 == 2) then SouzCount++ end
		if ((haiHand(cnt, ActivePlayer)/10 == 3)||(haiHand(cnt, ActivePlayer)\10 == 1)||(haiHand(cnt, ActivePlayer)\10 == 9)) then YaojiuCount++ end
		targetTile = cnt: await 0: gosub *getpaiinfo
		if (((haiTileInfo \ 2) == 1)||((haiTileInfo \ 8 / 4) == 1)) then KeziCount++ end
	loop
	repeat haiNakiMianziDat(0, ActivePlayer), 1
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 0) then WanzCount += 3 end
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 1) then PinzCount += 3 end
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 2) then SouzCount += 3 end
		if (haiNakiMianziDat(cnt, ActivePlayer)/1000 >= 4) then KeziCount += 3 end
	loop
--	if (WanzCount >= 8) then haiAimingYise(ActivePlayer) = 1 end -- 一色手を狙ってみよう！
--	if (PinzCount >= 8) then haiAimingYise(ActivePlayer) = 2 end
--	if (SouzCount >= 8) then haiAimingYise(ActivePlayer) = 3 end
--	if (KeziCount >= 6) then
--		haiAimingDuidui(ActivePlayer) = 1 -- どうやら対子場のようだ
--	end

	repeat 14
		targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
		tmpHand = haiHand(cnt, ActivePlayer)
		-- 存在しない牌の場合
		if (haiHand(cnt, ActivePlayer) == 0) then haiDiscardability(cnt) = -9999999: continue end
		-- 向聴数から、大まかな評価値を算出
		haiHand(cnt, ActivePlayer) = 0
		targetPlayer = ActivePlayer: await 0: gosub *countshanten
		haiDiscardability(cnt) = (6-Shanten)*1000
		haiHand(cnt, ActivePlayer) = tmpHand
		if (haiTileInfo == 0) then
			if (tmpHand >= 30) then
				haiDiscardability(cnt) += 900 -- 字牌の浮き牌
				if (tmpHand >= 35) then
					haiDiscardability(cnt) -= 100 -- 三元牌のとき
				end
				if (tmpHand == 31) then
					if ((hncnTurn/4 == 0)||((ActivePlayer+32-hncnTurn)\4 == 0)) then
						haiDiscardability(cnt) -= 100 -- 東が役牌のとき
					end
				end
				if (tmpHand == 32) then
					if ((hncnTurn/4 == 1)||((ActivePlayer+32-hncnTurn)\4 == 1)) then
						haiDiscardability(cnt) -= 100 -- 南が役牌のとき
					end
				end
				if (tmpHand == 33) then
					if ((hncnTurn/4 == 2)||((ActivePlayer+32-hncnTurn)\4 == 2)) then
						haiDiscardability(cnt) -= 100 -- 西が役牌のとき
					end
				end
				if (tmpHand == 34) then
					if ((hncnTurn/4 == 3)||((ActivePlayer+32-hncnTurn)\4 == 3)) then
						haiDiscardability(cnt) -= 100 -- 北が役牌のとき
					end
				end
			end else
				haiDiscardability(cnt) += 700 -- 数牌の浮き牌
			end
		end
		else
			if ((haiTileInfo \ 16 / 8) == 1) then haiDiscardability(cnt) += 500 end -- 辺張
			else
				if ((haiTileInfo \ 64 / 32) == 1) then haiDiscardability(cnt) += 300 end -- 嵌張
				else
					if (haiAimingDuidui(ActivePlayer)) then
						if ((haiTileInfo \ 32 / 16) == 1) then haiDiscardability(cnt) += 100 end -- 対子場の時は両面
					end else
						if ((haiTileInfo \ 8 / 4) == 1) then haiDiscardability(cnt) += 100 end -- そうでなければ対子
					end
				end
			end
		end
		if (haiHandAkadora(cnt, ActivePlayer) >= 0) then
			haiDiscardability(cnt) -= 50
		end
		if ((haiTileInfo \ 2) == 1) then
			-- 役牌の暗刻を切らないようにする
			if (tmpHand >= 30) then
				if (tmpHand >= 35) then
					haiDiscardability(cnt) -= 99999 -- 三元牌のとき
				end
				if (tmpHand == 31) then
					if ((hncnTurn/4 == 0)||((ActivePlayer+32-hncnTurn)\4 == 0)) then
						haiDiscardability(cnt) -= 99999 -- 東が役牌のとき
					end
				end
				if (tmpHand == 32) then
					if ((hncnTurn/4 == 1)||((ActivePlayer+32-hncnTurn)\4 == 1)) then
						haiDiscardability(cnt) -= 99999 -- 南が役牌のとき
					end
				end
				if (tmpHand == 33) then
					if ((hncnTurn/4 == 2)||((ActivePlayer+32-hncnTurn)\4 == 2)) then
						haiDiscardability(cnt) -= 99999 -- 西が役牌のとき
					end
				end
				if (tmpHand == 34) then
					if ((hncnTurn/4 == 3)||((ActivePlayer+32-hncnTurn)\4 == 3)) then
						haiDiscardability(cnt) -= 99999 -- 北が役牌のとき
					end
				end
			end
		end
		haiDiscardability(cnt) += (haiSeenCount(tmpHand)*10) -- 場に見えてる個数
		if (tmpHand >= 30) then haiDiscardability(cnt) += 50 end
		else
			if ((tmpHand\10 == 1)||(tmpHand\10 == 9)) then haiDiscardability(cnt) += 40 end
			if ((tmpHand\10 == 2)||(tmpHand\10 == 8)) then haiDiscardability(cnt) += 30 end
			if ((tmpHand\10 == 3)||(tmpHand\10 == 7)) then haiDiscardability(cnt) += 20 end
			if ((tmpHand\10 == 4)||(tmpHand\10 == 6)) then haiDiscardability(cnt) += 10 end
		end
		haiHand(cnt, ActivePlayer) = tmpHand
#if 0 --[[ いろいろあったんで、やめた ]]
		if (nowShanten >= 2) then
			-- 染めてみようかな？？
			if ((WanzCount >= 11)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((PinzCount >= 11)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((SouzCount >= 11)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) then haiDiscardability(cnt) += 10000 end
			if ((WanzCount >= 10)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((PinzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((SouzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) then haiDiscardability(cnt) += 10000 end
			if ((WanzCount >= 8)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((PinzCount >= 8)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((SouzCount >= 8)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) then haiDiscardability(cnt) += 10000 end
		end
		if (nowShanten == 1) then
			if ((WanzCount >= 12)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 20000 end
			if ((PinzCount >= 12)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 20000 end
			if ((SouzCount >= 12)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) then haiDiscardability(cnt) += 20000 end
			if ((WanzCount >= 10)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((PinzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) then haiDiscardability(cnt) += 10000 end
			if ((SouzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) then haiDiscardability(cnt) += 10000 end
		end
		if (nowShanten >= 1) then
			-- やっぱタンヤオ狙いでしょっ！
			if ((YaojiuCount <= 3)&&((tmpHand/10 == 3)||(tmpHand\10 == 1)||(tmpHand\10 == 9))) then haiDiscardability(cnt) += 30000 end
		end
#endif
		if (nowShanten == 2) then -- 二向聴でも先読みさせてみる
			Yishanten = 1
			tmpXTileNum = cnt
			if (tmpXTileNum > 0) then
				if (haiHand(tmpXTileNum, ActivePlayer) == haiHand(tmpXTileNum-1, ActivePlayer)) then
					continue -- 同じ牌を２度調べない
				end
			end
			repeat 38
				await 0
				if (cnt\10 == 0) then continue end
				if (haiCount(cnt)+haiSeenCount(cnt) == 4) then continue end -- ４枚見えてる
				if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) then continue end -- 字牌のラス牌はとりあえず無視
				haiHand(tmpXTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				tmpXde = 4-haiCount(cnt)-haiSeenCount(cnt)
				if (Shanten == 1) then -- 次のツモ牌で一向聴の場合
					nowTime = gettime(6)*1000+gettime(7)
					if (nowTime < startTime) then nowTime += 60000 end
					if ((nowTime-startTime) < COMTHINKTIME) then
						repeat 14
							nowTime = gettime(6)*1000+gettime(7)
							tmpTileNum = cnt
							tmpTile2a = haiHand(cnt, ActivePlayer)
							if (tmpTileNum > 0) then
								if (haiHand(tmpTileNum, ActivePlayer) == haiHand(tmpTileNum-1, ActivePlayer)) then
									continue -- 同じ牌を２度調べない
								end
							end
							repeat 38
								nowTime = gettime(6)*1000+gettime(7)
								await 0
								if (cnt\10 == 0) then continue end
								if (haiCount(cnt)+haiSeenCount(cnt) == 4) then continue end -- ４枚見えてる
								if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) then continue end -- 字牌のラス牌はとりあえず無視
								haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
								haiHand(13, ActivePlayer) = cnt
								await 0: gosub *countshanten
								tmpde = 4-haiCount(cnt)-haiSeenCount(cnt)
								if (Shanten == 0) then -- 次のツモ牌で聴牌の場合
									nowTime = gettime(6)*1000+gettime(7)
									if (nowTime < startTime) then nowTime += 60000 end
									if ((nowTime-startTime) < COMTHINKTIME) then
										repeat 14
											nowTime = gettime(6)*1000+gettime(7)
											if (nowTime < startTime) then nowTime += 60000 end
											if ((nowTime-startTime) >= COMTHINKTIME) then break end
											tmpTile2 = haiHand(cnt, ActivePlayer)
											tmpTileNum2 = cnt
											repeat 38
												nowTime = gettime(6)*1000+gettime(7)
												await 0
												if (cnt\10 == 0) then continue end -- 存在しない牌
												if (haiCount(cnt)+haiSeenCount(cnt) == 4) then continue end -- ４枚切れた牌
												tmpHe = 0: tmpHai = cnt
												repeat haiSutehai(0, targetPlayer), 1
													if (haiSutehai(cnt, targetPlayer) == tmpHai) then
														tmpHe = 1: break
													end
												loop
												if (tmpHe == 1) then tmpDiscardability == 0: break end -- 振聴になる牌を避ける
												haiHand(tmpTileNum2, ActivePlayer) = haiHand(13, ActivePlayer)
												haiHand(13, ActivePlayer) = cnt
												await 0: gosub *countshanten
												if (Shanten == -1) then
													TsumoAgari = 1: await 0: gosub *countyaku
													tmpDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
												end
												haiHand(13, ActivePlayer) = haiHand(tmpTileNum2, ActivePlayer)
												haiHand(tmpTileNum2, ActivePlayer) = tmpTile2
											loop
										loop
									end else -- 処理が遅い時
										repeat 14
											tmpDiscardability(tmpTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
										loop
									end
								end
								haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
								haiHand(tmpTileNum, ActivePlayer) = tmpTile2a
								nowTime = gettime(6)*1000+gettime(7)
								if (nowTime < startTime) then nowTime += 60000 end
								if ((nowTime-startTime) < COMTHINKTIME) then break end
							loop
							nowTime = gettime(6)*1000+gettime(7)
							if (nowTime < startTime) then nowTime += 60000 end
							if ((nowTime-startTime) < COMTHINKTIME) then break end
						loop
					end else -- 処理が遅い時
						repeat 14
							tmpDiscardability(tmpXTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpXde
						loop
					end
				end
				haiHand(13, ActivePlayer) = haiHand(tmpXTileNum, ActivePlayer)
				haiHand(tmpXTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) then nowTime += 60000 end
				if ((nowTime-startTime) < COMTHINKTIME) then break end
			loop
		end
		if (nowShanten == 1) then -- 一向聴のとき
			Yishanten = 1
			tmpTileNum = cnt
			if (tmpTileNum > 0) then
				if (haiHand(tmpTileNum, ActivePlayer) == haiHand(tmpTileNum-1, ActivePlayer)) then
					continue -- 同じ牌を２度調べない
				end
			end
			repeat 38
				await 0
				if (cnt\10 == 0) then continue end
				if (haiCount(cnt)+haiSeenCount(cnt) == 4) then continue end -- ４枚見えてる
				if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) then continue end -- 字牌のラス牌はとりあえず無視
				haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				tmpde = 4-haiCount(cnt)-haiSeenCount(cnt)
				if (Shanten == 0) then -- 次のツモ牌で聴牌の場合
					nowTime = gettime(6)*1000+gettime(7)
					if (nowTime < startTime) then nowTime += 60000 end
					if ((nowTime-startTime) < COMTHINKTIME) then
						repeat 14
							nowTime = gettime(6)*1000+gettime(7)
							if (nowTime < startTime) then nowTime += 60000 end
							if ((nowTime-startTime) >= COMTHINKTIME) then break end
							tmpTile2 = haiHand(cnt, ActivePlayer)
							tmpTileNum2 = cnt
							repeat 38
								nowTime = gettime(6)*1000+gettime(7)
								if (nowTime < startTime) then nowTime += 60000 end
								if ((nowTime-startTime) >= COMTHINKTIME) then break end
								await 0
								if (cnt\10 == 0) then continue end -- 存在しない牌
								if (haiCount(cnt)+haiSeenCount(cnt) == 4) then continue end -- ４枚切れた牌
								tmpHe = 0: tmpHai = cnt
								repeat haiSutehai(0, targetPlayer), 1
									if (haiSutehai(cnt, targetPlayer) == tmpHai) then
										tmpHe = 1: break
									end
								loop
								if (tmpHe == 1) then tmpDiscardability == 0: break end -- 振聴になる牌を避ける
								haiHand(tmpTileNum2, ActivePlayer) = haiHand(13, ActivePlayer)
								haiHand(13, ActivePlayer) = cnt
								await 0: gosub *countshanten
								if (Shanten == -1) then
									TsumoAgari = 1: await 0: gosub *countyaku
									tmpDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
								end
								haiHand(13, ActivePlayer) = haiHand(tmpTileNum2, ActivePlayer)
								haiHand(tmpTileNum2, ActivePlayer) = tmpTile2
							loop
						loop
					end else -- 処理が遅い時
						repeat 14
							tmpDiscardability(tmpTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
						loop
					end
				end
				haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
				haiHand(tmpTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) then nowTime += 60000 end
				if ((nowTime-startTime) < COMTHINKTIME) then break end
			loop
		end
		if (nowShanten == 0) then -- 聴牌したとき
			tmpTileNum = cnt
			repeat 38
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) then nowTime += 60000 end
				if ((nowTime-startTime) >= COMTHINKTIME) then break end
				if (cnt\10 == 0) then continue end
				haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				if (Shanten == -1) then
					TsumoAgari = 1: await 0: gosub *countyaku
					if (haiDiscardability(tmpTileNum) >= 10000) then haiDiscardability(tmpTileNum) -= 10000 end
					haiDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
				end
				haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
				haiHand(tmpTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) then nowTime += 60000 end
				if ((nowTime-startTime) < COMTHINKTIME) then break end
			loop
		end
	loop
	nowTime = gettime(6)*1000+gettime(7)
	if (nowTime < startTime) then nowTime += 60000 end
	if ((Yishanten)&&((nowTime-startTime) < COMTHINKTIME)) then
		-- 一向聴のとき
		repeat 14
			haiDiscardability(cnt) += tmpDiscardability(cnt)+100000
		loop
	end
	-- オープン立直の待ち牌を捨てない(捨ててはならない！)ようにする
	repeat 14
		if (haiHand(cnt, ActivePlayer) < 40) then
			if (haiOpenRichiMachihai(haiHand(cnt, ActivePlayer)) == 1) then
				haiDiscardability(cnt) = -999999999
			end
		end
	loop
	-- 大三元の包になるような牌を捨てないようにする
	await 0: gosub *countseentiles
	if (nowShanten > 1) then
		-- ノーチャンスか・振聴となるか・白を鳴いてるか・発を鳴いてるか・中を鳴いてるか
		tmpDiscardabilityChkFlag = 0
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) then continue end
			if (haiNakiMianziDat(0, tmpchkcnt) == 0) then continue end
			repeat haiNakiMianziDat(0, tmpchkcnt)
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 35) then
					tmpDiscardabilityChkFlag += 4
				end
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 36) then
					tmpDiscardabilityChkFlag += 2
				end
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 37) then
					tmpDiscardabilityChkFlag += 1
				end
			loop
			if (haiSutehai(0, tmpchkcnt) == 0) then continue end
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 35)&&(tmpDiscardabilityChkFlag == 3) then tmpDiscardabilityChkFlag += 8 end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 36)&&(tmpDiscardabilityChkFlag == 5) then tmpDiscardabilityChkFlag += 8 end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 37)&&(tmpDiscardabilityChkFlag == 6) then tmpDiscardabilityChkFlag += 8 end
			loop
			if ((haiSeenCount(35) >= 2)&&(tmpDiscardabilityChkFlag == 3)) then tmpDiscardabilityChkFlag += 16 end
			if ((haiSeenCount(36) >= 2)&&(tmpDiscardabilityChkFlag == 5)) then tmpDiscardabilityChkFlag += 16 end
			if ((haiSeenCount(37) >= 2)&&(tmpDiscardabilityChkFlag == 6)) then tmpDiscardabilityChkFlag += 16 end
		loop
		repeat 14
			if ((tmpDiscardabilityChkFlag == 3)&&(haiHand(cnt, ActivePlayer) == 35)) then
				haiDiscardability(cnt) = -799999999
			end
			if ((tmpDiscardabilityChkFlag == 5)&&(haiHand(cnt, ActivePlayer) == 36)) then
				haiDiscardability(cnt) = -799999999
			end
			if ((tmpDiscardabilityChkFlag == 6)&&(haiHand(cnt, ActivePlayer) == 37)) then
				haiDiscardability(cnt) = -799999999
			end
		loop
	end
	-- 大四喜の包になるような牌を捨てないようにする
	if (nowShanten > 1) then
		-- ノーチャンスか・振聴となるか・東を鳴いてるか
		-- ・南を鳴いてるか・西を鳴いてるか・北を鳴いてるか
		tmpDiscardabilityChkFlag = 0
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) then continue end
			if (haiNakiMianziDat(0, tmpchkcnt) == 0) then continue end
			repeat haiNakiMianziDat(0, tmpchkcnt)
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 31) then
					tmpDiscardabilityChkFlag += 8
				end
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 32) then
					tmpDiscardabilityChkFlag += 4
				end
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 33) then
					tmpDiscardabilityChkFlag += 2
				end
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 34) then
					tmpDiscardabilityChkFlag += 1
				end
			loop
			if (haiSutehai(0, tmpchkcnt) == 0) then continue end
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 31)&&(tmpDiscardabilityChkFlag == 7) then tmpDiscardabilityChkFlag += 16 end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 32)&&(tmpDiscardabilityChkFlag == 11) then tmpDiscardabilityChkFlag += 16 end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 33)&&(tmpDiscardabilityChkFlag == 13) then tmpDiscardabilityChkFlag += 16 end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 34)&&(tmpDiscardabilityChkFlag == 14) then tmpDiscardabilityChkFlag += 16 end
			loop
			if ((haiSeenCount(31) >= 3)&&(tmpDiscardabilityChkFlag == 7)) then tmpDiscardabilityChkFlag += 32 end
			if ((haiSeenCount(32) >= 3)&&(tmpDiscardabilityChkFlag == 11)) then tmpDiscardabilityChkFlag += 32 end
			if ((haiSeenCount(33) >= 3)&&(tmpDiscardabilityChkFlag == 13)) then tmpDiscardabilityChkFlag += 32 end
			if ((haiSeenCount(34) >= 3)&&(tmpDiscardabilityChkFlag == 14)) then tmpDiscardabilityChkFlag += 32 end
		loop
		repeat 14
			if ((tmpDiscardabilityChkFlag == 7)&&(haiHand(cnt, ActivePlayer) == 31)) then
				haiDiscardability(cnt) = -799999999
			end
			if ((tmpDiscardabilityChkFlag == 11)&&(haiHand(cnt, ActivePlayer) == 32)) then
				haiDiscardability(cnt) = -799999999
			end
			if ((tmpDiscardabilityChkFlag == 13)&&(haiHand(cnt, ActivePlayer) == 33)) then
				haiDiscardability(cnt) = -799999999
			end
			if ((tmpDiscardabilityChkFlag == 14)&&(haiHand(cnt, ActivePlayer) == 34)) then
				haiDiscardability(cnt) = -799999999
			end
		loop
	end
	-- 一色手を警戒する
	if (nowShanten > 1) then
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) then continue end
			tmpDiscardabilityChkCount1 = 0
			tmpDiscardabilityChkCount2 = 0
			tmpDiscardabilityChkCount3 = 0
			if (haiSutehai(0, tmpchkcnt) == 0) then continue end
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 0) then tmpDiscardabilityChkCount1++ end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 1) then tmpDiscardabilityChkCount2++ end
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 2) then tmpDiscardabilityChkCount3++ end
			loop
			repeat 14
				if ((tmpDiscardabilityChkCount2 > 0)&&(tmpDiscardabilityChkCount3 > 0)) then
					if (((tmpDiscardabilityChkCount1*1000)/(tmpDiscardabilityChkCount2+tmpDiscardabilityChkCount3)) < 200) then
						if ((haiHand(cnt, ActivePlayer) /10) == 0) then
							haiDiscardability(cnt) = 0
						end
					end
				end
				if ((tmpDiscardabilityChkCount1 > 0)&&(tmpDiscardabilityChkCount3 > 0)) then
					if (((tmpDiscardabilityChkCount2*1000)/(tmpDiscardabilityChkCount1+tmpDiscardabilityChkCount3)) < 200) then
						if ((haiHand(cnt, ActivePlayer) /10) == 1) then
							haiDiscardability(cnt) = 0
						end
					end
				end
				if ((tmpDiscardabilityChkCount1 > 0)&&(tmpDiscardabilityChkCount2 > 0)) then
					if (((tmpDiscardabilityChkCount3*1000)/(tmpDiscardabilityChkCount1+tmpDiscardabilityChkCount2)) < 200) then
						if ((haiHand(cnt, ActivePlayer) /10) == 2) then
							haiDiscardability(cnt) = 0
						end
					end
				end
			loop
		loop
	end
	-- 国士無双を警戒する
	if (nowShanten > 1) then
		repeat 4: tmpchkcnt = 0
			tmpDiscardabilityChkCount1 = 0
			tmpDiscardabilityChkCount2 = 0
			if (tmpchkcnt == hncnZijia) then continue end
			if (haiSutehai(0, tmpchkcnt) == 0) then continue end
			repeat haiSutehai(0, tmpchkcnt), 1
				if (((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) < 3)&&((haiSutehai(cnt, tmpchkcnt) \ 10) > 1)&&((haiSutehai(cnt, tmpchkcnt) \ 10) < 9)) then tmpDiscardabilityChkCount1++ end
				tmpDiscardabilityChkCount2++
			loop
			repeat 14
				if (tmpDiscardabilityChkCount2 > 0) then
					if (((tmpDiscardabilityChkCount1*1000)/(tmpDiscardabilityChkCount2)) > 840) then
						if (((haiHand(cnt, ActivePlayer) \10) == 1)||((haiHand(cnt, ActivePlayer) \10) == 9)||(haiHand(cnt, ActivePlayer) > 30)) then
							haiDiscardability(cnt) = 0
						end
					end
				end
			loop
		loop
	end
	-- 警戒するプレーヤーを決定する
	dim haiMarkingPlayer, 4
	repeat 4
		if (cnt == ActivePlayer) then continue --[[ 自分自身の場合は飛ばす ]] end
		if (haiNakiMianziDat(0, cnt) >= 2) then
			haiMarkingPlayer(cnt) = 1 --[[ 二面子以上鳴いていたら降りる ]]
		end
		if (haiRichi(cnt) > 0) then
			haiMarkingPlayer(cnt) = 1 --[[ 誰かが立直していたら降りる！ ]]
		end
		if (haiNakiMianziDat(0, cnt) == 1) then
			if ((haiRinshanPointer - (haiDeadTiles-1) - haiPointer) < 30) then
				--[[ 残り３０枚未満で１副露の者がいた場合は降りる ]]
				haiMarkingPlayer(cnt) = 1
			end
		end
	loop
	-- オリるかどうかの判定…和了り手の得点を調べる
	TmpMinScore = 999999999
	TmpMaxScore = -999999999
	repeat 14
		if (haiHand(cnt, ActivePlayer) == 0) then continue end
		tmpTileCodeNum = haiHand(cnt, ActivePlayer)
		tmpTileNum = cnt
		MinScore(cnt) = 999999999
		MaxScore(cnt) = -999999999
		repeat 38
			if (cnt\10 == 0) then continue end
			haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
			haiHand(13, ActivePlayer) = cnt
			await 0: gosub *countshanten
			if (Shanten == -1) then
				-- ここで計算されるのはダマ聴で自摸和のときの点数
				TsumoAgari = 1: await 0: gosub *countyaku
				if (haiDiscardability(tmpTileNum) >= 10000) then haiDiscardability(tmpTileNum) -= 10000 end
				haiDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
				if (MinScore(tmpTileNum) > (haiHan+haiBonusHan+haiDoraQuantity)) then MinScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity) end
				if (MaxScore(tmpTileNum) < (haiHan+haiBonusHan+haiDoraQuantity)) then MaxScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity) end
			end
			haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
			haiHand(tmpTileNum, ActivePlayer) = tmpHandTileCode
			if (MinScore(tmpTileNum) == 999999999) then MinScore(tmpTileNum) = 0 end
			if (MaxScore(tmpTileNum) ==-999999999) then MaxScore(tmpTileNum) = 0 end
		loop
		haiHand(cnt, ActivePlayer) = tmpTileCodeNum
		if (MinScore(tmpTileNum) < TmpMinScore) then TmpMinScore = MinScore(tmpTileNum) end
		if (MaxScore(tmpTileNum) > TmpMinScore) then TmpMaxScore = MaxScore(tmpTileNum) end
	loop
	if (TmpMaxScore >= 3000) then
		-- とりあえず、跳満以上聴牌なら全ツッパする
		repeat 4: haiMarkingPlayer(cnt) = 0: loop
	end
	-- 降り打ちモードのための処理
	repeat 14: tmphaiindex = cnt
		repeat 4: tmpchkcnt = cnt
			if (haiMarkingPlayer(tmpchkcnt) == 0) then break end
			tmpflag = 0
			if (tmpflag == 0) then
				-- 合わせ打ちができるなら合わせ打ち
				if ((haiSutehai(haiSutehai(0, (ActivePlayer+3)\4), (ActivePlayer+3)\4) \ 100) == haiHand(tmphaiindex, ActivePlayer)) then
					haiDiscardability(tmphaiindex) += 7000000
					tmpflag = 1
				end
			end
			repeat haiSutehai(0, tmpchkcnt), 1
				if (haiHand(tmphaiindex, ActivePlayer) == (haiSutehai(cnt, tmpchkcnt) \ 100)) then
					-- 警戒しているプレーヤーの現物があれば優先的にそれを捨てる
					if (haiHand(tmphaiindex, ActivePlayer) < 30) then
						haiDiscardability(tmphaiindex) += 10000000
					end else
						haiDiscardability(tmphaiindex) += 5000000
					end
					tmpflag = 1
					break
				end
			loop
			if (tmpflag == 0) then
				repeat haiSutehai(0, tmpchkcnt), 1
					if ((haiSutehai(cnt, tmpchkcnt) \ 100) < 30) then
						-- 合わせ打ちも無理なら筋牌
						if (((haiSutehai(cnt, tmpchkcnt) \ 10)\3) == ((haiHand(tmphaiindex, ActivePlayer) \ 10)\3)) then
							haiDiscardability(tmphaiindex) += 1200000
							break
						end
					end
				loop
			end
		loop
	loop
	-- 捨牌を決定する
	repeat 14
		if (haiDiscardability(teDahai) < haiDiscardability(13-cnt)) then
			teDahai = 13-cnt
		end
	loop
#ifdef SANMAX
	-- 決定された捨牌が北だった場合、捨てる代わりに抜く
	if ((strmid(RuleConf, 30, 1) != "0")&&(haiHand(13, ActivePlayer) != 0)) then
		if (haiHand((teDahai\20), ActivePlayer) == 34) then
			teDahai += 80
		end
	end
#endif
	-- 長考してるふり
	repeat
		await 0
		nowTime = gettime(6)*1000+gettime(7)
		if (nowTime < startTime) then nowTime += 60000 end
		if ((nowTime-startTime) >= COMTHINKTIME) then break end
	loop
return
--]====]
end
--]=======]
--[=======[
--[[ ＡＩの鳴き・栄和 ]]
*compnaki
	--[[haiHand(13, PassivePlayer) = 0: haiHandAkaDora(13, PassivePlayer) = 0: return]] --[[ 何もせず戻る(デバッグ用) ]]
	if (haiCurrentSutehai > 70) then
		--[[ 花牌の場合は戻る ]]
		haiHand(13, PassivePlayer) = 0
		haiHandAkaDora(13, PassivePlayer) = 0
		return
	end

	haiHand(13, PassivePlayer) = 0
	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	currentShanten = Shanten
	haiHand(13, PassivePlayer) = haiCurrentSutehai

	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	TsumoAgari = 0: targetPlayer = PassivePlayer: await 0: gosub *countyaku
	targetPlayer = PassivePlayer: await 0: gosub *chkFuriten
	-- 暗槓に対する搶槓の判定で、国士聴牌でない場合は戻る
	if ((ShisanyaoShanten != 0)&&(ChanKanFlag == 2)) then
		haiHand(13, PassivePlayer) = 0
		haiHandAkaDora(13, PassivePlayer) = 0
		return
	end
	haiHand(13, PassivePlayer) = haiCurrentSutehai
	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	if (Shanten == -1) then
		haiRon(PassivePlayer) = 1 -- 出和了り
		haiHand(13, PassivePlayer) = haiCurrentSutehai
		haiHandAkaDora(13, PassivePlayer) = haiCurrentSutehaiAkaDora
		if ((haiHan <= hncnShibari)||(haiFuriten == 1)||(haiDoujunFuriten(PassivePlayer))||((strmid(RuleConf, 39, 1) != "0")&&(haiRichi(PassivePlayer) == 0))) then
			haiRon(PassivePlayer) = 0
			haiHand(13, PassivePlayer) = 0
			haiHandAkaDora(13, PassivePlayer) = 0
		end
		return
	end
	haiHand(13, PassivePlayer) = 0
	haiHandAkaDora(13, PassivePlayer) = 0
	if (ChanKanFlag == 1) then return end -- 搶槓の判定中なら判定打ち切り
	if (haiPointer >= (haiRinshanPointer - (haiDeadTiles-1))) then return end -- 河底牌なら判定打ち切り
	if (haiRichi(PassivePlayer) > 0) then return end -- リーチしているなら判定打ち切り
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
