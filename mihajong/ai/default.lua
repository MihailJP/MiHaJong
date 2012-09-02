-- MiHaJong 1.7.0以降用 デフォルトAI
-- 先読みＡＩなのでたまに時間がかかる場合があります

--[=======[
/*=============================
 * HSP麻雀クライアントMiHaJong
 *        [AIルーチン]
 *=============================
 */

#define COMTHINKTIME 600

/* 作るのが面倒なので全キャラ共通のＡＩにしてます */
/* 先読みＡＩなのでたまに時間がかかる場合があります */

/* ＡＩの打牌 */
*compdahai
	/*teDahai = 13: return*/ /* ツモ切りだけを行う(デバッグ用) */
	startTime = gettime(6)*1000+gettime(7)
	/* ここに思考ルーチンが入る */
	teDahai = 13 // ツモ切り
	if (haiHand(13, ActivePlayer) == 0) {teDahai = 0} // 鳴いた直後（多牌しないための処理）
	else {
		// 花牌を抜く
		repeat 14
			if (haiHand(cnt, ActivePlayer) > 70) {teDahai = cnt+20: break}
		loop
		if (teDahai >= 20) {
			repeat // 時間いっぱいまで待つ
				await 0
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) >= COMTHINKTIME) {break}
			loop
			return
		}
	}
	if (haiRichi(ActivePlayer)) {
		await 0: gosub *chkankanability // 立直後の暗槓
		if (Ankanabilityflag == 1) {
			repeat // 時間いっぱいまで待つ
				await 0
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) >= COMTHINKTIME) {break}
			loop
			teDahai = 33: return
		}
	}
	await 0: gosub *countseentiles
	targetPlayer = ActivePlayer: await 0: gosub *countshanten
	if (Shanten = -1) {
		TsumoAgari = 1
		targetPlayer = ActivePlayer: await 0: gosub *countyaku
		repeat // 時間いっぱいまで待つ
			await 0
			nowTime = gettime(6)*1000+gettime(7)
			if (nowTime < startTime) {nowTime += 60000}
			if ((nowTime-startTime) >= COMTHINKTIME) {break}
		loop
		if ((haiHan <= hncnShibari)||((strmid(RuleConf, 39, 1) != "0")&&(haiRichi(ActivePlayer) == 0))) {
			// 和了れないならツモ切り
			teDahai = 13: return
		} else {
			// 和了れるなら和了る
			teDahai = -1: return
		}
	} // ツモ和了り
	// 十三不塔なら和了る
	targetPlayer = ActivePlayer: await 0: gosub *chkshisanbuda: gosub *chkshisibuda
	if ((ShisanBuDa == 1)&&(haiDiHeable(ActivePlayer))&&(strmid(RuleConf, 55, 1) != "0")) {teDahai = -1: return}
	if ((ShisiBuDa == 1)&&(haiDiHeable(ActivePlayer))&&(strmid(RuleConf, 73, 1) != "0")) {teDahai = -1: return}
	await 0: gosub *chkdaopaiability
	if (haiDaoPaiable) {
		// 九種九牌
		if (haiDiHeable(ActivePlayer)) {
#ifdef SANMA
			if (((hncnPoint(ActivePlayer) > 400)&&(Shanten > 1))||(Shanten > 2)) {
#else
			if (((hncnPoint(ActivePlayer) > 300)&&(Shanten > 1))||(Shanten > 2)) {
#endif
				// 国士聴牌は流さない
				if (Shanten > 0) {
					teDahai = -2: return // 流局
				}
			}
		}
	}
	// リーチの場合は、和了れなければツモ切り
	if (haiRichi(ActivePlayer)) {teDahai = 13: return}
	targetPlayer = ActivePlayer: await 0: gosub *countshanten
	nowShanten = Shanten
	// リーチをかけるかどうか
	dim haiDiscardability, 14
	dim MinScore, 14
	dim MaxScore, 14
	dim MachihaiTotalTiles, 14
	dim MachihaiFuritenFlag, 14
	if ((nowShanten == 0)&&((haiMenzen(ActivePlayer) == 1)||(strmid(RuleConf, 39, 1) != "0"))) {
		Richiability = 0
		dim tmpHaiHand, 14
		repeat 14
			tmpHaiHand(cnt) = haiHand(cnt, ActivePlayer)
		loop
		repeat 14
			await 0
			if (cnt > 0) {
				if (tmpHaiHand(cnt-1) == tmpHaiHand(cnt)) {
					haiDiscardability(cnt) = haiDiscardability(cnt-1) // 同じ牌を２度調べない
					continue
				}
			}
			targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
			tmpHandTileCode = haiHand(cnt, ActivePlayer)
			// 存在しない牌の場合
			if (haiHand(cnt, ActivePlayer) == 0) {haiDiscardability(cnt) = -9999999: continue}
			// 向聴数から、大まかな評価値を算出
			haiHand(cnt, ActivePlayer) = haiHand(13, ActivePlayer)
			haiHand(13, ActivePlayer) = 0
			targetPlayer = ActivePlayer: await 0: gosub *chkfuriten
			targetPlayer = ActivePlayer: await 0: gosub *countshanten
			haiHand(13, ActivePlayer) = haiHand(cnt, ActivePlayer)
			haiHand(cnt, ActivePlayer) = tmpHandTileCode
			MachihaiTotalTiles(cnt) = haiMachihaiTotal
			// ダブル立直になる時は一部の判定を省略する
			if (haiDiHeable(ActivePlayer) == 0) {
				// 他家のプンリーの当たり牌になっている場合は捨てない(捨ててはならない！)
				if (haiOpenRichiMachihai(tmpHandTileCode) == 1) {haiDiscardability(cnt) = -999999999: continue}
				// 空聴リーチを避ける(錯和ではないが、和了れなくなるため)
				if (haiMachihaiTotal == 0) {haiDiscardability(cnt) = -999999: continue}
				// 振聴リーチを避ける(錯和ではないが、手変わりの可能性を残す)
				if (haiFuriten == 1) {MachihaiFuritenFlag(cnt) = 1: haiDiscardability(cnt) = -99999: continue}
				// 待ち牌が残り２枚以下の場合
				if (haiMachihaiTotal <= 2) {
					haiDiscardability(cnt) = -9999: continue
				}
			}
			// 不聴立直防止用
			if (Shanten > 0) {haiDiscardability(cnt) = -999999: continue}
			Richiability++
			
			tmpTileNum = cnt
			MinScore(cnt) = 999999999
			MaxScore(cnt) = -999999999
			repeat 38
				if (cnt\10 == 0) {continue}
				haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) >= COMTHINKTIME) {
					repeat 14
						if (haiDiscardability(cnt) > 0) {haiDiscardability(cnt) = 0}
					loop
					haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
					haiHand(tmpTileNum, ActivePlayer) = tmpHandTileCode
					break
				}
				await 0: gosub *countshanten
				if (Shanten == -1) {
					// ここで計算されるのはダマ聴で自摸和のときの点数
					TsumoAgari = 1: await 0: gosub *countyaku
					if (haiDiscardability(tmpTileNum) >= 10000) {haiDiscardability(tmpTileNum) -= 10000}
					haiDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
					if (MinScore(tmpTileNum) > (haiHan+haiBonusHan+haiDoraQuantity)) {MinScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity)}
					if (MaxScore(tmpTileNum) < (haiHan+haiBonusHan+haiDoraQuantity)) {MaxScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity)}
				}
				haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
				haiHand(tmpTileNum, ActivePlayer) = tmpHandTileCode
				if (MinScore(tmpTileNum) == 999999999) {MinScore(tmpTileNum) = 0}
				if (MaxScore(tmpTileNum) ==-999999999) {MaxScore(tmpTileNum) = 0}
			loop
		loop
		repeat 14
			haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
		loop
		if ((nowTime-startTime) >= COMTHINKTIME) {
			// 時間がかかる時用の簡略版
			repeat 14
				if (haiDiscardability(cnt) >= 0) {
					tmpTileNum = cnt
					MinScore(tmpTileNum) = 0: MaxScore(tmpTileNum) = 0
					repeat 38
						if (cnt\10 == 0) {continue}
						haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
						haiHand(13, ActivePlayer) = cnt
						await 0: gosub *countshanten
						if (Shanten == -1) {
							haiDiscardability(tmpTileNum) += 2000*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
						}
						haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
						haiHand(tmpTileNum, ActivePlayer) = tmpHandTileCode
					loop
				}
			loop
		}
		repeat 14
			haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
		loop
		if (Richiability > 0) {
			teDahai = 13
			repeat 14
				if (haiDiscardability(teDahai) < haiDiscardability(13-cnt)) {
					teDahai = 13-cnt
				}
			loop
			// 長考してるふり
			repeat
				await 0
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) >= COMTHINKTIME) {break}
			loop
			// 役満聴牌の場合、ダマ聴でも数え役満の場合、
			// 跳満以上が確定の場合は立直しない
			// 但しダブル立直になる時はリーチする
			// 待ちが８枚以上あるならプンリー
			// 振聴立直になるような場合は開き直ってプンリー
			// 誰かが裸単騎しているならプンリー
			// 但しリーチ縛りならリーチする
			if (((MaxScore(teDahai) < 14)&&(MinScore(teDahai) < 7))||(haiDiHeable(ActivePlayer) == 1)) {
				if ((MachihaiTotalTiles(teDahai) >= 8)||(MachihaiFuritenFlag(teDahai) == 1)) {
					if (haiMenzen(ActivePlayer) == 1) {teDahai += 100}
					else {teDahai += 60}
				} else {
					if ((haiNakiMianziDat(0, 0) == 4)||(haiNakiMianziDat(0, 1) == 4)||(haiNakiMianziDat(0, 2) == 4)||(haiNakiMianziDat(0, 3) == 4)) {
						if (MachihaiTotalTiles(teDahai) >= 2) {
							if (haiMenzen(ActivePlayer) == 1) {teDahai += 100}
							else {teDahai += 60}
						}
					} else {
						teDahai += 60
					}
				}
			} else {
				if (strmid(RuleConf, 39, 1) != "0") {
					teDahai += 60
				}
			}
			return
		}
	}
	// チー、ポンした直後に暗槓/加槓できないようにするためのif処理
	// チー、ポンした直後はツモ牌がない状態なので、それを利用する
	// カンをした直後はツモ牌が嶺上牌になっている
	// カンをしたあと、続けて暗槓をすることはかまわない
	if (haiHand(13, ActivePlayer) != 0) {
		await 0
		/* 暗槓するかどうか */
		dim tmpHaiHand, 14
		repeat 14
			tmpHaiHand(cnt) = haiHand(cnt, ActivePlayer)
		loop
		repeat 14
			if (haiHand(cnt, ActivePlayer) == 0) {continue}
			targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
			tmpnum = haiHand(cnt, ActivePlayer)
			repeat 14
				if (haiHand(cnt, ActivePlayer) == tmpnum) {
					haiHand(cnt, ActivePlayer) = 0
				}
			loop
			targetPlayer = ActivePlayer: await 0: gosub *countshanten
			repeat 14
				haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
			loop
			if ((haiTileInfo\128 / 64) == 1) {
				if (Shanten == (nowShanten+2)) {
					// 長考してるふり
					repeat
						await 0
						nowTime = gettime(6)*1000+gettime(7)
						if (nowTime < startTime) {nowTime += 60000}
						if ((nowTime-startTime) >= COMTHINKTIME) {break}
					loop
					teDahai = cnt+20
					break
				}
			}
		loop
		/* 加槓するかどうか */
		repeat 14
			Kakanability = 0
			if (haiHand(cnt, ActivePlayer) == 0) {continue}
			targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
			tmpnum = haiHand(cnt, ActivePlayer)
			haiHand(cnt, ActivePlayer) = 0
			targetPlayer = ActivePlayer: await 0: gosub *countshanten
			repeat 14
				haiHand(cnt, ActivePlayer) = tmpHaiHand(cnt)
			loop
			repeat haiNakiMianziDat(0, ActivePlayer), 1
				if (haiNakiMianziDat(cnt, ActivePlayer)\100 == tmpnum) {
					if ((haiNakiMianziDat(cnt, ActivePlayer)/1000 >= 5)&&(haiNakiMianziDat(cnt, ActivePlayer)/1000 <= 7)) {Kakanability = 1}
				}
			loop
			if (Kakanability == 1) {
				if (Shanten == nowShanten) {
					// 長考してるふり
					repeat
						await 0
						nowTime = gettime(6)*1000+gettime(7)
						if (nowTime < startTime) {nowTime += 60000}
						if ((nowTime-startTime) >= COMTHINKTIME) {break}
					loop
					teDahai = cnt+40
					break
				}
			}
		loop
		if (teDahai >= 20) {return}
	}
	/* それぞれを捨てたときの手の評価を計算する */
	dim haiDiscardability, 14
	startTime = gettime(6)*1000+gettime(7)
	Yishanten = 0
	dim tmpDiscardability, 14

	WanzCount = 0: PinzCount = 0: SouzCount = 0
	YaojiuCount = 0: KeziCount = 0
	repeat 14
		if (haiHand(cnt, ActivePlayer)/10 == 0) {WanzCount++}
		if (haiHand(cnt, ActivePlayer)/10 == 1) {PinzCount++}
		if (haiHand(cnt, ActivePlayer)/10 == 2) {SouzCount++}
		if ((haiHand(cnt, ActivePlayer)/10 == 3)||(haiHand(cnt, ActivePlayer)\10 == 1)||(haiHand(cnt, ActivePlayer)\10 == 9)) {YaojiuCount++}
		targetTile = cnt: await 0: gosub *getpaiinfo
		if (((haiTileInfo \ 2) == 1)||((haiTileInfo \ 8 / 4) == 1)) {KeziCount++}
	loop
	repeat haiNakiMianziDat(0, ActivePlayer), 1
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 0) {WanzCount += 3}
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 1) {PinzCount += 3}
		if (haiNakiMianziDat(cnt, ActivePlayer)\100/10 == 2) {SouzCount += 3}
		if (haiNakiMianziDat(cnt, ActivePlayer)/1000 >= 4) {KeziCount += 3}
	loop
