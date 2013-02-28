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

	/* ���O������ */
	{
		logger::initLogger(); CodeConv::tostringstream o;
		o << _T("MiHaJong Ver. ") << MIHAJONG_VER; info(o.str().c_str()); o.str(_T(""));
		o << _T("�r���h������ ") << _T(__DATE__) << _T(" ") << _T(__TIME__) << _T(" �ł��B"); info(o.str().c_str());
	}

	/* �𑜓x���`�F�b�N���� */ // TODO: ���������Ȃ���
	/*info "��ʂ̉𑜓x�� "+ginfo_dispx+" x "+ginfo_dispy+" �ł��B"
	if ((ginfo_dispx < 1024)||(ginfo_dispy < 768)) {
		errordlg "��ʉ𑜓x������܂���\nXGA(1024x768)�ȏ�ŋN�����Ă�������", "��ʂ����������܂�"
		error "�𑜓x�v���𖞂����Ă��܂���B�I�����܂��B"
		end 1
	}*/

	/* ��ʂ������� */
	/* screen SCR_CHAT, 160, 630, 14, ((ginfo_dispx-840)/2)+840-76, (ginfo_dispy-630)/2-20
	info "�`���b�g�p�̃E�B���h�E�������������܂����B"
	gsel 0, -1: await 0*/

	/* �X�v���b�V���X�N���[�� */ // TODO: �X�v���b�V���X�N���[���I�Ȃ��̂����Ȃ���
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
		mihajong_graphic::rules::setconffile(configFile.c_str());
		bool cnfFileExists = exist(configFile.c_str()); // �ݒ�t�@�C�������邩�ǂ������ׂ�
		RuleData::configinit();
		if (!cnfFileExists) {
			info(_T("�ݒ�t�@�C����������܂���B�f�t�H���g�̐ݒ���g�p���܂��B"));
			RuleData::saveConfigFile(configFile.c_str()); // �f�t�H���g�̃R���t�B�O�f�[�^���쐬
		} else {
			info(_T("�ݒ�t�@�C����������܂����B�ǂݍ��݂��J�n���܂��B"));
			RuleData::loadConfigFile(configFile.c_str()); // �ݒ�t�@�C���ǂݍ���
		}
		// UI�p��DLL�Ɋ֐��̏ꏊ��������
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

	/* ������������ */
	if (sound::Initialize(hwnd) == 0)
		info(_T("�T�E���hDLL�����������܂����B"));
	else
		error(_T("�T�E���hDLL�̏������Ɏ��s���܂����B"));

	/* �I�����ɉ������N�����i�b�v���邽�߂̏��� */
	/* TODO: ������ڐA
	onexit *cleanup
#ifndef _debug
	onerror goto *errorproc
#endif*/

	/* �ʎq�\���f�[�^�x�[�X�̓ǂݍ��� */
	/* �������̌v�Z�Ɏg�p�F�\�z�ɂ͂P�����K�v */
	ShantenAnalyzer::initMentsuAnalysisDat();

	/* �[�������������� */
	RndNum::init();
	info(_T("�^�����������������܂����B"));

	/* �v��T�C�R���̉摜��ǂݍ��� */ // TODO: ����͑�����������Ȃ��B���ƂŊm�F���邱��
	/*gmode gmode_mem
	buffer SCR_TILE_PIC: picload "img\\tileset.gif": info "�v�̉摜��ǂݍ��݂܂����B"
	buffer SCR_TILE_BLOCK, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X, TILE_BLOCK_SCREEN_Y, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	buffer SCR_TILE_SHADE, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_Y, TILE_BLOCK_SCREEN_WIDTH, TILE_BLOCK_SCREEN_HEIGHT
	buffer SCR_DICE_PIC, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH*2+TILE_DICE_SCREEN_WIDTH, TILE_BLOCK_SCREEN_Y, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	buffer SCR_DICE_SHADE, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	gcopy SCR_TILE_PIC, TILE_BLOCK_SCREEN_X+TILE_BLOCK_SCREEN_WIDTH*2, TILE_BLOCK_SCREEN_Y, TILE_DICE_SCREEN_WIDTH, TILE_DICE_SCREEN_HEIGHT
	info "�v�̉摜�̏������������܂����B"*/

	/* �^�C�g�����S��摜�̓ǂݍ��� */ // TODO: ����͑�����������Ȃ��B���ƂŊm�F���邱��
	/*buffer SCR_MAIN_LOGO: pngload "img\\logo.png": info "���v���̉E�ネ�S��ǂݍ��݂܂����B"
	buffer SCR_MAIN_BACKGROUND: pngload "img\\mainbg.png": info "���v���̔w�i�摜��ǂݍ��݂܂����B"
	buffer SCR_TITLE_BACKGROUND: pngload "img\\background.png": info "�^�C�g�����S��ǂݍ��݂܂����B"
	buffer SCR_TITLE_LOGO: pngload "img\\title.png": info "�^�C�g����ʂ̔w�i�摜��ǂݍ��݂܂����B"*/

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

	/* �X�v���b�V���X�N���[�������܂� */ // TODO: ���������Ȃ���
	/*gsel SCR_SPLASH_WINDOW, -1
	screen 0, 840, 630, , (ginfo_dispx-840)/2, (ginfo_dispy-630)/2-20
	info "�E�B���h�E�����������܂����B"
	gsel 0, 1*/

}
