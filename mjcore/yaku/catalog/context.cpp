#include "../catalog.h"

#include "../../func.h"

inline bool isFinalRound(const GameTable* const gameStat) { // オーラスである？
	return (gameStat->LoopRound * roundLoopRate() +
		gameStat->GameRound) >= gameStat->GameLength;
}

void yaku::yakuCalculator::YakuCatalog::catalogInit::yakulst_contextual() {
#ifndef GUOBIAO
	/* リーチ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("立直"), 1_hanM,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->PlayerStat->RichiFlag.RichiFlag)); // 立直している
		}
	));
	/* ダブリー */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("ダブル立直"), 2_hanM,
		_T("立直"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
				(analysis->PlayerStat->RichiFlag.DoubleFlag)); // ダブル立直フラグが立っている
		}
	));
	if (RuleData::chkRuleApplied("open_riichi")) {
		/* プンリー */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("オープン立直"), 2_hanM,
			_T("立直"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* ダブプン */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ダブル開立直"), 3_hanM,
			_T("立直"), _T("ダブル立直"), _T("オープン立直"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // ダブル立直フラグが立っている
					(analysis->PlayerStat->RichiFlag.OpenFlag)); // オープン立直フラグが立っている
			}
		));
		/* プンリー放銃 */
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("開立直栄和"), 11_hanM,
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.OpenFlag) && // オープン立直フラグが立っている
					(!*analysis->TsumoAgariFlag) && // ロン
					(analysis->GameStat->CurrentPlayer.Furikomi >= 0) &&
					(!analysis->GameStat->Player[analysis->GameStat->CurrentPlayer.Furikomi].RichiFlag.RichiFlag)); // 非リーチ者からの和了
			}
		));
	}
	/* 一発(方言では即ともいう) */
	if (RuleData::chkRuleApplied("riichi_ippatsu")&&(!RuleData::chkRule("riichi_ippatsu", "chip_only"))) { // 一発が役にならないルールを除外
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一発"), RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
			1_hanMD : /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
			1_hanM, // リー即のみでも和了って良いルール
			/* 必ず立直と複合する */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.IppatsuFlag)); // 一発フラグが立っている
			}
		));
		/* 超一発(リーチ一発を下家から和了る) */
		if (RuleData::chkRuleApplied("chouippatsu"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("超一発"), RuleData::chkRule("riichi_ippatsu", "yes_but_unbound") ?
				2_hanMD : /* 一発は縛りを満たさない(リャンシバでリー即のみの和了を認めない)ルール */
				2_hanM, // リー即のみでも和了って良いルール
				_T("一発"), /* 必ず立直と複合する */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // 一発フラグが立っている
						(!*analysis->TsumoAgariFlag) && // ロンである
						(
							(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, SeatRelative::right)) || // 下家から和了った
							((analysis->GameStat->chkGameType(GameTypeID::sanmaT)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == 0) && (analysis->player == 2)) || // 三麻の特例
							((analysis->GameStat->chkGameType(GameTypeID::sanma4)) &&
								(analysis->GameStat->CurrentPlayer.Furikomi == RelativePositionOf(analysis->player, SeatRelative::opposite)) &&
								(analysis->GameStat->playerwind(analysis->player) == SeatAbsolute::west)) // 四人三麻の特例
						));
				}
			));
		/* 南単 */
		if (RuleData::chkRuleApplied("minamityan"))
			yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
				_T("南単"), get_yaku_han("minamityan"),
				/* 必ず立直、一発、ツモと複合する */
				[](const MENTSU_ANALYSIS* const analysis) -> bool {
					return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
						(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
						(analysis->PlayerStat->RichiFlag.IppatsuFlag) && // 一発フラグが立っている
						(*analysis->TsumoAgariFlag) && // ツモである
						(analysis->TsumoHai->tile == SouthWind)); // 南で和了
				}
			));
	}
	/* 鳥リーチ */
	if (RuleData::chkRuleApplied("bird_riichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("鳥リーチ"), get_yaku_han("bird_riichi"),
			/* 立直は複合するらしい */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				if ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag)) // 立直している
				{
					for (int i = 1; i < analysis->PlayerStat->DiscardPointer; i++)
						if ((analysis->PlayerStat->Discard[i].tcode.tile == BambooOne) &&
							((analysis->PlayerStat->Discard[i].dstat == discardRiichi) ||
							(analysis->PlayerStat->Discard[i].dstat == discardRiichiTaken)))
							return true;
					return false;
				}
				else return false;
			}
		));
	/* リリーホワイト */
	if (RuleData::chkRuleApplied("lily_white"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("リリーホワイト"), 1_hanM,
			/* ダブル立直に追加する役 */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->PlayerStat->RichiFlag.DoubleFlag) && // ダブル立直フラグが立っている
					(analysis->TsumoHai->tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 北四巡(立直自摸) */
	if (RuleData::chkRuleApplied("pei4jun_riichi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("北四巡"), get_yaku_han("pei4jun_riichi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->RichiFlag.RichiFlag) && // 立直している
					(analysis->TsumoHai->tile == NorthWind) && // 和了牌が北
					(analysis->GameStat->TurnRound == 4)); // 4巡目である
			}
		));

	// ---------------------------------------------------------------------

	/* 天和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("天和"), get_yaku_han("tenhoh"), // 普通に役満になるか
		_T("門前清自摸和"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->playerwind(analysis->player) == SeatAbsolute::east) && // 親である(東家として決め打ち……)
				(analysis->PlayerStat->FirstDrawFlag)); // 天和・地和フラグが立っている
		}
	));
	/* 地和 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("地和"), 1_yakumanM,
		_T("門前清自摸和"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->playerwind(analysis->player) != SeatAbsolute::east) && // 親である(東家として決め打ち……)
				(analysis->PlayerStat->FirstDrawFlag) && // 天和・地和フラグが立っている
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* ２位じゃ駄目なんですか？ */
	if (RuleData::chkRuleApplied("renhoh")) {
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("人和"), get_yaku_han("renhoh"), _T("門前清自摸和"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->playerwind(analysis->player) != SeatAbsolute::east) && // 親である(東家として決め打ち……)
					(analysis->PlayerStat->FirstDrawFlag) && // 天和・地和フラグが立っている
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
	}
