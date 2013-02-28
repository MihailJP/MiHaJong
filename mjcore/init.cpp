#include "init.h"

#include "logging.h"
#include "func.h"
#include <string>
#include "../common/strcode.h"
#include "../common/version.h"
#include "envtbl.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "sound.h"
#include "shanten.h"
#include "random.h"
#include "../graphic/graphic.h"
#include "yaku/yaku.h"
#include "ruletbl.h"

MJCORE void initapp(GameTypeID gameType, HWND hwnd) {
	/* コンフィグファイルのパスを設定する */
	/* Vista以降、Program Files以下にファイルを作れないので自分で調整する */
	std::string configFile;
	switch (gameType) {
	case Yonma:
		configFile = confpath::confPath() + "mihajong.ini";
		break;
	case Sanma:
		configFile = confpath::confPath() + "mihasanm.ini";
		break;
	case Sanma4:
		configFile = confpath::confPath() + "mihaysnm.ini";
		break;
	case SanmaS:
		configFile = confpath::confPath() + "mihassnm.ini";
		break;
	}

	/* ログ初期化 */
	{
		logger::initLogger(); CodeConv::tostringstream o;
		o << _T("MiHaJong Ver. ") << MIHAJONG_VER; info(o.str().c_str()); o.str(_T(""));
		o << _T("ビルド日時は ") << _T(__DATE__) << _T(" ") << _T(__TIME__) << _T(" です。"); info(o.str().c_str());
	}

	/* 解像度をチェックする */ // TODO: ここを作りなおす
	/*info "画面の解像度は "+ginfo_dispx+" x "+ginfo_dispy+" です。"
	if ((ginfo_dispx < 1024)||(ginfo_dispy < 768)) {
		errordlg "画面解像度が足りません\nXGA(1024x768)以上で起動してください", "画面が小さすぎます"
		error "解像度要件を満たしていません。終了します。"
		end 1
	}*/

	/* 画面を初期化 */
	/* screen SCR_CHAT, 160, 630, 14, ((ginfo_dispx-840)/2)+840-76, (ginfo_dispy-630)/2-20
	info "チャット用のウィンドウを仮初期化しました。"
	gsel 0, -1: await 0*/

	/* スプラッシュスクリーン */ // TODO: スプラッシュスクリーン的なものを作りなおす
	/*buffer SCR_SPLASH_BUF: pngload "img\\splash.png"
	bgscr SCR_SPLASH_WINDOW, 320, 240, , (ginfo_dispx-320)/2, (ginfo_dispy-240)/2
	gsel SCR_SPLASH_WINDOW, 2
	menubgcolor
	boxf 0, 0, 319, 239
	gmode 7: pos 0, 0: gcopy SCR_SPLASH_BUF, 0, 0, 320, 240
	color 0, 0, 0: line 319, 0, 0, 0: line 319, 239: line 0, 239: line 0, 0
	tmptxt = "Version "+VERSION_MAJ+"."+VERSION_MED+"."+VERSION_MIN+VERSION_MIC
	borderedtxt2 tmptxt, 160-(strlen(tmptxt)*4), 130, 255, 255, 255, 0, 16, 192, 192, 192
	tmptxt = "Copyright (c) 2008-2012 MihailJP"
	borderedtxt2 tmptxt, 160-(strlen(tmptxt)*4), 160, 255, 255, 255, 0, 16, 192, 192, 192
	tmptxt = "Part of rights reserved"
	borderedtxt2 tmptxt, 160-(strlen(tmptxt)*4), 180, 255, 255, 255, 0, 16, 192, 192, 192
	gsel 0, 0: await 0*/

	/* 卓の環境を初期化 */
	{
		EnvTable::Instantiate()->bgColorR =
			EnvTable::Instantiate()->bgColorG =
			EnvTable::Instantiate()->bgColorB = 0;
		GameStat.gameType = gameType;
		mihajong_graphic::GameStatus::updateGameStat(&GameStat);
	}

	/* 設定ファイル読み込み */
	{
		mihajong_graphic::rules::setconffile(configFile.c_str());
		bool cnfFileExists = exist(configFile.c_str()); // 設定ファイルがあるかどうか調べる
		RuleData::configinit();
		if (!cnfFileExists) {
			info(_T("設定ファイルが見つかりません。デフォルトの設定を使用します。"));
			RuleData::saveConfigFile(configFile.c_str()); // デフォルトのコンフィグデータを作成
		} else {
			info(_T("設定ファイルが見つかりました。読み込みを開始します。"));
			RuleData::loadConfigFile(configFile.c_str()); // 設定ファイル読み込み
		}
		// UI用のDLLに関数の場所を教える
		mihajong_graphic::rules::setfunc(
			RuleData::getRuleName, RuleData::getRuleDescription, RuleData::getRuleTxt,
			RuleData::getRule, RuleData::getRuleSize, RuleData::reqFailed,
			RuleData::getPageCaption, RuleData::storeRule, RuleData::exportRule,
			RuleData::saveConfigFile, RuleData::chkRule, RuleData::ruleDigit());
		mihajong_graphic::utils::setfunc(
			playerwind, playerRelative, RelativePositionOf, tilesLeft, calcRank, chkFuriten,
			isTenpai, isRichiReqSatisfied, ShantenAnalyzer::calcShanten, chkdaopaiability,
			yaku::yakuCalculator::chkShisanBuDa, yaku::yakuCalculator::chkShisiBuDa,
			countTilesInHand, chkAnkanAbility, isPao, isPaoAgari, sound::util::bgmplay);
	}

	/* 音源を初期化 */
	if (sound::Initialize(hwnd) == 0)
		info(_T("サウンドDLLを初期化しました。"));
	else
		error(_T("サウンドDLLの初期化に失敗しました。"));

	/* 終了時に音源をクリンナップするための処理 */
	/* TODO: これを移植
	onexit *cleanup
#ifndef _debug
	onerror goto *errorproc
#endif*/

	/* 面子構成データベースの読み込み */
	/* 向聴数の計算に使用：構築には１ヶ月必要 */
	ShantenAnalyzer::initMentsuAnalysisDat();

	/* 擬似乱数を初期化 */
	RndNum::init();
	info(_T("疑似乱数を初期化しました。"));

	/* 牌やサイコロの画像を読み込む */ // TODO: これは多分もういらない。あとで確認すること
	/*gmode gmode_mem
	buffer SCR_TILE_PIC: picload "img\\tileset.gif": info "牌の画像を読み込みました。"
	buffer SCR_TILE_BLOCK, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X, TILE_BLOCK_SCREEN_Y, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	buffer SCR_TILE_SHADE, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_Y, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	buffer SCR_DICE_PIC, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH*2+TILE_DICE_SCREEN_WIDTH, TILE_BLOCK_SCREEN_Y, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	buffer SCR_DICE_SHADE, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH*2, TILE_BLOCK_SCREEN_Y, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	info "牌の画像の処理が完了しました。"*/

	/* タイトルロゴや画像の読み込み */ // TODO: これは多分もういらない。あとで確認すること
	/*buffer SCR_MAIN_LOGO: pngload "img\\logo.png": info "闘牌時の右上ロゴを読み込みました。"
	buffer SCR_MAIN_BACKGROUND: pngload "img\\mainbg.png": info "闘牌時の背景画像を読み込みました。"
	buffer SCR_TITLE_BACKGROUND: pngload "img\\background.png": info "タイトルロゴを読み込みました。"
	buffer SCR_TITLE_LOGO: pngload "img\\title.png": info "タイトル画面の背景画像を読み込みました。"*/

	/* ＢＧＭを読み込み */
	for (unsigned i = sound::IDs::BgmStart; i < (sound::IDs::BgmRounds - sound::IDs::BgmStart); i++) {
		std::ostringstream o; o << "bgm" << (i + 1);
		sound::util::bgmload(i, o.str().c_str(), true);
	}

	/* ＢＧＭを読み込み（続き） */
	sound::util::bgmload(sound::IDs::musRichi1, "richi1", true); // 他家リーチ
	sound::util::bgmload(sound::IDs::musRichi2, "richi2", true); // 自家リーチ
	sound::util::bgmload(sound::IDs::musRichi3, "richi3", true); // 追っかけリーチ
	sound::util::bgmload(sound::IDs::musOpenrichi, "opnrichi", true); // オープン立直
	sound::util::bgmload(sound::IDs::musAgariSelf1, "agari1", false); // 満貫未満、自分の和了
	sound::util::bgmload(sound::IDs::musAgariSelf2, "agari2", false); // 満貫以上、自分の和了
	sound::util::bgmload(sound::IDs::musAgariSelf3, "agari3", false); // 役満、自分の和了
	sound::util::bgmload(sound::IDs::musAgariFurikomi1, "agari4", false); // 満貫未満、放銃または被自摸
	sound::util::bgmload(sound::IDs::musAgariFurikomi2, "agari5", false); // 満貫以上、放銃または被自摸
	sound::util::bgmload(sound::IDs::musAgariFurikomi3, "agari6", false); // 役満、放銃または被自摸
	sound::util::bgmload(sound::IDs::musAgariOther1, "agari7", false); // 満貫未満、他家の栄和
	sound::util::bgmload(sound::IDs::musAgariOther2, "agari8", false); // 満貫以上、他家の栄和
	sound::util::bgmload(sound::IDs::musAgariOther3, "agari9", false); // 役満、他家の栄和
	sound::util::bgmload(sound::IDs::musRyuukyoku, "pingju", false); // 流局・錯和
	sound::util::bgmload(sound::IDs::musFinal, "final", true); // オーラス曲
	sound::util::bgmload(sound::IDs::musShibari, "shibari", true); // リャンシバ時の曲
	sound::util::bgmload(sound::IDs::musTitle, "title", true);
	sound::util::bgmload(sound::IDs::musEnding, "ending", true);
	sound::util::bgmload(sound::IDs::musEnding2, "ending2", true);
	sound::util::bgmload(sound::IDs::musEnding3, "ending3", true);

	/* 効果音読み込み(DirectSound使用) */
	sound::util::soundload(sound::IDs::sndDahai1, "sound\\dahai1.wav", false);
	sound::util::soundload(sound::IDs::sndDahai2, "sound\\dahai2.wav", false);
	sound::util::soundload(sound::IDs::sndTsumo, "sound\\tsumo.wav", false);
	sound::util::soundload(sound::IDs::sndSaikoro, "sound\\saikoro.wav", false);
	sound::util::soundload(sound::IDs::sndBell, "sound\\bell.wav", false);
	sound::util::soundload(sound::IDs::sndCountdown, "sound\\countdn.wav", false);
	sound::util::soundload(sound::IDs::sndCuohu, "sound\\chonbo.wav", false);
	sound::util::soundload(sound::IDs::sndYakulst1, "sound\\yakulst1.wav", false);
	sound::util::soundload(sound::IDs::sndYakulst2, "sound\\yakulst2.wav", false);
	sound::util::soundload(sound::IDs::sndMekuri, "sound\\mekuri.wav", false);
	sound::util::soundload(sound::IDs::sndButton, "sound\\button.wav", false);
	sound::util::soundload(sound::IDs::sndClick, "sound\\click.wav", false);
	sound::util::soundload(sound::IDs::sndCursor, "sound\\cursor.wav", false);
	sound::util::soundload(sound::IDs::sndPage, "sound\\page.wav", false);
	sound::util::soundload(sound::IDs::sndType, "sound\\type.wav", false);
	sound::util::soundload(sound::IDs::sndPingju, "sound\\pingju.wav", false);
	sound::util::soundload(sound::IDs::sndFlash, "sound\\flash.wav", false);
	sound::util::soundload(sound::IDs::sndSignal, "sound\\signal.wav", false);
	sound::util::soundload(sound::IDs::sndClock, "sound\\clock.wav", false);

	/* 鳴き仕掛け関係の効果音を読み込む */
	sound::util::soundload(sound::IDs::voxChi, "sound\\chi.wav", false);
	sound::util::soundload(sound::IDs::voxPon, "sound\\pon.wav", false);
	sound::util::soundload(sound::IDs::voxKan, "sound\\kan.wav", false);
	sound::util::soundload(sound::IDs::voxRichi, "sound\\richi.wav", false);
	sound::util::soundload(sound::IDs::voxTsumo, "sound\\agari1.wav", false);
	sound::util::soundload(sound::IDs::voxRon, "sound\\agari2.wav", false);
	sound::util::soundload(sound::IDs::voxKyuushu, "sound\\kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxFlower, "sound\\flower.wav", false);
	sound::util::soundload(sound::IDs::voxSikang, "sound\\kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSifeng, "sound\\kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSanjiahu, "sound\\kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSijiarichi, "sound\\kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxRonFurikomi, "sound\\agari2.wav", false);

	/* スプラッシュスクリーンをしまう */ // TODO: ここを作りなおす
	/*gsel SCR_SPLASH_WINDOW, -1
	screen 0, 840, 630, , (ginfo_dispx-840)/2, (ginfo_dispy-630)/2-20
	info "ウィンドウを初期化しました。"
	gsel 0, 1*/

}
