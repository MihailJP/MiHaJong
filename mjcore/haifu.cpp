#include "haifu.h"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "largenum.h"
#include "envtbl.h"
#include "../common/version.h"
#include "chat.h"
#include "ruletbl.h"

/* ���v�̖��O�f�[�^ */
const CodeConv::tstring haifu::tilecodelabel =
	_T("�H���O�l�ܘZ������H�@�A�B�C�D�E�F�G�H�H�P�Q�R�S�T�U�V�W�X�H���쐼�k��ᢒ��H�H�H���Q��ޗ����J��H�J�K�L�M�N�O�P�Q�R�H�T�U�V�W�X�Y�Z�[�\�H�d�r�v�m�o�e�b�H�H�H�b��������ȍM�h�p�H����͂ɂقւƂ���H�@�A�B�C�D�E�F�G�H�H���������������H�H�H�t�ďH�~�S�~���e�|�H");
const CodeConv::tstring haifu::HTtilecodelabel1 =
	_T(" qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   -^\\[5@;:]");
const CodeConv::tstring haifu::HTtilecodelabel2 =
	_T(" QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   -^\\[%@;:]");

const std::array<CodeConv::tstring, TileFlowerMax> haifu::Xtilerefcode = {
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&m1;"),     _T("&m2;"),     _T("&m3;"),     _T("&m4;"),     _T("&m5;"),    _T("&m6;"),    _T("&m7;"),     _T("&m8;"),    _T("&m9;"),
	_T(""), _T("&p1;"),     _T("&p2;"),     _T("&p3;"),     _T("&p4;"),     _T("&p5;"),    _T("&p6;"),    _T("&p7;"),     _T("&p8;"),    _T("&p9;"),
	_T(""), _T("&s1;"),     _T("&s2;"),     _T("&s3;"),     _T("&s4;"),     _T("&s5;"),    _T("&s6;"),    _T("&s7;"),     _T("&s8;"),    _T("&s9;"),
	_T(""), _T("&east;"),   _T("&south;"),  _T("&west;"),   _T("&north;"),  _T("&white;"), _T("&green;"), _T("&red;"),    _T(""),        _T(""),
	_T(""), _T("&spring;"), _T("&summer;"), _T("&autumn;"), _T("&winter;"), _T("&joker;"), _T("&plum;"),  _T("&orchid;"), _T("&chrys;"), _T("&bamboo;"),
};

InfoByPlayer<LNum> haifu::origPoint;
CodeConv::tostringstream haifu::haifuBuffer, haifu::HThaifuBuffer,
	haifu::XMLhaifuBuffer, haifu::XhaifuBuffer, haifu::XhaifuBufferBody;
bool haifu::haifukanflag = false;

haifu::HaifuStreams haifu::haifuP, haifu::HThaifuP, haifu::XhaifuP;

/* �v���L�^�p�̕⏕���[�`�� */
void haifu::tools::haifuskip(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PlayerID PassivePlayer, PlayerID ActivePlayer
	) {
		static CodeConv::tostringstream* p[] = {
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
		};
		static CodeConv::tostringstream* h[] = {
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
			&HThaifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
		};
		if (playerRelative(ActivePlayer, PassivePlayer) == sOpposite) {
			// �Ζʂ���|�������ꍇ
			for (int i = 0; i < 4; i++) {
				*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
			}
			checkCycle();
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
				(int)RelativePositionOf(ActivePlayer, sRight) << _T("\" />") << std::endl;
		}
		if (playerRelative(ActivePlayer, PassivePlayer) == sRight) {
			// ���Ƃ���|�������ꍇ
			for (int i = 0; i < 8; i++) {
				*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
			}
			checkCycle();
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
				(int)RelativePositionOf(ActivePlayer, sRight) << _T("\" />") << std::endl;
			checkCycle();
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
				(int)RelativePositionOf(ActivePlayer, sOpposite) << _T("\" />") << std::endl;
		}
}

CodeConv::tstring haifu::tools::haifudoraClass(doraCol Akadora) { // �Ԕv���v�̕�(HTML�v��)
	switch (Akadora) {
	case AkaDora:
		return _T(" class=\"akadora\"");
	case AoDora:
		return _T(" class=\"aodora\"");
	default:
		return _T("");
	}
}
CodeConv::tstring haifu::tools::haifudoraClassX(doraCol Akadora) { // �Ԕv���v�̕�(XML�v��)
	switch (Akadora) {
	case AkaDora:
		return _T(" dora=\"red\"");
	case AoDora:
		return _T(" dora=\"blue\"");
	default:
		return _T("");
	}
}

void haifu::tools::recordDoraStream(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, CodeConv::tostringstream* const x, TileCode tmpDora) {
	*p << tilecodelabel.substr((int)tmpDora * StringElemSize, StringElemSize);
	*h << HTtilecodelabel1.substr((int)tmpDora, 1);
	*x << _T("\t\t\t\t<tile tile=\"") << Xtilerefcode[tmpDora] << _T("\" />") << std::endl;
}

/* �v���L�^���� */
void haifu::tools::recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, Tile tlCode, bool rotate) {
	// plain-text
	*p << (rotate ? _T("[") : _T("")) <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TileNonflowerMax) * StringElemSize, StringElemSize) <<
		(rotate ? _T("]") : _T(""));
	// hypertext
	if (tlCode.red) *h << _T("<span") << haifudoraClass(tlCode.red) << _T(">");
	*h << (rotate ? HTtilecodelabel2 : HTtilecodelabel1).substr(
		(int)tlCode.tile + (int)tlCode.red * TileNonflowerMax, 1);
	if (tlCode.red) *h << _T("</span>");
	// XML machine-readable
	XhaifuBufferBody << _T("<tile tile=\"") <<
		Xtilerefcode[(int)tlCode.tile + (int)tlCode.red * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(tlCode.red) << _T(" />");
}
void haifu::tools::recordTile_Inline(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, Tile tlCode, doraCol kakanCol) {
	// plain-text
	*p << _T("[") <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TileNonflowerMax) * StringElemSize, StringElemSize) <<
		tilecodelabel.substr(((int)tlCode.tile + (int)kakanCol * TileNonflowerMax) * StringElemSize, StringElemSize) <<
		_T("]");
	// hypertext
	*h << _T("<table class=\"kakan\"><tr><td>");
		if (kakanCol) *h << _T("<span") << haifudoraClass(kakanCol) << _T(">");
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)kakanCol * TileNonflowerMax, 1);
		if (kakanCol) *h << _T("</span>");
	*h << _T("</span>");
		if (tlCode.red) *h << _T("<span") << haifudoraClass(tlCode.red) << _T(">");
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)tlCode.red * TileNonflowerMax, 1);
		if (tlCode.red) *h << _T("</span>");
	*h << _T("</tr></td></table>");
	// XML machine-readable
	XhaifuBufferBody << _T("<tile tile=\"") <<
		Xtilerefcode[(int)tlCode.tile + (int)kakanCol * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(kakanCol) << _T(" /><tile tile=\"") <<
		Xtilerefcode[(int)tlCode.tile + (int)tlCode.red * TileNonflowerMax] << _T('\"') <<
		haifudoraClassX(tlCode.red) << _T(" />");
}
void haifu::tools::recordTile_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h, Tile tlCode, bool omitXml, CodeConv::tstring tagName, bool keepOpen) {
	// plain-text
	*p << tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TileNonflowerMax) * StringElemSize, StringElemSize) << _T(" ");
	// hypertext
	*h << _T("<td") << haifudoraClass(tlCode.red) << _T(">") <<
		HTtilecodelabel1.substr((int)tlCode.tile + (int)tlCode.red * TileNonflowerMax, 1) <<
		_T("</td>");
	// XML machine-readable
	if (!omitXml) {
		XhaifuBufferBody << _T('<') << tagName << _T(" tile=\"") <<
			Xtilerefcode[(int)tlCode.tile + (int)tlCode.red * TileNonflowerMax] << _T('\"') <<
			haifudoraClassX(tlCode.red);
		if (!keepOpen) XhaifuBufferBody << _T(" />") << std::endl;
	}
}