#endif /* GUOBIAO */
#ifdef GUOBIAO
	/* 不求人 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("不求人"), 4_fenF,
		_T("自摸"), _T("門前清"),
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 和了になっている
				(*analysis->MenzenFlag) && // 門前である
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
	/* 門前清 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("門前清"), 2_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 和了になっている
				(*analysis->MenzenFlag)); // 門前である
		}
	));
	/* 自摸 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("自摸"), 1_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 和了になっている
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
#else /* GUOBIAO */
	/* ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("門前清自摸和"), 1_hanM,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(*analysis->MenzenFlag) && // 門前である
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 三隻転覆 */
	if (RuleData::chkRuleApplied("sanseki_tempuku"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("三隻転覆"), get_yaku_han("sanseki_tempuku"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // 自分はリーチしていない
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::right)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::opposite)].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::left)].RichiFlag.RichiFlag) // 他家全員立直
					);
			}
		));
	/* 起死回生 */
	if (RuleData::chkRuleApplied("kishi_kaisei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("起死回生"), get_yaku_han("kishi_kaisei"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(!analysis->PlayerStat->RichiFlag.RichiFlag) && // 自分はリーチしていない
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::right)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::opposite)].RichiFlag.DoubleFlag) &&
					(analysis->GameStat->Player[RelativePositionOf(analysis->player, SeatRelative::left)].RichiFlag.DoubleFlag) // 他家全員ダブリー(!)
					);
			}
		));

	// ---------------------------------------------------------------------

	/* 不死鳥 */
	if (RuleData::chkRuleApplied("phoenix"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("不死鳥"), get_yaku_han("phoenix"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->YakitoriFlag) && // 焼き鳥である
					(isFinalRound(analysis->GameStat))); // オーラスである
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* リンシャンツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("杠上開花"), 8_fenF,
		_T("自摸"),
#else /* GUOBIAO */
		_T("嶺上開花"), 1_hanF,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
				(*analysis->TsumoAgariFlag)); // ツモアガリ
		}
	));