//	if (WanzCount >= 8) {haiAimingYise(ActivePlayer) = 1} // 一色手を狙ってみよう！
//	if (PinzCount >= 8) {haiAimingYise(ActivePlayer) = 2}
//	if (SouzCount >= 8) {haiAimingYise(ActivePlayer) = 3}
//	if (KeziCount >= 6) {
//		haiAimingDuidui(ActivePlayer) = 1 // どうやら対子場のようだ
//	}

	repeat 14
		targetPlayer = ActivePlayer: targetTile = cnt: await 0: gosub *getpaiinfo
		tmpHand = haiHand(cnt, ActivePlayer)
		// 存在しない牌の場合
		if (haiHand(cnt, ActivePlayer) == 0) {haiDiscardability(cnt) = -9999999: continue}
		// 向聴数から、大まかな評価値を算出
		haiHand(cnt, ActivePlayer) = 0
		targetPlayer = ActivePlayer: await 0: gosub *countshanten
		haiDiscardability(cnt) = (6-Shanten)*1000
		haiHand(cnt, ActivePlayer) = tmpHand
		if (haiTileInfo == 0) {
			if (tmpHand >= 30) {
				haiDiscardability(cnt) += 900 // 字牌の浮き牌
				if (tmpHand >= 35) {
					haiDiscardability(cnt) -= 100 // 三元牌のとき
				}
				if (tmpHand == 31) {
					if ((hncnTurn/4 == 0)||((ActivePlayer+32-hncnTurn)\4 == 0)) {
						haiDiscardability(cnt) -= 100 // 東が役牌のとき
					}
				}
				if (tmpHand == 32) {
					if ((hncnTurn/4 == 1)||((ActivePlayer+32-hncnTurn)\4 == 1)) {
						haiDiscardability(cnt) -= 100 // 南が役牌のとき
					}
				}
				if (tmpHand == 33) {
					if ((hncnTurn/4 == 2)||((ActivePlayer+32-hncnTurn)\4 == 2)) {
						haiDiscardability(cnt) -= 100 // 西が役牌のとき
					}
				}
				if (tmpHand == 34) {
					if ((hncnTurn/4 == 3)||((ActivePlayer+32-hncnTurn)\4 == 3)) {
						haiDiscardability(cnt) -= 100 // 北が役牌のとき
					}
				}
			} else {
				haiDiscardability(cnt) += 700 // 数牌の浮き牌
			}
		}
		else {
			if ((haiTileInfo \ 16 / 8) == 1) {haiDiscardability(cnt) += 500} // 辺張
			else {
				if ((haiTileInfo \ 64 / 32) == 1) {haiDiscardability(cnt) += 300} // 嵌張
				else {
					if (haiAimingDuidui(ActivePlayer)) {
						if ((haiTileInfo \ 32 / 16) == 1) {haiDiscardability(cnt) += 100} // 対子場の時は両面
					} else {
						if ((haiTileInfo \ 8 / 4) == 1) {haiDiscardability(cnt) += 100} // そうでなければ対子
					}
				}
			}
		}
		if (haiHandAkadora(cnt, ActivePlayer) >= 0) {
			haiDiscardability(cnt) -= 50
		}
		if ((haiTileInfo \ 2) == 1) {
			// 役牌の暗刻を切らないようにする
			if (tmpHand >= 30) {
				if (tmpHand >= 35) {
					haiDiscardability(cnt) -= 99999 // 三元牌のとき
				}
				if (tmpHand == 31) {
					if ((hncnTurn/4 == 0)||((ActivePlayer+32-hncnTurn)\4 == 0)) {
						haiDiscardability(cnt) -= 99999 // 東が役牌のとき
					}
				}
				if (tmpHand == 32) {
					if ((hncnTurn/4 == 1)||((ActivePlayer+32-hncnTurn)\4 == 1)) {
						haiDiscardability(cnt) -= 99999 // 南が役牌のとき
					}
				}
				if (tmpHand == 33) {
					if ((hncnTurn/4 == 2)||((ActivePlayer+32-hncnTurn)\4 == 2)) {
						haiDiscardability(cnt) -= 99999 // 西が役牌のとき
					}
				}
				if (tmpHand == 34) {
					if ((hncnTurn/4 == 3)||((ActivePlayer+32-hncnTurn)\4 == 3)) {
						haiDiscardability(cnt) -= 99999 // 北が役牌のとき
					}
				}
			}
		}
		haiDiscardability(cnt) += (haiSeenCount(tmpHand)*10) // 場に見えてる個数
		if (tmpHand >= 30) {haiDiscardability(cnt) += 50}
		else {
			if ((tmpHand\10 == 1)||(tmpHand\10 == 9)) {haiDiscardability(cnt) += 40}
			if ((tmpHand\10 == 2)||(tmpHand\10 == 8)) {haiDiscardability(cnt) += 30}
			if ((tmpHand\10 == 3)||(tmpHand\10 == 7)) {haiDiscardability(cnt) += 20}
			if ((tmpHand\10 == 4)||(tmpHand\10 == 6)) {haiDiscardability(cnt) += 10}
		}
		haiHand(cnt, ActivePlayer) = tmpHand