void haifu::tools::recordBlank_Table(CodeConv::tostringstream* const p, CodeConv::tostringstream* const h) {
	*p << _T("�@ ");
	*h << _T("<td></td>");
}

void haifu::tools::haifuwritetsumohai(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PlayerID ActivePlayer, Tile tlCode,
	CodeConv::tstring PText, CodeConv::tstring HTText, CodeConv::tstring XAttr
	) {
		checkCycle();
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)ActivePlayer << _T("\">") << std::endl;
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Table(
			&haifuP->streamDat[ActivePlayer].tsumo,
			&HThaifuP->streamDat[ActivePlayer].tsumo,
			tlCode, false, _T("pick"), true);
		haifuP->streamDat[ActivePlayer].tsumolabel << PText;
		HThaifuP->streamDat[ActivePlayer].tsumolabel << HTText;
		XhaifuBufferBody << XAttr << _T(" />") << std::endl;
}

void haifu::tools::haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PlayerID PassivePlayer) {
	static CodeConv::tostringstream* p[] = {
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehailabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehailabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumo,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumolabel,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehai,
		&haifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehailabel,
	};
	static CodeConv::tostringstream* h[] = {
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sRight)].sutehailabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sOpposite)].sutehailabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumo,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].tsumolabel,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehai,
		&HThaifuP->streamDat[RelativePositionOf(PassivePlayer, sLeft)].sutehailabel,
	};
	for (int i = 0; i < 12; i++) {
		*p[i] << _T("�@ "); *h[i] << _T("<td></td>");
	}
	checkCycle();
	XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
		(int)RelativePositionOf(PassivePlayer, sRight) << _T("\" />") << std::endl;
	checkCycle();
	XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
		(int)RelativePositionOf(PassivePlayer, sOpposite) << _T("\" />") << std::endl;
	checkCycle();
	XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") <<
		(int)RelativePositionOf(PassivePlayer, sLeft) << _T("\" />") << std::endl;
}

/* �ꔼ�����̔v���o�b�t�@�������� */
__declspec(dllexport) void haifu::haifubufinit() {
	CodeConv::tostringstream headerTxt; headerTxt.str(_T(""));
	headerTxt << _T("MiHaJong ");
	switch (GameStat.gameType) {
		case SanmaS: headerTxt << _T("���v�O��"); break;
		case Sanma4: headerTxt << _T("�l�l�O��"); break;
		case Sanma:  headerTxt << _T("�O�l�ł�"); break;
		case Yonma:  headerTxt << _T("�l�l�ł�"); break;
	}
	headerTxt << _T("�v���f�[�^ Ver. ") << MIHAJONG_VER;

	/* �v���[���e�L�X�g�v�� */
	haifuBuffer.str(_T(""));
	haifuBuffer << headerTxt.str() << std::endl << std::endl <<
		_T("------------------------------------------------------------------------------") <<
		std::endl << std::endl;

	/* HTML�v�� */
	HThaifuBuffer.str(_T(""));
	HThaifuBuffer <<  _T("<html>") << std::endl << _T("<head>") << std::endl <<
		_T("<title>") << headerTxt.str() << _T("</title>") << std::endl <<
#ifdef UNICODE
		_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">") <<
#else
		_T("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\">") <<
#endif
		std::endl <<
		_T("<link rel=\"stylesheet\" href=\"haifu.css\" type=\"text/css\">") << std::endl <<
		_T("</head>") << std::endl << _T("<body>") << std::endl <<
		_T("<h1>") << headerTxt.str() << _T("</h1>") << std::endl << _T("<hr>") << std::endl;

	/* XML�v�� */
	XMLhaifuBuffer.str(_T("")); XhaifuBuffer.str(_T(""));
	XMLhaifuBuffer <<
#ifdef UNICODE
		_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>") <<
#else
		_T("<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>") <<