#ifndef GUOBIAO
	/* 明槓開花 */
	if (RuleData::chkRule("minkan_pao", "no_but_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han") ||
		RuleData::chkRule("minkan_pao", "yes_2han_andalso_contiguous_kong"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("明槓開花"), 2_hanF,
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->PaoFlag[PaoYakuPage::minkan].paoPlayer >= 0)); // 明槓の直後
			}
		));
	/* 連槓開花 */
	if (RuleData::chkRuleApplied("renkan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("連槓開花"), get_yaku_han("renkan_kaihoh"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->KangFlag.chainFlag >= 2)); // 連続で槓した直後
			}
		));
	/* 五筒開花 */
	if (RuleData::chkRuleApplied("uupin_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("五筒開花"), get_yaku_han("uupin_kaihoh"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->TsumoHai->tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 中上開花 */
	if (RuleData::chkRuleApplied("chunshan_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("中上開花"), get_yaku_han("chunshan_kaihoh"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->TsumoHai->tile == RedDragon)); // 和了牌が中
			}
		));
	/* 頭槓和 */
	if (RuleData::chkRuleApplied("toukanhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("頭槓和"), get_yaku_han("toukanhoh"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->KangFlag.topFlag)); // 頭槓和フラグが立っている
			}
		));
	/* サヨナラホームラン */
	if (RuleData::chkRuleApplied("sayonara_homerun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("サヨナラホームラン"), get_yaku_han("sayonara_homerun"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0)); // 王牌を除いた残り山牌が0
			}
		));
	/* 深上開花 */
	if (RuleData::chkRuleApplied("shenshang_kaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("深上開花"), get_yaku_han("shenshang_kaihoh"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(isFinalRound(analysis->GameStat))); // オーラスである
			}
		));
	/* 東花園 */
	if (RuleData::chkRuleApplied("higashi_hanazono_triplets"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("東花園"), get_yaku_han("higashi_hanazono_triplets"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->GameRound / 4 == 0) && // 東場
					(analysis->GameStat->playerwind(analysis->player) == SeatAbsolute::east) && // 東家
					(analysis->KangziCount[EastWind] >= 1)); // 東の槓子がある
			}
		));
	/* 花見 */
	if (RuleData::chkRuleApplied("hanami"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("花見"), get_yaku_han("hanami"),
			_T("嶺上開花"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後である
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					((analysis->TsumoHai->tile == CharacterThree) || // 和了牌が三萬か
					(analysis->TsumoHai->tile == CircleThree) || // 三筒か
					(analysis->TsumoHai->tile == BambooThree)) && // 三索
					(analysis->KangziCount[CircleFive] >= 1)); // 五筒を槓している
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 海底ツモ */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("妙手回春"), 8_fenF,
		_T("自摸"),
#else /* GUOBIAO */
		_T("海底摸月"), 1_hanF,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
#ifndef GUOBIAO
				(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
#endif /* GUOBIAO */
				(*analysis->TsumoAgariFlag) && // ツモアガリ
				(analysis->GameStat->tilesLeft() == 0)); // ハイテイである
		}
	));