#if 0 /* いろいろあったんで、やめた */
		if (nowShanten >= 2) {
			// 染めてみようかな？？
			if ((WanzCount >= 11)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((PinzCount >= 11)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((SouzCount >= 11)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) {haiDiscardability(cnt) += 10000}
			if ((WanzCount >= 10)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((PinzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((SouzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) {haiDiscardability(cnt) += 10000}
			if ((WanzCount >= 8)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((PinzCount >= 8)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((SouzCount >= 8)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) {haiDiscardability(cnt) += 10000}
		}
		if (nowShanten == 1) {
			if ((WanzCount >= 12)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 20000}
			if ((PinzCount >= 12)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 20000}
			if ((SouzCount >= 12)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) {haiDiscardability(cnt) += 20000}
			if ((WanzCount >= 10)&&((tmpHand/10 == 1)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((PinzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 2))) {haiDiscardability(cnt) += 10000}
			if ((SouzCount >= 10)&&((tmpHand/10 == 0)||(tmpHand/10 == 1))) {haiDiscardability(cnt) += 10000}
		}
		if (nowShanten >= 1) {
			// やっぱタンヤオ狙いでしょっ！
			if ((YaojiuCount <= 3)&&((tmpHand/10 == 3)||(tmpHand\10 == 1)||(tmpHand\10 == 9))) {haiDiscardability(cnt) += 30000}
		}
#endif
		if (nowShanten == 2) { // 二向聴でも先読みさせてみる
			Yishanten = 1
			tmpXTileNum = cnt
			if (tmpXTileNum > 0) {
				if (haiHand(tmpXTileNum, ActivePlayer) == haiHand(tmpXTileNum-1, ActivePlayer)) {
					continue // 同じ牌を２度調べない
				}
			}
			repeat 38
				await 0
				if (cnt\10 == 0) {continue}
				if (haiCount(cnt)+haiSeenCount(cnt) == 4) {continue} // ４枚見えてる
				if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) {continue} // 字牌のラス牌はとりあえず無視
				haiHand(tmpXTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				tmpXde = 4-haiCount(cnt)-haiSeenCount(cnt)
				if (Shanten == 1) { // 次のツモ牌で一向聴の場合
					nowTime = gettime(6)*1000+gettime(7)
					if (nowTime < startTime) {nowTime += 60000}
					if ((nowTime-startTime) < COMTHINKTIME) {
						repeat 14
							nowTime = gettime(6)*1000+gettime(7)
							tmpTileNum = cnt
							tmpTile2a = haiHand(cnt, ActivePlayer)
							if (tmpTileNum > 0) {
								if (haiHand(tmpTileNum, ActivePlayer) == haiHand(tmpTileNum-1, ActivePlayer)) {
									continue // 同じ牌を２度調べない
								}
							}
							repeat 38
								nowTime = gettime(6)*1000+gettime(7)
								await 0
								if (cnt\10 == 0) {continue}
								if (haiCount(cnt)+haiSeenCount(cnt) == 4) {continue} // ４枚見えてる
								if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) {continue} // 字牌のラス牌はとりあえず無視
								haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
								haiHand(13, ActivePlayer) = cnt
								await 0: gosub *countshanten
								tmpde = 4-haiCount(cnt)-haiSeenCount(cnt)
								if (Shanten == 0) { // 次のツモ牌で聴牌の場合
									nowTime = gettime(6)*1000+gettime(7)
									if (nowTime < startTime) {nowTime += 60000}
									if ((nowTime-startTime) < COMTHINKTIME) {
										repeat 14
											nowTime = gettime(6)*1000+gettime(7)
											if (nowTime < startTime) {nowTime += 60000}
											if ((nowTime-startTime) >= COMTHINKTIME) {break}
											tmpTile2 = haiHand(cnt, ActivePlayer)
											tmpTileNum2 = cnt
											repeat 38
												nowTime = gettime(6)*1000+gettime(7)
												await 0
												if (cnt\10 == 0) {continue} // 存在しない牌
												if (haiCount(cnt)+haiSeenCount(cnt) == 4) {continue} // ４枚切れた牌
												tmpHe = 0: tmpHai = cnt
												repeat haiSutehai(0, targetPlayer), 1
													if (haiSutehai(cnt, targetPlayer) == tmpHai) {
														tmpHe = 1: break
													}
												loop
												if (tmpHe == 1) {tmpDiscardability == 0: break} // 振聴になる牌を避ける
												haiHand(tmpTileNum2, ActivePlayer) = haiHand(13, ActivePlayer)
												haiHand(13, ActivePlayer) = cnt
												await 0: gosub *countshanten
												if (Shanten == -1) {
													TsumoAgari = 1: await 0: gosub *countyaku
													tmpDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
												}
												haiHand(13, ActivePlayer) = haiHand(tmpTileNum2, ActivePlayer)
												haiHand(tmpTileNum2, ActivePlayer) = tmpTile2
											loop
										loop
									} else { // 処理が遅い時
										repeat 14
											tmpDiscardability(tmpTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
										loop
									}
								}
								haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
								haiHand(tmpTileNum, ActivePlayer) = tmpTile2a
								nowTime = gettime(6)*1000+gettime(7)
								if (nowTime < startTime) {nowTime += 60000}
								if ((nowTime-startTime) < COMTHINKTIME) {break}
							loop
							nowTime = gettime(6)*1000+gettime(7)
							if (nowTime < startTime) {nowTime += 60000}
							if ((nowTime-startTime) < COMTHINKTIME) {break}
						loop
					} else { // 処理が遅い時
						repeat 14
							tmpDiscardability(tmpXTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpXde
						loop
					}
				}
				haiHand(13, ActivePlayer) = haiHand(tmpXTileNum, ActivePlayer)
				haiHand(tmpXTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) < COMTHINKTIME) {break}
			loop
		}
		if (nowShanten == 1) { // 一向聴のとき
			Yishanten = 1
			tmpTileNum = cnt
			if (tmpTileNum > 0) {
				if (haiHand(tmpTileNum, ActivePlayer) == haiHand(tmpTileNum-1, ActivePlayer)) {
					continue // 同じ牌を２度調べない
				}
			}
			repeat 38
				await 0
				if (cnt\10 == 0) {continue}
				if (haiCount(cnt)+haiSeenCount(cnt) == 4) {continue} // ４枚見えてる
				if ((cnt >= 30)&&(haiCount(cnt)+haiSeenCount(cnt) == 3)) {continue} // 字牌のラス牌はとりあえず無視
				haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				tmpde = 4-haiCount(cnt)-haiSeenCount(cnt)
				if (Shanten == 0) { // 次のツモ牌で聴牌の場合
					nowTime = gettime(6)*1000+gettime(7)
					if (nowTime < startTime) {nowTime += 60000}
					if ((nowTime-startTime) < COMTHINKTIME) {
						repeat 14
							nowTime = gettime(6)*1000+gettime(7)
							if (nowTime < startTime) {nowTime += 60000}
							if ((nowTime-startTime) >= COMTHINKTIME) {break}
							tmpTile2 = haiHand(cnt, ActivePlayer)
							tmpTileNum2 = cnt
							repeat 38
								nowTime = gettime(6)*1000+gettime(7)
								if (nowTime < startTime) {nowTime += 60000}
								if ((nowTime-startTime) >= COMTHINKTIME) {break}
								await 0
								if (cnt\10 == 0) {continue} // 存在しない牌
								if (haiCount(cnt)+haiSeenCount(cnt) == 4) {continue} // ４枚切れた牌
								tmpHe = 0: tmpHai = cnt
								repeat haiSutehai(0, targetPlayer), 1
									if (haiSutehai(cnt, targetPlayer) == tmpHai) {
										tmpHe = 1: break
									}
								loop
								if (tmpHe == 1) {tmpDiscardability == 0: break} // 振聴になる牌を避ける
								haiHand(tmpTileNum2, ActivePlayer) = haiHand(13, ActivePlayer)
								haiHand(13, ActivePlayer) = cnt
								await 0: gosub *countshanten
								if (Shanten == -1) {
									TsumoAgari = 1: await 0: gosub *countyaku
									tmpDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
								}
								haiHand(13, ActivePlayer) = haiHand(tmpTileNum2, ActivePlayer)
								haiHand(tmpTileNum2, ActivePlayer) = tmpTile2
							loop
						loop
					} else { // 処理が遅い時
						repeat 14
							tmpDiscardability(tmpTileNum) = (4-haiCount(cnt)-haiSeenCount(cnt))*tmpde
						loop
					}
				}
				haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
				haiHand(tmpTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) < COMTHINKTIME) {break}
			loop
		}
		if (nowShanten == 0) { // 聴牌したとき
			tmpTileNum = cnt
			repeat 38
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) >= COMTHINKTIME) {break}
				if (cnt\10 == 0) {continue}
				haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
				haiHand(13, ActivePlayer) = cnt
				await 0: gosub *countshanten
				if (Shanten == -1) {
					TsumoAgari = 1: await 0: gosub *countyaku
					if (haiDiscardability(tmpTileNum) >= 10000) {haiDiscardability(tmpTileNum) -= 10000}
					haiDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
				}
				haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
				haiHand(tmpTileNum, ActivePlayer) = tmpHand
				nowTime = gettime(6)*1000+gettime(7)
				if (nowTime < startTime) {nowTime += 60000}
				if ((nowTime-startTime) < COMTHINKTIME) {break}
			loop
		}
	loop
	nowTime = gettime(6)*1000+gettime(7)
	if (nowTime < startTime) {nowTime += 60000}
	if ((Yishanten)&&((nowTime-startTime) < COMTHINKTIME)) {
		// 一向聴のとき
		repeat 14
			haiDiscardability(cnt) += tmpDiscardability(cnt)+100000
		loop
	}
	// オープン立直の待ち牌を捨てない(捨ててはならない！)ようにする
	repeat 14
		if (haiHand(cnt, ActivePlayer) < 40) {
			if (haiOpenRichiMachihai(haiHand(cnt, ActivePlayer)) == 1) {
				haiDiscardability(cnt) = -999999999
			}
		}
	loop
	// 大三元の包になるような牌を捨てないようにする
	await 0: gosub *countseentiles
	if (nowShanten > 1) {
		// ノーチャンスか・振聴となるか・白を鳴いてるか・発を鳴いてるか・中を鳴いてるか
		tmpDiscardabilityChkFlag = 0
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) {continue}
			if (haiNakiMianziDat(0, tmpchkcnt) == 0) {continue}
			repeat haiNakiMianziDat(0, tmpchkcnt)
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 35) {
					tmpDiscardabilityChkFlag += 4
				}
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 36) {
					tmpDiscardabilityChkFlag += 2
				}
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 37) {
					tmpDiscardabilityChkFlag += 1
				}
			loop
			if (haiSutehai(0, tmpchkcnt) == 0) {continue}
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 35)&&(tmpDiscardabilityChkFlag == 3) {tmpDiscardabilityChkFlag += 8}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 36)&&(tmpDiscardabilityChkFlag == 5) {tmpDiscardabilityChkFlag += 8}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 37)&&(tmpDiscardabilityChkFlag == 6) {tmpDiscardabilityChkFlag += 8}
			loop
			if ((haiSeenCount(35) >= 2)&&(tmpDiscardabilityChkFlag == 3)) {tmpDiscardabilityChkFlag += 16}
			if ((haiSeenCount(36) >= 2)&&(tmpDiscardabilityChkFlag == 5)) {tmpDiscardabilityChkFlag += 16}
			if ((haiSeenCount(37) >= 2)&&(tmpDiscardabilityChkFlag == 6)) {tmpDiscardabilityChkFlag += 16}
		loop
		repeat 14
			if ((tmpDiscardabilityChkFlag == 3)&&(haiHand(cnt, ActivePlayer) == 35)) {
				haiDiscardability(cnt) = -799999999
			}
			if ((tmpDiscardabilityChkFlag == 5)&&(haiHand(cnt, ActivePlayer) == 36)) {
				haiDiscardability(cnt) = -799999999
			}
			if ((tmpDiscardabilityChkFlag == 6)&&(haiHand(cnt, ActivePlayer) == 37)) {
				haiDiscardability(cnt) = -799999999
			}
		loop
	}
	// 大四喜の包になるような牌を捨てないようにする
	if (nowShanten > 1) {
		// ノーチャンスか・振聴となるか・東を鳴いてるか
		// ・南を鳴いてるか・西を鳴いてるか・北を鳴いてるか
		tmpDiscardabilityChkFlag = 0
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) {continue}
			if (haiNakiMianziDat(0, tmpchkcnt) == 0) {continue}
			repeat haiNakiMianziDat(0, tmpchkcnt)
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 31) {
					tmpDiscardabilityChkFlag += 8
				}
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 32) {
					tmpDiscardabilityChkFlag += 4
				}
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 33) {
					tmpDiscardabilityChkFlag += 2
				}
				if ((haiNakiMianziDat(cnt, tmpchkcnt) \ 100) == 34) {
					tmpDiscardabilityChkFlag += 1
				}
			loop
			if (haiSutehai(0, tmpchkcnt) == 0) {continue}
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 31)&&(tmpDiscardabilityChkFlag == 7) {tmpDiscardabilityChkFlag += 16}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 32)&&(tmpDiscardabilityChkFlag == 11) {tmpDiscardabilityChkFlag += 16}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 33)&&(tmpDiscardabilityChkFlag == 13) {tmpDiscardabilityChkFlag += 16}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100) == 34)&&(tmpDiscardabilityChkFlag == 14) {tmpDiscardabilityChkFlag += 16}
			loop
			if ((haiSeenCount(31) >= 3)&&(tmpDiscardabilityChkFlag == 7)) {tmpDiscardabilityChkFlag += 32}
			if ((haiSeenCount(32) >= 3)&&(tmpDiscardabilityChkFlag == 11)) {tmpDiscardabilityChkFlag += 32}
			if ((haiSeenCount(33) >= 3)&&(tmpDiscardabilityChkFlag == 13)) {tmpDiscardabilityChkFlag += 32}
			if ((haiSeenCount(34) >= 3)&&(tmpDiscardabilityChkFlag == 14)) {tmpDiscardabilityChkFlag += 32}
		loop
		repeat 14
			if ((tmpDiscardabilityChkFlag == 7)&&(haiHand(cnt, ActivePlayer) == 31)) {
				haiDiscardability(cnt) = -799999999
			}
			if ((tmpDiscardabilityChkFlag == 11)&&(haiHand(cnt, ActivePlayer) == 32)) {
				haiDiscardability(cnt) = -799999999
			}
			if ((tmpDiscardabilityChkFlag == 13)&&(haiHand(cnt, ActivePlayer) == 33)) {
				haiDiscardability(cnt) = -799999999
			}
			if ((tmpDiscardabilityChkFlag == 14)&&(haiHand(cnt, ActivePlayer) == 34)) {
				haiDiscardability(cnt) = -799999999
			}
		loop
	}
	// 一色手を警戒する
	if (nowShanten > 1) {
		repeat 4: tmpchkcnt = 0
			if (tmpchkcnt == hncnZijia) {continue}
			tmpDiscardabilityChkCount1 = 0
			tmpDiscardabilityChkCount2 = 0
			tmpDiscardabilityChkCount3 = 0
			if (haiSutehai(0, tmpchkcnt) == 0) {continue}
			repeat haiSutehai(0, tmpchkcnt), 1
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 0) {tmpDiscardabilityChkCount1++}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 1) {tmpDiscardabilityChkCount2++}
				if ((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) == 2) {tmpDiscardabilityChkCount3++}
			loop
			repeat 14
				if ((tmpDiscardabilityChkCount2 > 0)&&(tmpDiscardabilityChkCount3 > 0)) {
					if (((tmpDiscardabilityChkCount1*1000)/(tmpDiscardabilityChkCount2+tmpDiscardabilityChkCount3)) < 200) {
						if ((haiHand(cnt, ActivePlayer) /10) == 0) {
							haiDiscardability(cnt) = 0
						}
					}
				}
				if ((tmpDiscardabilityChkCount1 > 0)&&(tmpDiscardabilityChkCount3 > 0)) {
					if (((tmpDiscardabilityChkCount2*1000)/(tmpDiscardabilityChkCount1+tmpDiscardabilityChkCount3)) < 200) {
						if ((haiHand(cnt, ActivePlayer) /10) == 1) {
							haiDiscardability(cnt) = 0
						}
					}
				}
				if ((tmpDiscardabilityChkCount1 > 0)&&(tmpDiscardabilityChkCount2 > 0)) {
					if (((tmpDiscardabilityChkCount3*1000)/(tmpDiscardabilityChkCount1+tmpDiscardabilityChkCount2)) < 200) {
						if ((haiHand(cnt, ActivePlayer) /10) == 2) {
							haiDiscardability(cnt) = 0
						}
					}
				}
			loop
		loop
	}
	// 国士無双を警戒する
	if (nowShanten > 1) {
		repeat 4: tmpchkcnt = 0
			tmpDiscardabilityChkCount1 = 0
			tmpDiscardabilityChkCount2 = 0
			if (tmpchkcnt == hncnZijia) {continue}
			if (haiSutehai(0, tmpchkcnt) == 0) {continue}
			repeat haiSutehai(0, tmpchkcnt), 1
				if (((haiSutehai(cnt, tmpchkcnt) \ 100 / 10) < 3)&&((haiSutehai(cnt, tmpchkcnt) \ 10) > 1)&&((haiSutehai(cnt, tmpchkcnt) \ 10) < 9)) {tmpDiscardabilityChkCount1++}
				tmpDiscardabilityChkCount2++
			loop
			repeat 14
				if (tmpDiscardabilityChkCount2 > 0) {
					if (((tmpDiscardabilityChkCount1*1000)/(tmpDiscardabilityChkCount2)) > 840) {
						if (((haiHand(cnt, ActivePlayer) \10) == 1)||((haiHand(cnt, ActivePlayer) \10) == 9)||(haiHand(cnt, ActivePlayer) > 30)) {
							haiDiscardability(cnt) = 0
						}
					}
				}
			loop
		loop
	}
	// 警戒するプレーヤーを決定する
	dim haiMarkingPlayer, 4
	repeat 4
		if (cnt == ActivePlayer) {continue /* 自分自身の場合は飛ばす */}
		if (haiNakiMianziDat(0, cnt) >= 2) {
			haiMarkingPlayer(cnt) = 1 /* 二面子以上鳴いていたら降りる */
		}
		if (haiRichi(cnt) > 0) {
			haiMarkingPlayer(cnt) = 1 /* 誰かが立直していたら降りる！ */
		}
		if (haiNakiMianziDat(0, cnt) == 1) {
			if ((haiRinshanPointer - (haiDeadTiles-1) - haiPointer) < 30) {
				/* 残り３０枚未満で１副露の者がいた場合は降りる */
				haiMarkingPlayer(cnt) = 1
			}
		}
	loop
	// オリるかどうかの判定…和了り手の得点を調べる
	TmpMinScore = 999999999
	TmpMaxScore = -999999999
	repeat 14
		if (haiHand(cnt, ActivePlayer) == 0) {continue}
		tmpTileCodeNum = haiHand(cnt, ActivePlayer)
		tmpTileNum = cnt
		MinScore(cnt) = 999999999
		MaxScore(cnt) = -999999999
		repeat 38
			if (cnt\10 == 0) {continue}
			haiHand(tmpTileNum, ActivePlayer) = haiHand(13, ActivePlayer)
			haiHand(13, ActivePlayer) = cnt
			await 0: gosub *countshanten
			if (Shanten == -1) {
				// ここで計算されるのはダマ聴で自摸和のときの点数
				TsumoAgari = 1: await 0: gosub *countyaku
				if (haiDiscardability(tmpTileNum) >= 10000) {haiDiscardability(tmpTileNum) -= 10000}
				haiDiscardability(tmpTileNum) += haiScore*(4-haiCount(cnt)-haiSeenCount(cnt))+100000
				if (MinScore(tmpTileNum) > (haiHan+haiBonusHan+haiDoraQuantity)) {MinScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity)}
				if (MaxScore(tmpTileNum) < (haiHan+haiBonusHan+haiDoraQuantity)) {MaxScore(tmpTileNum) = (haiHan+haiBonusHan+haiDoraQuantity)}
			}
			haiHand(13, ActivePlayer) = haiHand(tmpTileNum, ActivePlayer)
			haiHand(tmpTileNum, ActivePlayer) = tmpHandTileCode
			if (MinScore(tmpTileNum) == 999999999) {MinScore(tmpTileNum) = 0}
			if (MaxScore(tmpTileNum) ==-999999999) {MaxScore(tmpTileNum) = 0}
		loop
		haiHand(cnt, ActivePlayer) = tmpTileCodeNum
		if (MinScore(tmpTileNum) < TmpMinScore) {TmpMinScore = MinScore(tmpTileNum)}
		if (MaxScore(tmpTileNum) > TmpMinScore) {TmpMaxScore = MaxScore(tmpTileNum)}
	loop
	if (TmpMaxScore >= 3000) {
		// とりあえず、跳満以上聴牌なら全ツッパする
		repeat 4: haiMarkingPlayer(cnt) = 0: loop
	}
	// 降り打ちモードのための処理
	repeat 14: tmphaiindex = cnt
		repeat 4: tmpchkcnt = cnt
			if (haiMarkingPlayer(tmpchkcnt) == 0) {break}
			tmpflag = 0
			if (tmpflag == 0) {
				// 合わせ打ちができるなら合わせ打ち
				if ((haiSutehai(haiSutehai(0, (ActivePlayer+3)\4), (ActivePlayer+3)\4) \ 100) == haiHand(tmphaiindex, ActivePlayer)) {
					haiDiscardability(tmphaiindex) += 7000000
					tmpflag = 1
				}
			}
			repeat haiSutehai(0, tmpchkcnt), 1
				if (haiHand(tmphaiindex, ActivePlayer) == (haiSutehai(cnt, tmpchkcnt) \ 100)) {
					// 警戒しているプレーヤーの現物があれば優先的にそれを捨てる
					if (haiHand(tmphaiindex, ActivePlayer) < 30) {
						haiDiscardability(tmphaiindex) += 10000000
					} else {
						haiDiscardability(tmphaiindex) += 5000000
					}
					tmpflag = 1
					break
				}
			loop
			if (tmpflag == 0) {
				repeat haiSutehai(0, tmpchkcnt), 1
					if ((haiSutehai(cnt, tmpchkcnt) \ 100) < 30) {
						// 合わせ打ちも無理なら筋牌
						if (((haiSutehai(cnt, tmpchkcnt) \ 10)\3) == ((haiHand(tmphaiindex, ActivePlayer) \ 10)\3)) {
							haiDiscardability(tmphaiindex) += 1200000
							break
						}
					}
				loop
			}
		loop
	loop
	// 捨牌を決定する
	repeat 14
		if (haiDiscardability(teDahai) < haiDiscardability(13-cnt)) {
			teDahai = 13-cnt
		}
	loop