#endif
		std::endl <<
		_T("<?xml-stylesheet href=\"haifu.xsl\" type=\"text/xsl\"?>") << std::endl <<
		_T("<!DOCTYPE haifu [") << std::endl <<
		_T("\t<!ENTITY east \"&#x1f000;\"><!ENTITY south \"&#x1f001;\"><!ENTITY west \"&#x1f002;\"><!ENTITY north \"&#x1f003;\">")
		_T("<!ENTITY red \"&#x1f004;\"><!ENTITY green \"&#x1f005;\"><!ENTITY white \"&#x1f006;\">") << std::endl <<
		_T("\t<!ENTITY m1 \"&#x1f007;\"><!ENTITY m2 \"&#x1f008;\"><!ENTITY m3 \"&#x1f009;\"><!ENTITY m4 \"&#x1f00a;\"><!ENTITY m5 \"&#x1f00b;\">")
		_T("<!ENTITY m6 \"&#x1f00c;\"><!ENTITY m7 \"&#x1f00d;\"><!ENTITY m8 \"&#x1f00e;\"><!ENTITY m9 \"&#x1f00f;\">") << std::endl <<
		_T("\t<!ENTITY s1 \"&#x1f010;\"><!ENTITY s2 \"&#x1f011;\"><!ENTITY s3 \"&#x1f012;\"><!ENTITY s4 \"&#x1f013;\"><!ENTITY s5 \"&#x1f014;\">")
		_T("<!ENTITY s6 \"&#x1f015;\"><!ENTITY s7 \"&#x1f016;\"><!ENTITY s8 \"&#x1f017;\"><!ENTITY s9 \"&#x1f018;\">") << std::endl <<
		_T("\t<!ENTITY p1 \"&#x1f019;\"><!ENTITY p2 \"&#x1f01a;\"><!ENTITY p3 \"&#x1f01b;\"><!ENTITY p4 \"&#x1f01c;\"><!ENTITY p5 \"&#x1f01d;\">")
		_T("<!ENTITY p6 \"&#x1f01e;\"><!ENTITY p7 \"&#x1f01f;\"><!ENTITY p8 \"&#x1f020;\"><!ENTITY p9 \"&#x1f021;\">") << std::endl <<
		_T("\t<!ENTITY plum \"&#x1f022;\"><!ENTITY orchid \"&#x1f023;\"><!ENTITY bamboo \"&#x1f024;\"><!ENTITY chrys \"&#x1f025;\"><!ENTITY chrysanthemum \"&#x1f025;\">")
		_T("<!ENTITY spring \"&#x1f026;\"><!ENTITY summer \"&#x1f027;\"><!ENTITY autumn \"&#x1f028;\"><!ENTITY winter \"&#x1f029;\">")
		_T("<!ENTITY joker \"&#x1f02a;\"><!ENTITY back \"&#x1f02b;\">") << std::endl <<
		_T("]>") << std::endl <<
		_T("<haifu xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance")
		_T("\" xsi:noNamespaceSchemaLocation=\"haifu.xsd\">") << std::endl <<
		_T("\t<match-description>") << std::endl <<
		_T("\t\t<title>") << headerTxt.str() << _T("</title>") << std::endl;
	tools::haifuRecTime(_T("time-began"));
	XhaifuBuffer << _T("\t\t<ruleset system=\"") <<
		(chkGameType(&GameStat, Yonma) ? _T("richi") :
		chkGameType(&GameStat, Sanma) ? _T("sanma") :
		chkGameType(&GameStat, Sanma4) ? _T("sanma-4players") :
		chkGameType(&GameStat, SanmaS) ? _T("sanma-numerals") : _T(""))
		<< _T("\">") << std::endl;
	RuleData::forEachRule([&](std::string key, std::string val) -> void {
		XhaifuBuffer << _T("\t\t\t<rule key=\"") << CodeConv::EnsureTStr(key) << _T("\" val=\"") <<
			CodeConv::EnsureTStr(val) << _T("\" />") << std::endl;
	});
	XhaifuBuffer << _T("\t\t</ruleset>") << std::endl;
	XhaifuBuffer << _T("\t\t<player-description>") << std::endl;
	const CodeConv::tstring nomenVenti[4] = {_T("east"), _T("south"), _T("west"), _T("north")};
	for (int i = 0; i < Players; ++i)
		XhaifuBuffer << _T("\t\t\t<player-data id=\"player") << i << _T("\" name=\"") <<
		EnvTable::Instantiate()->PlayerDat[i].PlayerName << _T("\" starting-wind=\"") <<
		nomenVenti[i] << _T("\" />") << std::endl;
	XhaifuBuffer <<
		_T("\t\t</player-description>") << std::endl <<
		_T("\t</match-description>") << std::endl;
}

void haifu::tools::haifuRecTime(CodeConv::tstring tagName) { // ���ݎ����^�O
	SYSTEMTIME currTime; GetLocalTime(&currTime);
	TIME_ZONE_INFORMATION tz; GetTimeZoneInformation(&tz);
	XMLhaifuBuffer << _T("\t\t<") << tagName << _T(">") <<
		std::setw(4) << std::setfill(_T('0')) << currTime.wYear << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wMonth << _T("-") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wDay << _T("T") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wHour << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wMinute << _T(":") <<
		std::setw(2) << std::setfill(_T('0')) << currTime.wSecond << _T(".") <<
		std::setw(3) << std::setfill(_T('0')) << currTime.wMilliseconds <<
		std::showpos << std::setw(3) << std::setfill(_T('0')) << std::internal << ((-tz.Bias) / 60) << _T(":") <<
		std::noshowpos << std::setw(2) << std::setfill(_T('0')) << ((-tz.Bias) % 60) <<
		_T("</") << tagName << _T(">") << std::endl;
}

/* ��Ǖ��̔v���o�b�t�@�������� */
__declspec(dllexport) void haifu::haifuinit() {
	static HaifuStreams* const bufs[] = {&haifuP, &HThaifuP, &XhaifuP};
	for (int i = 0; i < 3; i++) {
		for (int p = 0; p < Players; p++) {
			bufs[i]->streamDat[p].haipai.str(_T(""));
			bufs[i]->streamDat[p].tsumo.str(_T(""));
			bufs[i]->streamDat[p].tsumolabel.str(_T(""));
			bufs[i]->streamDat[p].sutehai.str(_T(""));
			bufs[i]->streamDat[p].sutehailabel.str(_T(""));
			bufs[i]->streamDat[p].final.str(_T(""));
		}
		bufs[i]->dora.str(_T(""));
		bufs[i]->uraDora.str(_T(""));
		bufs[i]->aliceDora.str(_T(""));
		bufs[i]->aliceDoraMax.str(_T(""));
		bufs[i]->resultDesc.str(_T(""));
	}
	XhaifuBufferBody.str(_T(""));
}

/* �z�v��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurechaipai(const GameTable* const gameStat) {
	XhaifuBufferBody << _T("\t\t<distribution>") << std::endl;
	for (int p = 0; p < Players; p++) {
		XhaifuBufferBody << _T("\t\t\t<initial-hand player=\"player") << p << _T("\">") << std::endl;
		for (int i = 0; i < NumOfTilesInHand; i++) {
			if (gameStat->Player[p].Hand[i].tile != NoTile) {
				XhaifuBufferBody << _T("\t\t\t\t");
				tools::recordTile_Inline(
					&haifuP.streamDat[p].haipai,
					&HThaifuP.streamDat[p].haipai,
					gameStat->Player[p].Hand[i],
					false
				);
				XhaifuBufferBody << std::endl;
			}
		}
		XhaifuBufferBody << _T("\t\t\t</initial-hand>") << std::endl;
		origPoint[p] = gameStat->Player[p].PlayerScore;
	}
	XhaifuBufferBody << _T("\t\t</distribution>") << std::endl;
	XhaifuBufferBody << _T("\t\t<progress>") << std::endl;
}

/* �h����v���ɋL�^ */
void haifu::haifurecdora(TileCode tmpDora) {
	tools::recordDoraStream(&haifuP.dora, &HThaifuP.dora, &XhaifuP.dora, tmpDora);
}
__declspec(dllexport) void haifu::haifurecdora(int tmpDora) {
	haifurecdora((TileCode)tmpDora);
}