#ifndef GUOBIAO
	/* 一筒摸月 */
	if (RuleData::chkRuleApplied("iipin_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("一筒摸月"), get_yaku_han("iipin_moyue"),
			_T("海底摸月"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleOne)); // 和了牌が一筒
			}
		));
	/* 海底撈針 */
	if (RuleData::chkRuleApplied("haitei_raotsen"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("海底撈針"), get_yaku_han("haitei_raotsen"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CharacterOne)); // 和了牌が一萬
			}
		));
	/* 海底撈花 */
	if (RuleData::chkRuleApplied("haitei_raohwa"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("海底撈花"), get_yaku_han("haitei_raohwa"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleFive)); // 和了牌が五筒
			}
		));
	/* 発底摸月 */
	if (RuleData::chkRuleApplied("hatsutei_moyue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("発底摸月"), get_yaku_han("hatsutei_moyue"),
			_T("海底摸月"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == GreenDragon)); // 和了牌が發
			}
		));
	/* 深海底 */
	if (RuleData::chkRuleApplied("shen_haitei"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("深海底"), get_yaku_han("shen_haitei"),
			_T("海底摸月"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(*analysis->TsumoAgariFlag) && // ツモアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(isFinalRound(analysis->GameStat))); // オーラスである
			}
		));
#endif /* GUOBIAO */
	/* 河底ロン */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("海底撈月"), 8_fenF,
#else /* GUOBIAO */
		_T("河底撈魚"), 1_hanF,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
				(!*analysis->TsumoAgariFlag) && // ロンアガリ
				(analysis->GameStat->tilesLeft() == 0)); // ハイテイである
		}
	));
#ifndef GUOBIAO
	/* 九筒撈魚 */
	if (RuleData::chkRuleApplied("chuupin_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("九筒撈魚"), get_yaku_han("chuupin_raoyui"),
			_T("河底撈魚"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == CircleNine)); // 和了牌が九筒
			}
		));
	/* 白底撈魚 */
	if (RuleData::chkRuleApplied("hakutei_raoyui"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("白底撈魚"), get_yaku_han("hakutei_raoyui"),
			_T("河底撈魚"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 深淵和 */
	if (RuleData::chkRuleApplied("shenyuanhu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("深淵和"), get_yaku_han("shenyuanhu"),
			_T("河底撈魚"), 
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(isFinalRound(analysis->GameStat))); // オーラスである
			}
		));
	/* 泥底撈蟲 */
	if (RuleData::chkRuleApplied("nitei_raochun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("泥底撈蟲"), get_yaku_han("nitei_raochun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					((analysis->TotalAnShunzi + analysis->TotalAnKezi - analysis->TotalAnKangzi) == 0)); // 裸単騎である
			}
		));
	/* ラストオーダー麻婆豆腐 */
	if (RuleData::chkRuleApplied("maaboodoufu"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("ラストオーダー麻婆豆腐"), get_yaku_han("maaboodoufu"),
			_T("白底撈魚"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->TsumoHai->tile == WhiteDragon)); // 和了牌が白
			}
		));
	/* 石の上にも三年 */
	if (RuleData::chkRuleApplied("three_years_on_stone"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("石の上にも三年"), get_yaku_han("three_years_on_stone"),
			_T("ダブル立直"), _T("ダブル開立直"), _T("海底摸月"), _T("河底撈魚"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->KangFlag.kangFlag) && // 槓をした直後ではない
					(analysis->GameStat->tilesLeft() == 0) && // ハイテイである
					(analysis->PlayerStat->RichiFlag.DoubleFlag)); // ダブル立直している
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 槓振り */
	if (RuleData::chkRuleApplied("kamburi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("槓振り"), get_yaku_han("kamburi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chainFlag >= 1) && // 槓をした直後である
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
#endif /* GUOBIAO */
	/* 槍槓は正確には木偏ではなく手偏 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
#ifdef GUOBIAO
		_T("搶杠和"), 8_fenF,
		_T("和絶張"),
#else /* GUOBIAO */
		_T("搶槓"), 1_hanF,
		_T("欠牌和"), _T("槓振り") /* 槓振りは本来下位役ではないが下位役判定のシステムを使って複合しないようにする */,
#endif /* GUOBIAO */
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
				(analysis->GameStat->KangFlag.chankanFlag != ChankanStat::none)); // 槍槓フラグが立っている
		}
	));
