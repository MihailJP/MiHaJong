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
#include "conffile/conffile.h"

#ifdef _WIN32
#define SOUNDDIR "sound\\"
#else /*_WIN32*/
#define SOUNDDIR "sound/"
#endif /*_WIN32*/

#ifdef _WIN32
MJCORE void initapp(GameTypeID gameType, HWND hwnd)
#else /*_WIN32*/
MJCORE void initapp(GameTypeID gameType, Window hwnd)
#endif /*_WIN32*/
{
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
	case SanmaSeto:
		configFile = confpath::confPath() + "mihastsm.ini";
		break;
	}

#ifdef _WIN32 /* Windows版でのみ実装 */
	/* ログ初期化 */
	{
		logger::initLogger(); CodeConv::tostringstream o;
		o << _T("MiHaJong Ver. ") << MIHAJONG_VER; info(o.str().c_str()); o.str(_T(""));
		o << _T("ビルド日時は ") << _T(__DATE__) << _T(" ") << _T(__TIME__) << _T(" です。"); info(o.str().c_str());
	}
#endif /*_WIN32*/

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
		bool cnfFileExists = exist(configFile.c_str()); // 設定ファイルがあるかどうか調べる
		RuleData::configinit();
		if (!cnfFileExists) {
			info(_T("ルール設定ファイルが見つかりません。デフォルトの設定を使用します。"));
			RuleData::saveConfigFile(configFile.c_str()); // デフォルトのコンフィグデータを作成
		} else {
			info(_T("ルール設定ファイルが見つかりました。読み込みを開始します。"));
			RuleData::loadConfigFile(configFile.c_str()); // 設定ファイル読み込み
		}
		mihajong_graphic::rules::setconffile(configFile.c_str());
		// UI用のDLLに関数の場所を教える
		mihajong_graphic::rules::setfunc(
			RuleData::getRuleName, RuleData::getRuleDescription, RuleData::getRuleTxt,
			RuleData::getRule, RuleData::getRuleSize, RuleData::reqFailed,
			RuleData::getPageCaption, RuleData::storeRule, RuleData::exportRule,
			RuleData::saveConfigFile, RuleData::chkRule, RuleData::ruleDigit());
		mihajong_graphic::utils::setfunc(
			playerRelative, RelativePositionOf, calcRank, chkFuriten,
			isTenpai, isRichiReqSatisfied, ShantenAnalyzer::calcShanten, chkdaopaiability,
			yaku::yakuCalculator::chkShisanBuDa, yaku::yakuCalculator::chkShisiBuDa,
			countTilesInHand, chkAnkanAbility, isPao, isPaoAgari, sound::util::bgmplay,
			getName, isWatchMode, isAboveBase, isStandAlone, MoveTile::enqueue);
		{
			using namespace mihajong_graphic::preferences;
			auto tmpAddr(CodeConv::toANSI(RuleData::confFile.serverAddress()));
			useBlackTile(RuleData::confFile.blackTile());
			setServerAddr(tmpAddr.c_str());
		}
	}

	/* 音源を初期化 */
	if (sound::Initialize(hwnd, RuleData::confFile.midiDevice().c_str()) == 0)
		info(_T("サウンドDLLを初期化しました。"));
	else
		error(_T("サウンドDLLの初期化に失敗しました。"));

	/* 面子構成データベースの読み込み */
	/* 向聴数の計算に使用：構築にはクアッドコアで一昼夜必要 */
	ShantenAnalyzer::initMentsuAnalysisDat();

	/* 擬似乱数を初期化 */
	RndNum::init();
	info(_T("疑似乱数を初期化しました。"));

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
	sound::util::soundload(sound::IDs::sndDahai1, SOUNDDIR "dahai1.wav", false);
	sound::util::soundload(sound::IDs::sndDahai2, SOUNDDIR "dahai2.wav", false);
	sound::util::soundload(sound::IDs::sndTsumo, SOUNDDIR "tsumo.wav", false);
	sound::util::soundload(sound::IDs::sndSaikoro, SOUNDDIR "saikoro.wav", false);
	sound::util::soundload(sound::IDs::sndBell, SOUNDDIR "bell.wav", false);
	sound::util::soundload(sound::IDs::sndCountdown, SOUNDDIR "countdn.wav", false);
	sound::util::soundload(sound::IDs::sndCuohu, SOUNDDIR "chonbo.wav", false);
	sound::util::soundload(sound::IDs::sndYakulst1, SOUNDDIR "yakulst1.wav", false);
	sound::util::soundload(sound::IDs::sndYakulst2, SOUNDDIR "yakulst2.wav", false);
	sound::util::soundload(sound::IDs::sndMekuri, SOUNDDIR "mekuri.wav", false);
	sound::util::soundload(sound::IDs::sndButton, SOUNDDIR "button.wav", false);
	sound::util::soundload(sound::IDs::sndClick, SOUNDDIR "click.wav", false);
	sound::util::soundload(sound::IDs::sndCursor, SOUNDDIR "cursor.wav", false);
	sound::util::soundload(sound::IDs::sndPage, SOUNDDIR "page.wav", false);
	sound::util::soundload(sound::IDs::sndType, SOUNDDIR "type.wav", false);
	sound::util::soundload(sound::IDs::sndPingju, SOUNDDIR "pingju.wav", false);
	sound::util::soundload(sound::IDs::sndFlash, SOUNDDIR "flash.wav", false);
	sound::util::soundload(sound::IDs::sndSignal, SOUNDDIR "signal.wav", false);
	sound::util::soundload(sound::IDs::sndClock, SOUNDDIR "clock.wav", false);

	/* 鳴き仕掛け関係の効果音を読み込む */
	sound::util::soundload(sound::IDs::voxChi, SOUNDDIR "chi.wav", false);
	sound::util::soundload(sound::IDs::voxPon, SOUNDDIR "pon.wav", false);
	sound::util::soundload(sound::IDs::voxKan, SOUNDDIR "kan.wav", false);
	sound::util::soundload(sound::IDs::voxRichi, SOUNDDIR "richi.wav", false);
	sound::util::soundload(sound::IDs::voxTsumo, SOUNDDIR "agari1.wav", false);
	sound::util::soundload(sound::IDs::voxRon, SOUNDDIR "agari2.wav", false);
	sound::util::soundload(sound::IDs::voxKyuushu, SOUNDDIR "kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxFlower, SOUNDDIR "flower.wav", false);
	sound::util::soundload(sound::IDs::voxSikang, SOUNDDIR "kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSifeng, SOUNDDIR "kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSanjiahu, SOUNDDIR "kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxSijiarichi, SOUNDDIR "kyuushu.wav", false);
	sound::util::soundload(sound::IDs::voxRonFurikomi, SOUNDDIR "agari2.wav", false);

	/* 音量設定を読み込み */
	sound::util::setvolume();
}

MJCORE void testInit() {
	setGameType(Yonma);
	RuleData::configinit();
	ShantenAnalyzer::initMentsuAnalysisDat();
}