/* ���h����v���ɋL�^ */
void haifu::haifurecuradora(TileCode tmpDora) {
	tools::recordDoraStream(&haifuP.uraDora, &HThaifuP.uraDora, &XhaifuP.uraDora, tmpDora);
}

/* �A���X�h����v���ɋL�^ */
void haifu::haifurecalicedora(TileCode tmpDora) {
	tools::recordDoraStream(&haifuP.aliceDora, &HThaifuP.aliceDora, &XhaifuP.aliceDora, tmpDora);
}

/* �A���X�h�����X�V */
__declspec(dllexport) void haifu::haifualicedoraupd() {
	if (haifuP.aliceDora.str().length() > haifuP.aliceDoraMax.str().length()) {
		haifuP.aliceDoraMax.str(haifuP.aliceDora.str());
		HThaifuP.aliceDoraMax.str(HThaifuP.aliceDora.str());
		XhaifuP.aliceDoraMax.str(XhaifuP.aliceDora.str());
	}
}

/* �A���X�h�������� */
void haifu::haifuresetalicedora() {
	haifuP.aliceDora.str(_T(""));
	HThaifuP.aliceDora.str(_T(""));
	XhaifuP.aliceDora.str(_T(""));
}

/* �h���̃Z�p���[�^�[ */
__declspec(dllexport) void haifu::haifurecdorap() {
	haifuP.dora << _T(" ");
	haifuP.uraDora << _T(" ");
	haifuP.aliceDora << _T(" ");
	HThaifuP.dora << _T("</span> <span class=\"tile\">");
	HThaifuP.uraDora << _T("</span> <span class=\"tile\">");
	HThaifuP.aliceDora << _T("</span> <span class=\"tile\">");
}

/* �̑ł�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecmota(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecmota(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	// �c�����Ă����v���L�^
	bool discard_through = false;
	if (gameStat->TianHuFlag) {
		// �e�̂P����
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo);
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
		tools::checkCycle(true);
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)gameStat->CurrentPlayer.Active << _T("\">") << std::endl;
	} else if (gameStat->statOfActive().Tsumohai().tile == NoTile) {
		// �������� (�������Ȃ�)
	} else if ((DiscardTileIndex.id) == (NumOfTilesInHand - 1)) {
		// �c���؂�
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << _T("�� ");
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << _T("<td class=\"fallthru\">��</td>");
		discard_through = true;
		tools::checkCycle();
		XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)gameStat->CurrentPlayer.Active << _T("\">") << std::endl;
	} else {
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
			gameStat->statOfActive().Tsumohai(),
			_T("�@ "), _T("<td></td>"), _T(""));
	}
	// �̂Ă��v���L�^
	XhaifuBufferBody << _T("\t\t\t\t\t");
	tools::recordTile_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		gameStat->statOfActive().Hand[DiscardTileIndex.id], false,
		discard_through ? _T("discard-through") : _T("discard"), true);
}

/* ���e�������ۂ���v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecfurikomi(const GameTable* const gameStat) {
	if (gameStat->statOfActive().RichiFlag.IppatsuFlag) {
		// �����錾�v�̏ꍇ
		if (RonPlayers(gameStat) > 0) {
			// �����錾�v�ł̐U�荞��
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�X ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>���[�`<br>�E�`</td>");
			XhaifuBufferBody << _T(" call=\"richi\" let-go=\"true\" />") << std::endl;
		} else {
			// �������ʂ����ꍇ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("ذ�");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>���[�`</td>");
			XhaifuBufferBody << _T(" call=\"richi\" />") << std::endl;
		}
	} else {
		// ����ȊO�̏ꍇ
		if (RonPlayers(gameStat) > 0) {
			// �ʏ�̐U�荞��
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�� ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td>�E�`</td>");
			XhaifuBufferBody << _T(" let-go=\"true\" />") << std::endl;
		} else {
			// �����Ȃ������ꍇ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("�@ ");
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << _T("<td></td>");
			XhaifuBufferBody << _T(" />") << std::endl;
		}
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}

/* �`�[�������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchi(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("�� "), _T("<td>�`�[</td>"), _T(" meld=\"sequence\""));
}
/* �|���������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecpon(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("���"), _T("<td>�|��</td>"), _T(" meld=\"triplet\""));
}
/* ���̘a�������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurectsumo(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
		gameStat->statOfActive().Tsumohai(),
		_T("�� "), _T("<td>�c��</td>"), _T(" meld=\"mahjong\""));
		XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}
/* �喾�Ȃ������Ƃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecminkan(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, _T("�� "), _T("<td>�J��</td>"), _T(" meld=\"quad\""));
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai);
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel);
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
	// �v���̋L�q��́A���v�̎擾�͎����Ƃ��Ĉ���
	tools::haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive);
}

/* ���ڋ�؂肩 */
void haifu::tools::checkCycle(bool reset) {
	static int cycle = 1, turn = 0;
	if (reset) {
		cycle = 1; turn = 0;
		XhaifuBufferBody << _T("\t\t\t<cycle ord=\"1\">") << std::endl;
	} else {
		if ((++turn) >= Players) {
			++cycle; turn = 0;
			XhaifuBufferBody << _T("\t\t\t</cycle>") << std::endl <<
				_T("\t\t\t<cycle ord=\"") << cycle << _T("\">") << std::endl;
		}
	}
}