#ifndef GUOBIAO
	/* 二索搶槓 */
	if (RuleData::chkRuleApplied("ryanzoh_chankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("二索搶槓"), get_yaku_han("ryanzoh_chankan"),
			_T("搶槓"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chankanFlag != ChankanStat::none) && // 槍槓フラグが立っている
					(analysis->TsumoHai->tile == BambooTwo)); // 和了牌が二索
			}
		));
	/* 深搶槓 */
	if (RuleData::chkRuleApplied("shen_chankan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("深搶槓"), get_yaku_han("shen_chankan"),
			_T("搶槓"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chankanFlag != ChankanStat::none) && // 槍槓フラグが立っている
					(isFinalRound(analysis->GameStat))); // オーラスである
			}
		));
	/* 金鶏奪食 */
	if (RuleData::chkRuleApplied("jinji_duoshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金鶏奪食"), get_yaku_han("jinji_duoshi"),
			_T("欠牌和"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->KangFlag.chankanFlag != ChankanStat::none) && // 槍槓フラグが立っている
					((analysis->SeenTiles[analysis->TsumoHai->tile] +
					analysis->TileCount[analysis->TsumoHai->tile]) >=
					(*analysis->TsumoAgariFlag ? 4 : 5)) && // ラス牌かどうか判定
					(analysis->MachiInfo.MachiMen == 1) // 1面待ちである
					);
			}
		));

	// ---------------------------------------------------------------------

	/* 八連荘 */
	if (RuleData::chkRuleApplied("paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("八連荘"), get_yaku_han("paarenchan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->AgariChain == 8)); // 和了ったのが連続8回目
			}
		));
	/* 八連殺し(パーレンブレイカー) */
	if (RuleData::chkRuleApplied("breaking_paarenchan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("破回八連荘"), get_yaku_han("breaking_paarenchan"), // いいぜ、てめえがこの局で八連荘を和了れるっていうなら
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // まずはその
					(analysis->GameStat->AgariChain == -1)); // ふざけた和了を頭ハネ(じゃなくてもいいけど)！
			}
		));

	// ---------------------------------------------------------------------

	/* 燕返し(立直宣言牌でロンすると役が付く) */
	if (RuleData::chkRuleApplied("tsubame_gaeshi"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("燕返し"), get_yaku_han("tsubame_gaeshi"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->RichiCounter)); // フラグが立っている
			}
		));
	/* 飛燕(ダブリー宣言牌でロンすると役が付く) */
	if (RuleData::chkRuleApplied("hien"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("飛燕"), get_yaku_han("hien"),
			_T("燕返し"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->GameStat->DoubleRichiCounter)); // フラグが立っている
			}
		));

	// ---------------------------------------------------------------------

	/* 四馬路 */
	if (RuleData::chkRuleApplied("sumaro"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("四馬路"), get_yaku_han("sumaro"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile == CharacterFour) && // 和了牌が四萬
					(!*analysis->TsumoAgariFlag) && // ロンアガリ
					(analysis->PlayerStat->SumaroFlag)); // 解禁フラグが立っている
			}
		));
	/* 美鈴 */
	if (RuleData::chkRuleApplied("meiling"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("美鈴"), get_yaku_han("meiling"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile == BambooTwo) && // 和了牌が二索
					(!*analysis->TsumoAgariFlag)); // ロンアガリ
			}
		));
	/* 客来和(オタ風の出和了) */
	if (RuleData::chkRuleApplied("otakaze_ron"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("客来和"), get_yaku_han("otakaze_ron"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag;
				switch (TileCode tc = analysis->TsumoHai->tile) {
				case EastWind: case SouthWind: case WestWind: case NorthWind: // 風牌だった
					yakuFlag = (!*analysis->TsumoAgariFlag); // 仮にフラグ設定
					if (tc == Wind2Tile(analysis->GameStat->playerwind(analysis->player))) // 自風だった
						yakuFlag = false;
					if (tc == Wind2Tile(analysis->GameStat->prevailingwind())) // 場風だった
							yakuFlag = false;
					if (RuleData::chkRuleApplied("kaimenkaze") &&
						(tc == Wind2Tile(analysis->GameStat->playerwind(analysis->GameStat->WaremePlayer)))) // 開門風だった
							yakuFlag = false;
					if (RuleData::chkRuleApplied("urakaze") &&
						(tc == Wind2Tile(analysis->GameStat->playerwind(RelativePositionOf(analysis->player, SeatRelative::opposite))))) // 裏風だった
							yakuFlag = false;
					break;
				default: // 風牌じゃなかった
					yakuFlag = false; break;
				}
				return yakuFlag;
			}
		));

	// ---------------------------------------------------------------------

	/* 鳳還巣系列の役 */
	{
		constexpr int Rules = 10;
		constexpr char tmpRuleCodeList[Rules][16] = {
			"houkansou", "gekkanjun", "shinriishou", "kinki_kikan", "hokuto_shoukan",
			"daija_kanketsu", "dongfengchui", "nanfengchui", "xifengchui", "beifengchui",
		};
		constexpr TileCode tmpTileCodeList[Rules] = {
			BambooOne, CircleOne, CharacterOne, BambooSeven, CircleSeven,
			CharacterSeven, EastWind, SouthWind, WestWind, NorthWind,
		};
		constexpr TCHAR tmpYakuNameList[Rules][16] = {
			_T("鳳還巣"), _T("月還巡"), _T("針戻手"), _T("金亀帰還"), _T("北斗召還"),
			_T("大蛇還穴"), _T("東風吹"), _T("南風吹"), _T("西風吹"), _T("北風吹"),
		};
		for (int i = 0; i < Rules; i++) {
			if (RuleData::chkRuleApplied(tmpRuleCodeList[i])) {
				TileCode tc = tmpTileCodeList[i];
				yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
					tmpYakuNameList[i], get_yaku_han(tmpRuleCodeList[i]),
					[tc](const MENTSU_ANALYSIS* const analysis) -> bool {
						bool yakuFlag = false;
						for (int i = 0; i < analysis->PlayerStat->DiscardPointer; i++) {
							if (analysis->PlayerStat->Discard[i].tcode.tile == tc) {
								yakuFlag = true; break; // 現物フリテンである
							}
						}
						return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
							(analysis->TsumoHai->tile == tc) && // 和了牌
							(*analysis->TsumoAgariFlag) && (yakuFlag) && // フリテンツモ
							(analysis->Machi == yaku::yakuCalculator::MachiType::tanki)); // 単騎待ちである
					}
				));
			}
		}
	}

	// ---------------------------------------------------------------------

	/* 金龍天和 */
	if (RuleData::chkRuleApplied("golddragon"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("金龍天和"), get_yaku_han("golddragon"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile >= CircleOne) && // 和了牌が筒子
					(analysis->TsumoHai->tile <= CircleNine) &&
					(analysis->GameStat->TurnRound <= 3)); // 3巡目以内
			}
		));
	/* 旋風栄和 */
	if (RuleData::chkRuleApplied("whirlwind"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("旋風栄和"), get_yaku_han("whirlwind"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->TsumoHai->tile >= EastWind) && // 和了牌が風牌
					(analysis->TsumoHai->tile <= NorthWind) &&
					(analysis->GameStat->TurnRound <= 3)); // 3巡目以内
			}
		));
	/* 青天の霹靂 */
	if (RuleData::chkRuleApplied("bolt_from_the_blue"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("青天の霹靂"), get_yaku_han("bolt_from_the_blue"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(!analysis->GameStat->Player[0].RichiFlag.RichiFlag) && // 誰もリーチしていない
					(!analysis->GameStat->Player[1].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[2].RichiFlag.RichiFlag) &&
					(!analysis->GameStat->Player[3].RichiFlag.RichiFlag) &&
					(analysis->GameStat->Player[0].DiscardPointer == 0) && // 誰も鳴いていない
					(analysis->GameStat->Player[1].DiscardPointer == 0) &&
					(analysis->GameStat->Player[2].DiscardPointer == 0) &&
					(analysis->GameStat->Player[3].DiscardPointer == 0) &&
					(analysis->GameStat->TurnRound <= 4)); // 4巡目以内
			}
		));
	/* 猫まっしぐら */
	if (RuleData::chkRuleApplied("nekomasshigura"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("猫まっしぐら"), get_yaku_han("nekomasshigura"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::regular] == -1) && // 面子手で和了になっている
					(analysis->TsumoHai->tile == BambooThree) && // 和了牌が三索
					(analysis->Machi == yaku::yakuCalculator::MachiType::kanchan)); // 嵌張待ち
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

	/* 欠牌和(元々はchk-post.hspに書いてたけど後回しにする必要なんてなかった) */