#ifdef SANMAX
	// 決定された捨牌が北だった場合、捨てる代わりに抜く
	if ((strmid(RuleConf, 30, 1) != "0")&&(haiHand(13, ActivePlayer) != 0)) {
		if (haiHand((teDahai\20), ActivePlayer) == 34) {
			teDahai += 80
		}
	}
#endif
	// 長考してるふり
	repeat
		await 0
		nowTime = gettime(6)*1000+gettime(7)
		if (nowTime < startTime) {nowTime += 60000}
		if ((nowTime-startTime) >= COMTHINKTIME) {break}
	loop
return

/* ＡＩの鳴き・栄和 */
*compnaki
	/*haiHand(13, PassivePlayer) = 0: haiHandAkaDora(13, PassivePlayer) = 0: return*/ /* 何もせず戻る(デバッグ用) */
	if (haiCurrentSutehai > 70) {
		/* 花牌の場合は戻る */
		haiHand(13, PassivePlayer) = 0
		haiHandAkaDora(13, PassivePlayer) = 0
		return
	}

	haiHand(13, PassivePlayer) = 0
	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	currentShanten = Shanten
	haiHand(13, PassivePlayer) = haiCurrentSutehai

	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	TsumoAgari = 0: targetPlayer = PassivePlayer: await 0: gosub *countyaku
	targetPlayer = PassivePlayer: await 0: gosub *chkFuriten
	// 暗槓に対する搶槓の判定で、国士聴牌でない場合は戻る
	if ((ShisanyaoShanten != 0)&&(ChanKanFlag == 2)) {
		haiHand(13, PassivePlayer) = 0
		haiHandAkaDora(13, PassivePlayer) = 0
		return
	}
	haiHand(13, PassivePlayer) = haiCurrentSutehai
	targetPlayer = PassivePlayer: await 0: gosub *countshanten
	if (Shanten == -1) {
		haiRon(PassivePlayer) = 1 // 出和了り
		haiHand(13, PassivePlayer) = haiCurrentSutehai
		haiHandAkaDora(13, PassivePlayer) = haiCurrentSutehaiAkaDora
		if ((haiHan <= hncnShibari)||(haiFuriten == 1)||(haiDoujunFuriten(PassivePlayer))||((strmid(RuleConf, 39, 1) != "0")&&(haiRichi(PassivePlayer) == 0))) {
			haiRon(PassivePlayer) = 0
			haiHand(13, PassivePlayer) = 0
			haiHandAkaDora(13, PassivePlayer) = 0
		}
		return
	}
	haiHand(13, PassivePlayer) = 0
	haiHandAkaDora(13, PassivePlayer) = 0
	if (ChanKanFlag == 1) {return} // 搶槓の判定中なら判定打ち切り
	if (haiPointer >= (haiRinshanPointer - (haiDeadTiles-1))) {return} // 河底牌なら判定打ち切り
	if (haiRichi(PassivePlayer) > 0) {return} // リーチしているなら判定打ち切り
	await 0: gosub *countseentiles
	dim haiCount, 80 // 計算する牌
	repeat 14
		if (haiHand(cnt, PassivePlayer) != 0) {
			// 手牌の中の、それぞれの牌の枚数を数える
			haiCount(haiHand(cnt, PassivePlayer))++
		}
	loop
	// 捨てられた牌が役牌だった場合
	YakuhaiPon = 0
	if (haiCurrentSutehai == 35) {YakuhaiPon = 1}
	if (haiCurrentSutehai == 36) {YakuhaiPon = 1}
	if (haiCurrentSutehai == 37) {YakuhaiPon = 1}
	if ((haiCurrentSutehai == 31)&&((hncnTurn/4 == 0)||((targetPlayer+32-hncnTurn)\4 == 0))) {YakuhaiPon = 1}
	if ((haiCurrentSutehai == 32)&&((hncnTurn/4 == 1)||((targetPlayer+32-hncnTurn)\4 == 1))) {YakuhaiPon = 1}
	if ((haiCurrentSutehai == 33)&&((hncnTurn/4 == 2)||((targetPlayer+32-hncnTurn)\4 == 2))) {YakuhaiPon = 1}
	if ((haiCurrentSutehai == 34)&&((hncnTurn/4 == 3)||((targetPlayer+32-hncnTurn)\4 == 3))) {YakuhaiPon = 1}
	// 役牌を鳴いたらフラグを立てる
	// ポンが失敗する場合は他家のロンがあるか河底牌の場合
	// なのでこの時点でフラグを立てても特に問題はない
	if (YakuhaiPon) {haiYakuhaiNaki(PassivePlayer) = 1}
	await 0: gosub *countseentiles
	if (haiCount(haiCurrentSutehai) >= 3) {
		// すでに槓子を２つ持っているなら大明槓させる
		if (haiNumOfKangs(PassivePlayer) >= 2) {
			haiMinkan(PassivePlayer) = 1
			return
		}
	}
	if (currentShanten > 0) { // すでにテンパってたら鳴かない
		if (currentShanten > Shanten) { // 有効牌でなければ鳴かない
			if (haiCount(haiCurrentSutehai) >= 2) { // ポンできるかどうかの判定
				if ((haiMenzen(PassivePlayer) == 0)||(YakuhaiPon == 1)) {
					// すでに鳴いているか、役牌の場合はポン 対々狙いか一色狙いなら二鳴き
					haiPon(PassivePlayer) = 1
					return
				} else {
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
						if (haiNakiMianziDat(0, PassivePlayer) == 0) {break}
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 100) > 30) {
							tmpYaojiuTilesCount++: continue
						}
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 1) {
							tmpYaojiuTilesCount++: continue
						}
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 9) {
							tmpYaojiuTilesCount++: continue
						}
						if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 7) {
							if ((haiNakiMianziDat(cnt, PassivePlayer) / 1000) < 4) {
								tmpYaojiuTilesCount++: continue
							}
						}
					loop
					if (tmpYaojiuTilesCount == 0) {
						if ((currentShanten == 1)&&(strmid(RuleConf, 19, 1) == "0")) {
							// 喰い断をポン聴する
							haiPon(PassivePlayer) = 1
							return
						}
					}
				}
			}
		}
	}