/* �J������������ */
void haifu::tools::kan_sub::recordKanOrFlower(
	const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex,
	HaifuStreams* haifuP, HaifuStreams* HThaifuP
	) {
		if ((gameStat->TianHuFlag)||((DiscardTileIndex.id) != (NumOfTilesInHand - 1))) {
			// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
			if (gameStat->TianHuFlag) {
				checkCycle(true);
				XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)gameStat->CurrentPlayer.Active << _T("\">") << std::endl;
				XhaifuBufferBody << _T("\t\t\t\t\t");
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo);
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->statOfActive().Hand[DiscardTileIndex.id],
					false, _T("pick"), true);
				haifukanflag = true;
			} else if (gameStat->statOfActive().Tsumohai().tile ==
				gameStat->statOfActive().Hand[DiscardTileIndex.id].tile) {
					// �c�����Ă����v�Ɠ���������
					checkCycle();
					XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)gameStat->CurrentPlayer.Active << _T("\">") << std::endl;
					XhaifuBufferBody << _T("\t\t\t\t\t");
					recordTile_Table(
						&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						gameStat->statOfActive().Hand[DiscardTileIndex.id],
						false, _T("pick"), true);
					haifukanflag = false;
			} else {
				haifuwritetsumohai(
					haifuP, HThaifuP, gameStat->CurrentPlayer.Active,
					gameStat->statOfActive().Tsumohai(),
					_T("�@ "), _T("<td></td>"), _T(""));
				XhaifuBufferBody << _T("\t\t\t\t\t");
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->statOfActive().Hand[DiscardTileIndex.id],
					false, _T("discard"), true);
				haifukanflag = true;
			}
		} else {
			checkCycle();
			XhaifuBufferBody << _T("\t\t\t\t<turn player=\"player") << (int)gameStat->CurrentPlayer.Active << _T("\">") << std::endl;
			XhaifuBufferBody << _T("\t\t\t\t\t");
			recordTile_Table(
				&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				gameStat->statOfActive().Hand[DiscardTileIndex.id],
				false, _T("pick"), true);
			haifukanflag = false;
		}
}
/* ���g�k�h��v���ɋL�^ */
void inline haifu::tools::kan_sub::recordChanKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB) {
	if (haifukanflag) {
		// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
		XhaifuBufferBody << XAttrB << _T(" />") << std::endl;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
		XhaifuBufferBody << XAttrA << _T(" />") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
}
/* �����k�����������ꍇ�̔v������ */
void inline haifu::tools::kan_sub::recordKan(const GameTable* const gameStat, CodeConv::tstring pTxt, CodeConv::tstring hTxt, CodeConv::tstring XAttrA, CodeConv::tstring XAttrB) {
	if (haifukanflag == 1) {
		// �e�̂P���ڂ̏ꍇ���A�c�����Ă����v�ȊO���J�������ꍇ
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
		XhaifuBufferBody << XAttrB << _T(" />") << std::endl;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai);
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel);
		XhaifuBufferBody << XAttrA << _T(" />") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</turn>") << std::endl;
		
	// �v���̋L�q��́A���v�̎擾�͎����Ƃ��Ĉ���
	haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active);
}

/* �����k��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecnorth(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* ���g�k�h��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchanpei(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T("��X"), _T("<td>��<br>�E�`</td>"),
		_T(" meld=\"flower\" let-go=\"true\""), _T(" call=\"flower\" let-go=\"true\""));
}
/* �����k�����������ꍇ�̔v������ */
__declspec(dllexport) void haifu::haifurecnorthproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>��</td>"),
		_T(" meld=\"flower\""), _T(" call=\"flower\""));
}

/* �ÞȂȂ������Ȃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecankan(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecankan(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* ���Ȃ�v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecchankan(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, _T("��X"), _T("<td>�J��<br>�E�`</td>"),
		_T(" meld=\"quad\" let-go=\"true\""), _T(" call=\"quad\" let-go=\"true\""));
}
/* �J�������������ꍇ�̔v������ */
__declspec(dllexport) void haifu::haifureckanproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>�J��</td>"),
		_T(" meld=\"quad\""), _T(" call=\"quad\""));
}

/* �Ԕv��v���ɋL�^ */
__declspec(dllexport) void haifu::haifurecflower(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecflower(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
	tools::kan_sub::recordKan(gameStat, _T("�� "), _T("<td>��</td>"),
		_T(" meld=\"flower\""), _T(" call=\"flower\""));
}

/* hfwriter */
void haifu::tools::hfwriter::hfWriteHead(const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	CodeConv::tstring ResultDesc, EndType RoundEndType) {
		haifuBuffer << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) haifuBuffer << _T(" ") << OrigHonba << _T("�{��");
		haifuBuffer << _T(" �h���F") << haifuP.dora.str();
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			haifuBuffer << _T("���h���F") << haifuP.uraDora.str();
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			haifuBuffer << _T("�A���X�F") << haifuP.aliceDoraMax.str();
		haifuBuffer << std::endl << std::endl <<
			_T("���ʁF") << ResultDesc << std::endl << std::endl;

		HThaifuBuffer << _T("<h2>") << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) HThaifuBuffer << _T(" ") << OrigHonba <<_T("�{��");
		HThaifuBuffer << _T(" �h���F<span class=\"tile\">") <<
			HThaifuP.dora.str() << _T("</span>");
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			HThaifuBuffer << _T("���h���F<span class=\"tile\">") <<
			haifuP.uraDora.str() << _T("</span>"); /* FIXME: �ϐ��Ԉ���Ă�̂Ō�Œ������� */
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			HThaifuBuffer << _T("�A���X�F<span class=\"tile\">") <<
			haifuP.aliceDoraMax.str() << _T("</span>"); /* FIXME: �ϐ��Ԉ���Ă�̂Ō�Œ������� */
		HThaifuBuffer << _T("</h2>") << std::endl <<
			_T("<p>���ʁF") << ResultDesc << _T("</p>") << std::endl <<
			_T("<table>") << std::endl << _T("<tr>");
		for (unsigned int i = 0u; i < cols; i++)
			HThaifuBuffer << _T("<td width=") << (100.0/((double)cols)) << _T("%></td>");
		HThaifuBuffer << _T("</tr>") << std::endl;

		XhaifuBuffer << _T("\t<round>") << std::endl << _T("\t\t<round-description>") << std::endl;
		const CodeConv::tstring windName[7] = {
			_T("east"), _T("south"), _T("west"), _T("north"), _T("white"), _T("green"), _T("red"),
		};
		const bool eastOnly = RuleData::chkRule("game_length", "twice_east_game") || RuleData::chkRule("game_length", "east_only_game");
		XhaifuBuffer << _T("\t\t\t<round-number prevailing-wind=\"") << windName[OrigTurn / Players] <<
			_T("\" rotation=\"") << ((eastOnly ? gameStat->LoopRound : 0) * Players + (OrigTurn % Players) + 1) <<
			_T("\" counter=\"") << OrigHonba << _T("\" />") << std::endl;
		XhaifuBuffer << _T("\t\t\t<result>") << ResultDesc << _T("</result>") << std::endl;
		XhaifuBuffer << _T("\t\t\t<dora>") << std::endl <<
			XhaifuP.dora.str() << _T("\t\t\t</dora>") << std::endl;
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			XhaifuBuffer << _T("\t\t\t<uradora>") << std::endl <<
			XhaifuP.uraDora.str() << _T("\t\t\t</uradora>") << std::endl;
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			XhaifuBuffer << _T("\t\t\t<alice>") << std::endl <<
			XhaifuP.aliceDoraMax.str() << _T("\t\t\t</alice>") << std::endl;
}