#ifdef GUOBIAO
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("和絶張"), 4_fenF,
#else /* GUOBIAO */
	if (RuleData::chkRuleApplied("keppaihoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("欠牌和"), get_yaku_han("keppaihoh"),
#endif /* GUOBIAO */
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				bool yakuFlag =
					*analysis->TsumoAgariFlag ? // ツモアガリだったら、
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // 見えてる牌と
					analysis->TileCount[analysis->TsumoHai->tile] >= 4) // 手の内の牌を足して4枚
					: // ロンだったら、
					(analysis->SeenTiles[analysis->TsumoHai->tile] + // 見えてる牌と
					analysis->TileCount[analysis->TsumoHai->tile] > 4); // 手の内の牌を足して4枚
#ifdef GUOBIAO
				return yakuFlag;
#else /* GUOBIAO */
				return (yakuFlag && // 条件を満たしていて、
					(analysis->MachiInfo.MachiMen == 1)); // 1面待ち(移植時変更：ノベ単とかでは成立しないようにした)
#endif /* GUOBIAO */
			}
		));

	// ---------------------------------------------------------------------

#ifndef GUOBIAO
	/* 初槓 */
	if (RuleData::chkRuleApplied("shokan"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("初槓"), get_yaku_han("shokan"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->shokanFlag)); // フラグが立っている
			}
		));
	/* 槓三巡 */
	if (RuleData::chkRuleApplied("kansanjun"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("槓三巡"), get_yaku_han("kansanjun"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->kansanjunFlag)); // フラグが立っている
			}
		));
	/* 戻牌天和 */
	if (RuleData::chkRuleApplied("renpai_tenhoh"))
		yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
			_T("戻牌天和"), get_yaku_han("renpai_tenhoh"),
			[](const MENTSU_ANALYSIS* const analysis) -> bool {
				return ((analysis->shanten[ShantenType::all] == -1) && // 何かの手で和了になっている
					(analysis->PlayerStat->renpaiTenhohStat > 0)); // フラグが立っている
			}
		));
