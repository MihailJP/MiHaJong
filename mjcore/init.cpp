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
#include "chat.h"

#ifdef _WIN32
MJCORE void initapp(GameTypeID gameType, HWND hwnd)
#else /*_WIN32*/
MJCORE void initapp(GameTypeID gameType, Window hwnd)
#endif /*_WIN32*/
{
	/* �R���t�B�O�t�@�C���̃p�X��ݒ肷�� */
	/* Vista�ȍ~�AProgram Files�ȉ��Ƀt�@�C�������Ȃ��̂Ŏ����Œ������� */
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
	std::string preferenceFile = confpath::confPath() + "config.ini";

#ifdef _WIN32 /* Windows�łł̂ݎ��� */
	/* ���O������ */
	{
		logger::initLogger(); CodeConv::tostringstream o;
		o << _T("MiHaJong Ver. ") << MIHAJONG_VER; info(o.str().c_str()); o.str(_T(""));
		o << _T("�r���h������ ") << _T(__DATE__) << _T(" ") << _T(__TIME__) << _T(" �ł��B"); info(o.str().c_str());
	}
#endif /*_WIN32*/

	/* ��̊��������� */
	{
		EnvTable::Instantiate()->bgColorR =
			EnvTable::Instantiate()->bgColorG =
			EnvTable::Instantiate()->bgColorB = 0;
		GameStat.gameType = gameType;
		mihajong_graphic::GameStatus::updateGameStat(&GameStat);
	}

	/* �ݒ�t�@�C���ǂݍ��� */
	{
		mihajong_graphic::rules::setconffile(configFile.c_str(), preferenceFile.c_str());
		bool cnfFileExists = exist(configFile.c_str()); // �ݒ�t�@�C�������邩�ǂ������ׂ�
		bool prefFileExists = exist(preferenceFile.c_str()); // �ݒ�t�@�C�������邩�ǂ������ׂ�
		RuleData::configinit();
		if (!cnfFileExists) {
			info(_T("���[���ݒ�t�@�C����������܂���B�f�t�H���g�̐ݒ���g�p���܂��B"));
			RuleData::saveConfigFile(configFile.c_str()); // �f�t�H���g�̃R���t�B�O�f�[�^���쐬
		} else {
			info(_T("���[���ݒ�t�@�C����������܂����B�ǂݍ��݂��J�n���܂��B"));
			RuleData::loadConfigFile(configFile.c_str()); // �ݒ�t�@�C���ǂݍ���
		}
		if (!prefFileExists) {
			info(_T("���ݒ�t�@�C����������܂���B�f�t�H���g�̐ݒ���g�p���܂��B"));
			RuleData::savePreferenceFile(preferenceFile.c_str()); // �f�t�H���g�̃R���t�B�O�f�[�^���쐬
		} else {
			info(_T("���ݒ�t�@�C����������܂����B�ǂݍ��݂��J�n���܂��B"));
			RuleData::loadPreferenceFile(preferenceFile.c_str()); // �ݒ�t�@�C���ǂݍ���
		}
		// UI�p��DLL�Ɋ֐��̏ꏊ��������
		mihajong_graphic::rules::setfunc(
			RuleData::getRuleName, RuleData::getRuleDescription, RuleData::getRuleTxt,
			RuleData::getRule, RuleData::getRuleSize, RuleData::reqFailed,
			RuleData::getPageCaption, RuleData::storeRule, RuleData::exportRule,
			RuleData::saveConfigFile, RuleData::chkRule, RuleData::ruleDigit(),
			RuleData::storePref, RuleData::exportPref, RuleData::savePreferenceFile,
			RuleData::getPreferenceName, RuleData::getPreferenceDescription, RuleData::getPreferenceTxt,
			RuleData::getPreference, RuleData::getPreferenceSize,
			RuleData::getPreferenceInputSize, RuleData::getPreferenceRawStr, RuleData::setPreferenceFreeStr,
			RuleData::chkPreference);
		mihajong_graphic::utils::setfunc(
			playerRelative, RelativePositionOf, calcRank, chkFuriten,
			isTenpai, isRichiReqSatisfied, ShantenAnalyzer::calcShanten, chkdaopaiability,
			yaku::yakuCalculator::chkShisanBuDa, yaku::yakuCalculator::chkShisiBuDa,
			countTilesInHand, chkAnkanAbility, isPao, isPaoAgari, sound::util::bgmplay,
			getName, chat::sendchat);
	}

	/* ������������ */
	if (sound::Initialize(hwnd) == 0)
		info(_T("�T�E���hDLL�����������܂����B"));
	else
		error(_T("�T�E���hDLL�̏������Ɏ��s���܂����B"));

	/* �ʎq�\���f�[�^�x�[�X�̓ǂݍ��� */
	/* �������̌v�Z�Ɏg�p�F�\�z�ɂ̓N�A�b�h�R�A�ňꒋ��K�v */
	ShantenAnalyzer::initMentsuAnalysisDat();

	/* �[�������������� */
	RndNum::init();
	info(_T("�^�����������������܂����B"));

	/* �a�f�l��ǂݍ��� */
	for (unsigned i = sound::IDs::BgmStart; i < (sound::IDs::BgmRounds - sound::IDs::BgmStart); i++) {
		std::ostringstream o; o << "bgm" << (i + 1);
		sound::util::bgmload(i, o.str().c_str(), true);
	}

	/* �a�f�l��ǂݍ��݁i�����j */
	sound::util::bgmload(sound::IDs::musRichi1, "richi1", true); // ���ƃ��[�`
	sound::util::bgmload(sound::IDs::musRichi2, "richi2", true); // ���ƃ��[�`
	sound::util::bgmload(sound::IDs::musRichi3, "richi3", true); // �ǂ��������[�`
	sound::util::bgmload(sound::IDs::musOpenrichi, "opnrichi", true); // �I�[�v������
	sound::util::bgmload(sound::IDs::musAgariSelf1, "agari1", false); // ���і����A�����̘a��
	sound::util::bgmload(sound::IDs::musAgariSelf2, "agari2", false); // ���шȏ�A�����̘a��
	sound::util::bgmload(sound::IDs::musAgariSelf3, "agari3", false); // �𖞁A�����̘a��
	sound::util::bgmload(sound::IDs::musAgariFurikomi1, "agari4", false); // ���і����A���e�܂��͔펩��
	sound::util::bgmload(sound::IDs::musAgariFurikomi2, "agari5", false); // ���шȏ�A���e�܂��͔펩��
	sound::util::bgmload(sound::IDs::musAgariFurikomi3, "agari6", false); // �𖞁A���e�܂��͔펩��
	sound::util::bgmload(sound::IDs::musAgariOther1, "agari7", false); // ���і����A���Ƃ̉h�a
	sound::util::bgmload(sound::IDs::musAgariOther2, "agari8", false); // ���шȏ�A���Ƃ̉h�a
	sound::util::bgmload(sound::IDs::musAgariOther3, "agari9", false); // �𖞁A���Ƃ̉h�a
	sound::util::bgmload(sound::IDs::musRyuukyoku, "pingju", false); // ���ǁE���a
	sound::util::bgmload(sound::IDs::musFinal, "final", true); // �I�[���X��
	sound::util::bgmload(sound::IDs::musShibari, "shibari", true); // �������V�o���̋�
	sound::util::bgmload(sound::IDs::musTitle, "title", true);
	sound::util::bgmload(sound::IDs::musEnding, "ending", true);
	sound::util::bgmload(sound::IDs::musEnding2, "ending2", true);
	sound::util::bgmload(sound::IDs::musEnding3, "ending3", true);

	/* ���ʉ��ǂݍ���(DirectSound�g�p) */
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

	/* ���d�|���֌W�̌��ʉ���ǂݍ��� */
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

}

MJCORE void testInit() {
	setGameType(Yonma);
	RuleData::configinit();
	ShantenAnalyzer::initMentsuAnalysisDat();
}