// �ŏI�v�p
void haifu::tools::hfwriter::finalformWriter::hfFinalForm(const GameTable* const gameStat, PlayerID player, EndType RoundEndType) {
	// �ŏI�v�p(����v�̂�)
	bool agariFlag = false;
	XhaifuBufferBody << _T("\t\t\t\t<hand>") << std::endl;
	for (int i = 0; i < NumOfTilesInHand; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			if (i == NumOfTilesInHand - 1) {
				if ((RoundEndType == Ryuukyoku)||(RoundEndType == Agari)||(RoundEndType == Chonbo)) {
					if (gameStat->TsumoAgariFlag) {
						haifuP.streamDat[player].final << _T(" �c��");
						HThaifuP.streamDat[player].final << _T("</span> �c��<span class=\"tile\">");
						XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
						XhaifuBufferBody << _T("\t\t\t\t<finishing-tile finish-type=\"tsumo\">") << std::endl;
					} else {
						haifuP.streamDat[player].final << _T(" ����");
						HThaifuP.streamDat[player].final << _T("</span> ����<span class=\"tile\">");
						XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
						XhaifuBufferBody << _T("\t\t\t\t<finishing-tile finish-type=\"ron\">") << std::endl;
					}
					agariFlag = true;
				}
				else haifuP.streamDat[player].final << _T(" ");
			}
			XhaifuBufferBody << _T("\t\t\t\t\t");
			recordTile_Inline(
				&haifuP.streamDat[player].final, &HThaifuP.streamDat[player].final,
				gameStat->Player[player].Hand[i], false);
			XhaifuBufferBody << std::endl;
		}
	}
	if (agariFlag)
		XhaifuBufferBody << _T("\t\t\t\t</finishing-tile>") << std::endl;
	else
		XhaifuBufferBody << _T("\t\t\t\t</hand>") << std::endl;
}
void haifu::tools::hfwriter::finalformWriter::hfFlower(const GameTable* const gameStat, PlayerID player) {
	if (gameStat->Player[player].FlowerFlag.Spring || gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn || gameStat->Player[player].FlowerFlag.Winter ||
		gameStat->Player[player].FlowerFlag.Plum || gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys || gameStat->Player[player].FlowerFlag.Bamboo)
		XhaifuBufferBody << _T("\t\t\t\t<flower>") << std::endl;
	if (gameStat->Player[player].FlowerFlag.Spring ||
		gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn ||
		gameStat->Player[player].FlowerFlag.Winter) {
			haifuP.streamDat[player].final << _T(" ");
			HThaifuP.streamDat[player].final << _T("</span> <span class=\"tile\">");
			if (gameStat->Player[player].FlowerFlag.Spring) {
				haifuP.streamDat[player].final << _T("�t");
				HThaifuP.streamDat[player].final << _T("@");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&spring;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Summer) {
				haifuP.streamDat[player].final << _T("��");
				HThaifuP.streamDat[player].final << _T(";");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&summer;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Autumn) {
				haifuP.streamDat[player].final << _T("�H");
				HThaifuP.streamDat[player].final << _T(":");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&autumn;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Winter) {
				haifuP.streamDat[player].final << _T("�~");
				HThaifuP.streamDat[player].final << _T("]");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&winter;\" />") << std::endl;
			}
	}
	if (gameStat->Player[player].FlowerFlag.Plum ||
		gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys ||
		gameStat->Player[player].FlowerFlag.Bamboo) {
			haifuP.streamDat[player].final << _T(" ");
			HThaifuP.streamDat[player].final << _T("</span> <span class=\"tile\">");
			if (gameStat->Player[player].FlowerFlag.Plum) {
				haifuP.streamDat[player].final << _T("�~");
				HThaifuP.streamDat[player].final << _T("-");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&plum;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Orchid) {
				haifuP.streamDat[player].final << _T("��");
				HThaifuP.streamDat[player].final << _T("^");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&orchid;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Chrys) {
				haifuP.streamDat[player].final << _T("�e");
				HThaifuP.streamDat[player].final << _T("[");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&chrys;\" />") << std::endl;
			}
			if (gameStat->Player[player].FlowerFlag.Bamboo) {
				haifuP.streamDat[player].final << _T("�|");
				HThaifuP.streamDat[player].final << _T("\\");
				XhaifuBufferBody << _T("\t\t\t\t\t<tile tile=\"&bamboo;\" />") << std::endl;
			}
	}
	if (gameStat->Player[player].FlowerFlag.Spring || gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn || gameStat->Player[player].FlowerFlag.Winter ||
		gameStat->Player[player].FlowerFlag.Plum || gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys || gameStat->Player[player].FlowerFlag.Bamboo)
		XhaifuBufferBody << _T("\t\t\t\t</flower>") << std::endl;
}