#endif /* GUOBIAO */

	// ---------------------------------------------------------------------

#ifdef GUOBIAO
	/* 坎張 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("坎張"), 1_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::regular] == -1) && // 面子手で和了になっている
				(analysis->Machi == yaku::yakuCalculator::MachiType::kanchan) && // 嵌張待ちで
				(analysis->MachiInfo.MachiMen == 1)); // 1面待ち
		}
	));
	/* 辺張 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("辺張"), 1_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::regular] == -1) && // 面子手で和了になっている
				(analysis->Machi == yaku::yakuCalculator::MachiType::penchan) && // 辺張待ちで
				(analysis->MachiInfo.MachiMen == 1)); // 1面待ち
		}
	));
	/* 単調将 */
	yaku::yakuCalculator::YakuCatalog::Instantiate()->catalog.push_back(Yaku(
		_T("単調将"), 1_fenF,
		[](const MENTSU_ANALYSIS* const analysis) -> bool {
			return ((analysis->shanten[ShantenType::regular] == -1) && // 面子手で和了になっている
				(analysis->Machi == yaku::yakuCalculator::MachiType::tanki) && // 単騎待ちで
				(analysis->MachiInfo.MachiMen == 1)); // 1面待ち
		}
	));
#endif /* GUOBIAO */
}