#ifndef SANMAX
	// チーするかどうかを判定する
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
		if ((haiCurrentSutehai-chiType) < 0) {
			continue // バグ防止用
		}
		repeat 14
			await 0
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+1-chiType)&&(nakiCount1 < 1)) {
				haiHand(cnt, PassivePlayer) = 0
				nakiCount1++
			}
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+2-chiType)&&(nakiCount2 < 1)) {
				haiHand(cnt, PassivePlayer) = 0
				nakiCount2++
			}
			if ((haiHand(cnt, PassivePlayer) == haiCurrentSutehai+3-chiType)&&(nakiCount3 < 1)) {
				haiHand(cnt, PassivePlayer) = 0
				nakiCount3++
			}
		loop
		targetPlayer = PassivePlayer: await 0: gosub *countshanten
		repeat 14
			haiHand(cnt, PassivePlayer) = tmpHaiHand(cnt)
		loop
		if (haiCurrentSutehai >= 30) {
			continue // 字牌に順子なし
		}
		if ((nakiCount1 == 0)||(nakiCount2 == 0)||(nakiCount3 == 0)) {
			continue // 不可能な鳴きの場合
		}
		if (currentShanten > (Shanten - 2)) {chosenNaki == cnt}
	loop
	Shanten = tmp2Shanten
	haiHand(13, PassivePlayer) = 0
	if (currentShanten > 0) { // すでにテンパってたら鳴かない
//		if ((YakuhaiPon == 1)||(((haiCurrentSutehai/10) == (haiAimingYise(PassivePlayer)-1)))) {
		if (YakuhaiPon == 1) {
			// すでに役牌を鳴いているならチー
			haiChi(PassivePlayer) = chosenNaki
		} else {
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
				if (haiNakiMianziDat(0, PassivePlayer) == 0) {break}
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 100) > 30) {
					tmpYaojiuTilesCount++: continue
				}
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 1) {
					tmpYaojiuTilesCount++: continue
				}
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 9) {
					tmpYaojiuTilesCount++: continue
				}
				if ((haiNakiMianziDat(cnt, PassivePlayer) \ 10) == 7) {
					if ((haiNakiMianziDat(cnt, PassivePlayer) / 1000) < 4) {
						tmpYaojiuTilesCount++: continue
					}
				}
			loop
			if (tmpYaojiuTilesCount == 0) {
				if ((currentShanten == 1)&&(strmid(RuleConf, 19, 1) == "0")) {
					// 喰い断を吃聴する
					haiChi(PassivePlayer) = chosenNaki
				}
			}
		}
	}
#endif
return
]=======]