void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfChii(PlayerID player, MeldCode meld) {
	// �`�[
	haifuP.streamDat[player].final << _T(" ���`�[");
	HThaifuP.streamDat[player].final << _T("</span> �`�[<span class=\"tile\">");
	XhaifuBufferBody << _T("\t\t\t\t<sequence>") << std::endl;
	Tile meldTile[3];
	switch (meld.mstat) {
	case meldSequenceExposedLower:
		meldTile[0].tile = meld.tile; meldTile[0].red = meld.red[0];
		meldTile[1].tile = TileCode(meld.tile + 1); meldTile[1].red = meld.red[1];
		meldTile[2].tile = TileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
		break;
	case meldSequenceExposedMiddle:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[0].tile = TileCode(meld.tile + 1); meldTile[0].red = meld.red[1];
		meldTile[2].tile = TileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
	case meldSequenceExposedUpper:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[2].tile = TileCode(meld.tile + 1); meldTile[2].red = meld.red[1];
		meldTile[0].tile = TileCode(meld.tile + 2); meldTile[0].red = meld.red[2];
	}
	for (int i = 0; i < 3; i++) {
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile[i], i == 0);
		XhaifuBufferBody << std::endl;
	}
	XhaifuBufferBody << _T("\t\t\t\t</sequence>") << std::endl;
}
inline void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon1(PlayerID player, MeldCode meld) {
	Tile meldTile = {meld.tile, meld.red[0]};
	XhaifuBufferBody << _T("\t\t\t\t\t");
	if ((meld.mstat == meldQuadAddedLeft) ||
		(meld.mstat == meldQuadAddedCenter) ||
		(meld.mstat == meldQuadAddedRight))
			recordTile_Inline(
				&haifuP.streamDat[player].final,
				&HThaifuP.streamDat[player].final,
				meldTile, meld.red[3]);
	else recordTile_Inline(
		&haifuP.streamDat[player].final,
		&HThaifuP.streamDat[player].final,
		meldTile, true);
	XhaifuBufferBody << std::endl;
}
void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon(PlayerID player, MeldCode meld) {
	int tiles, interrupt; CodeConv::tstring meldDirection;
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		meldDirection = _T(" from=\"left\"");
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 1; break;
	case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		meldDirection = _T(" from=\"opposite\"");
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 2; break;
	case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		meldDirection = _T(" from=\"right\"");
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 8; break;
	case meldQuadConcealed:
		haifuP.streamDat[player].final << _T(" ��"); interrupt = 7; break;
	}
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		tiles = 3;
		haifuP.streamDat[player].final << _T("�|��");
		HThaifuP.streamDat[player].final << _T("</span> �|��<span class=\"tile\">");
		XhaifuBufferBody << _T("\t\t\t\t<triplet") << meldDirection << _T('>') << std::endl;
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter:
	case meldQuadExposedRight:
		XhaifuBufferBody << _T("\t\t\t\t<quad-exposed") << meldDirection << _T(" added=\"false\">") << std::endl;
		goto quad_4tiles;
	case meldQuadConcealed:
		XhaifuBufferBody << _T("\t\t\t\t<quad-concealed>") << std::endl;
		goto quad_4tiles;
	quad_4tiles:
		tiles = 4;
		haifuP.streamDat[player].final << _T("�J��");
		HThaifuP.streamDat[player].final << _T("</span> �J��<span class=\"tile\">");
		break;
	case meldQuadAddedLeft: case meldQuadAddedCenter:
	case meldQuadAddedRight:
		tiles = 3;
		haifuP.streamDat[player].final << _T("���J��");
		HThaifuP.streamDat[player].final << _T("</span> �J��<span class=\"tile\">");
		XhaifuBufferBody << _T("\t\t\t\t<quad-exposed") << meldDirection << _T(" added=\"true\">") << std::endl;
		break;
	}
	for (int i = (meld.mstat == meldQuadConcealed ? 0 : 1); i < tiles; i++) {
		if (i == interrupt) hfPon1(player, meld);
		Tile meldTile = {meld.tile, meld.red[i]};
		XhaifuBufferBody << _T("\t\t\t\t\t");
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile, false);
		XhaifuBufferBody << std::endl;
	}
	if (interrupt == 8) hfPon1(player, meld);
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		XhaifuBufferBody << _T("\t\t\t\t</triplet>") << std::endl;
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter: case meldQuadExposedRight:
	case meldQuadAddedLeft: case meldQuadAddedCenter: case meldQuadAddedRight:
		XhaifuBufferBody << _T("\t\t\t\t</quad-exposed>") << std::endl;
		break;
	case meldQuadConcealed:
		XhaifuBufferBody << _T("\t\t\t\t</quad-concealed>") << std::endl;
		break;
	}
}

void haifu::tools::hfwriter::finalformWriter::hfExposedMeld(const GameTable* const gameStat, PlayerID player) {
	for (int i = 1; i <= gameStat->Player[player].MeldPointer; i++) {
		switch (gameStat->Player[player].Meld[i].mstat) {
		case meldSequenceExposedLower: case meldSequenceExposedMiddle:
		case meldSequenceExposedUpper:
			MeldWriter::hfChii(player, gameStat->Player[player].Meld[i]);
			break;
		case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		case meldQuadConcealed:
			MeldWriter::hfPon(player, gameStat->Player[player].Meld[i]);
			break;
		}
	}
}

void haifu::tools::hfwriter::hfScoreWriteOut(const GameTable* const gameStat, PlayerID player, seatAbsolute wind) {
	// �_���̕ϓ�
	CodeConv::tostringstream o;
	o << _T(" ") << origPoint[player].to_str(_T(""), _T("��"));
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // �_������v���Ȃ��Ȃ�
		o << _T(" �� ") <<
			gameStat->Player[player].PlayerScore.bignumtotext(_T(""), _T("��")) << _T(" (") <<
			((LNum)gameStat->Player[player].PlayerScore -
			origPoint[player]).to_str(_T("+"), _T("-")) <<
			_T(")");
	if (RuleData::chkRuleApplied("chip")) // �`�b�v����̎�
		o << _T(" �`�b�v: ") <<
			((gameStat->Player[player].playerChip >= 0) ? _T("+") : _T("")) <<
			(int)gameStat->Player[player].playerChip;

	{
		CodeConv::tostringstream p;
		p << _T("*** ") << EnvTable::Instantiate()->PlayerDat[player].PlayerName << _T("(") <<
			windName(wind) << _T(") ") << o.str();
		chat::chatobj->sysmsg(p.str());
	}
					
	// �o��
	haifuBuffer << windName(wind) << _T(" ") <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << std::endl;
	HThaifuBuffer << _T("<tr><td colspan=") << cols << _T(" class=\"player\">") <<
		windName(wind) << _T(" ") <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << _T("</td></tr>") << std::endl;

	// XML�p�o��
	const CodeConv::tstring nomDeVent[4] = {_T("east"), _T("south"), _T("west"), _T("north")};
	XhaifuBuffer << _T("\t\t\t\t<player ref=\"player") << (int)player << _T("\" wind=\"") <<
		nomDeVent[wind] << _T("\" score=\"")
		<< origPoint[player].to_str_plain() << _T('"');
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // �_������v���Ȃ��Ȃ�
		XhaifuBuffer << _T(" score-after=\"") <<
			gameStat->Player[player].PlayerScore.bignumtoplaintext() << _T("\" delta=\"") <<
			((LNum)gameStat->Player[player].PlayerScore -
			origPoint[player]).to_str_plain() << _T('"');
	if (RuleData::chkRuleApplied("chip")) // �`�b�v����̎�
		XhaifuBuffer << _T(" chip=\"") <<
			(int)gameStat->Player[player].playerChip << _T('"');
	XhaifuBuffer << _T(" />") << std::endl;
}

void haifu::tools::hfwriter::hfWriteOut(const GameTable* const gameStat, PlayerID player) {
	haifuBuffer << _T("�z�v�F") << haifuP.streamDat[player].haipai.str() <<
		std::endl << std::endl;
	HThaifuBuffer << _T("<tr><td class=\"label\">�z�v</td><td colspan=") << (cols - 1) <<
		_T("><span class=\"tile\">") << HThaifuP.streamDat[player].haipai.str() <<
		_T("</span></td></tr>") << std::endl << std::endl;
	if (!gameStat->TianHuFlag) { // �V�a(�܂��͐e�̏\�O�s�������v)�̏ꍇ�͏ȗ�
		haifuBuffer <<
			_T("���́F") << haifuP.streamDat[player].tsumo.str() << std::endl <<
			_T("�@�@�@") << haifuP.streamDat[player].tsumolabel.str() << std::endl <<
			_T("�Ŕv�F") << haifuP.streamDat[player].sutehai.str() << std::endl <<
			_T("�@�@�@") << haifuP.streamDat[player].sutehailabel.str() << std::endl <<
			_T("�v�p�F") << haifuP.streamDat[player].final.str() << std::endl <<
			std::endl;
		HThaifuBuffer <<
			_T("<tr class=\"tile\"><td class=\"label\" rowspan=2>����</td>") <<
			HThaifuP.streamDat[player].tsumo.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"notice\">") <<
			HThaifuP.streamDat[player].tsumolabel.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"tile\"><td class=\"label\" rowspan=2>�Ŕv</td>") <<
			HThaifuP.streamDat[player].sutehai.str() << _T("</tr>") << std::endl <<
			_T("<tr class=\"notice\">") <<
			HThaifuP.streamDat[player].sutehailabel.str() << _T("</tr>") << std::endl <<
			_T("<tr><td class=\"label\">�v�p</td><td colspan=") << (cols - 1) <<
			_T(" class=\"hand\"><span class=\"tile\">") <<
			HThaifuP.streamDat[player].final.str() << _T("</span></td></tr>") << std::endl <<
			std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType) {
	XhaifuBufferBody << _T("\t\t\t</cycle>") << std::endl;
	XhaifuBufferBody << _T("\t\t</progress>") << std::endl;
	XhaifuBufferBody << _T("\t\t<final-hands>") << std::endl;
	XhaifuBuffer << _T("\t\t\t<player-score>") << std::endl;
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		XhaifuBufferBody << _T("\t\t\t<final-hand player=\"player") << i << _T("\">") << std::endl;
		PlayerID k = RelativePositionOf(i, OrigTurn % Players);
		if (chkGameType(gameStat, SanmaT))
			if (((OrigTurn % Players) + i) >= ACTUAL_PLAYERS)
				k = (k + 1) % Players;
		// ���I�ʎq���o�͂���
		finalformWriter::hfFinalForm(gameStat, k, RoundEndType);
		finalformWriter::hfFlower(gameStat, k);
		finalformWriter::hfExposedMeld(gameStat, k);
		// �_�_�󋵂������o��
		hfScoreWriteOut(gameStat, k, (seatAbsolute)i);
		// �F�X�����o��
		if ((!chkGameType(gameStat, Sanma4))||(i < 3))
			hfWriteOut(gameStat, k);
		else haifuBuffer << std::endl;
		XhaifuBufferBody << _T("\t\t\t</final-hand>") << std::endl;
	}
	XhaifuBufferBody << _T("\t\t</final-hands>") << std::endl;
	XhaifuBuffer << _T("\t\t\t</player-score>") << std::endl;
}

void haifu::tools::hfwriter::hfWriteBottom() {
	haifuBuffer <<
		_T("------------------------------------------------------------------------------") <<
		std::endl << std::endl;
	HThaifuBuffer << _T("</table>") << std::endl << _T("<hr>") << std::endl;
	XhaifuBuffer << _T("\t\t</round-description>") << std::endl <<
		XhaifuBufferBody.str() << _T("\t</round>") << std::endl;
}

/* �z�v���o�b�t�@�ɏo�� */
__declspec(dllexport) void haifu::haifuwritebuffer(
	const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	LPCTSTR ResultDesc, EndType RoundEndType
	) { /* �z�v���o�b�t�@�ɏo�� */
		tools::hfwriter::hfWriteHead(
			gameStat, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag,
			ResultDesc, RoundEndType);
		tools::hfwriter::hfWriteFinalForms(
			gameStat, OrigTurn, RoundEndType);
		tools::hfwriter::hfWriteBottom();
}

/* �v����ۑ� */
void haifu::haifusave(const GameTable* const gameStat) {
	HThaifuBuffer << _T("</body>") << std::endl << _T("</html>") << std::endl; // Finalize HTML
	XhaifuBuffer << _T("</haifu>") << std::endl; // Finalize XML

	tools::haifuRecTime(_T("time-finished"));
	XMLhaifuBuffer << XhaifuBuffer.str();

	/* �t�@�C�����̎������� */
	std::string configPath = confpath::confPath();
	std::ostringstream filename1, filename2;
	filename1 << configPath << "haifu\\";
	switch (gameStat->gameType) {
		case Yonma: filename1 << "mihajong"; break;
		case Sanma: filename1 << "mihasanm"; break;
		case Sanma4: filename1 << "mihaysnm"; break;
		case SanmaS: filename1 << "mihassnm"; break;
	}
	filename1 << "_" << MIHAJONG_MAJOR_VER << "_" <<
		MIHAJONG_MINOR_VER << "_" << MIHAJONG_PATCH_VER;

	SYSTEMTIME ltime; GetLocalTime(&ltime);
	filename2 << std::setw(4) << std::setfill('0') << ltime.wYear;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMonth;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wDay << "_";
	filename2 << std::setw(2) << std::setfill('0') << ltime.wHour;
	filename2 << std::setw(2) << std::setfill('0') << ltime.wMinute;

	/* �t�@�C�������o�� */
	std::ofstream fileout;
	fileout.open((filename1.str() + std::string("_haifu_") + // �v���[���e�L�X�g�`���v��
		filename2.str() + std::string(".txt")).c_str());
	fileout << CodeConv::EncodeStr(haifuBuffer.str()); fileout.close();
	fileout.open((filename1.str() + std::string("_chat_") + // �`���b�g���O
		filename2.str() + std::string(".txt")).c_str());
	fileout << CodeConv::EncodeStr(chat::chatobj->getlog()); fileout.close();
	fileout.open((filename1.str() + std::string("_haifu_") + // HTML�`���v��
		filename2.str() + std::string(".htm")).c_str());
	fileout << CodeConv::EncodeStr(HThaifuBuffer.str()); fileout.close();
	fileout.open((filename1.str() + std::string("_haifu_") + // XML�`���v��
		filename2.str() + std::string(".xml")).c_str());
	fileout << CodeConv::EncodeStr(XMLhaifuBuffer.str()); fileout.close();
}
__declspec(dllexport) void haifu::haifusave() {
	haifusave(&GameStat);
}
