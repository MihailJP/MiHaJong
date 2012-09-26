#include "haifu.h"

/* ”v‚Ì–¼‘Oƒf[ƒ^ */
const std::string haifu::tilecodelabel =
	"Hˆê“ñOlŒÜ˜Zµ”ª‹ãH‡@‡A‡B‡C‡D‡E‡F‡G‡HH‚P‚Q‚R‚S‚T‚U‚V‚W‚XH“Œ“ì¼–k”’á¢’†HHHˆë“óQãæŒŞ—¤½J‹èH‡J‡K‡L‡M‡N‡O‡P‡Q‡RH‡T‡U‡V‡W‡X‡Y‡Z‡[‡\H‚d‚r‚v‚m‚o‚e‚bHHHb‰³•¸’š•èŒÈMhpH‚¢‚ë‚Í‚É‚Ù‚Ö‚Æ‚¿‚èHú@úAúBúCúDúEúFúGúHH‚…‚“‚—‚‚‚†‚ƒHHHt‰ÄH“~•S”~—–‹e’|H";
const std::string haifu::HTtilecodelabel1 =
	" qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   qwertyuio zxcvbnm,. asdfghjkl 1234567   -^\\[5@;:]";
const std::string haifu::HTtilecodelabel2 =
	" QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   QWERTYUIO ZXCVBNM<> ASDFGHJKL !\"#$%&'   -^\\[%@;:]";

InfoByPlayer<LargeNum> haifu::origPoint;
std::ostringstream haifu::haifuBuffer, haifu::HThaifuBuffer;
bool haifu::haifukanflag = false;

haifu::HaifuStreams haifu::haifuP, haifu::HThaifuP;

/* ”v•ˆ‹L˜^—p‚Ì•â•ƒ‹[ƒ`ƒ“ */
void haifu::tools::haifuskip(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PLAYER_ID PassivePlayer, PLAYER_ID ActivePlayer
	) {
		static std::ostringstream* p[] = {
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sRight)].sutehailabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumo,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].tsumolabel,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehai,
			&haifuP->streamDat[RelativePositionOf(ActivePlayer, sOpposite)].sutehailabel,
		};
		static std::ostringstream* h[] = {
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
			// ‘Î–Ê‚©‚çƒ|ƒ“‚µ‚½ê‡
			for (int i = 0; i < 4; i++) {
				*p[i] << "@ "; *h[i] << "<td></td>";
			}
		}
		if (playerRelative(ActivePlayer, PassivePlayer) == sRight) {
			// ‰º‰Æ‚©‚çƒ|ƒ“‚µ‚½ê‡
			for (int i = 0; i < 8; i++) {
				*p[i] << "@ "; *h[i] << "<td></td>";
			}
		}
}

std::string haifu::tools::haifudoraClass(doraCol Akadora) {
	switch (Akadora) {
	case AkaDora:
		return " class=\"akadora\"";
	case AoDora:
		return " class=\"aodora\"";
	default:
		return "";
	}
}

void haifu::tools::recordDoraStream(std::ostringstream* const p, std::ostringstream* const h, tileCode tmpDora) {
	*p << tilecodelabel.substr((int)tmpDora * 2, 2);
	*h << HTtilecodelabel1.substr((int)tmpDora, 1);
}

void haifu::tools::recordTile_Inline(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode, bool rotate) {
	*p << (rotate ? "[" : "") <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) <<
		(rotate ? "]" : "");
	if (tlCode.red) *h << "<span" << haifudoraClass(tlCode.red) << ">";
	*h << (rotate ? HTtilecodelabel2 : HTtilecodelabel1).substr(
		(int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1);
	if (tlCode.red) *h << "</span>";
}
void haifu::tools::recordTile_Inline(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode, doraCol kakanCol) {
	*p << "[" <<
		tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) <<
		tilecodelabel.substr(((int)tlCode.tile + (int)kakanCol * TILE_NONFLOWER_MAX) * 2, 2) <<
		"]";
	*h << "<table class=\"kakan\"><tr><td>";
		if (kakanCol) *h << "<span" << haifudoraClass(kakanCol) << ">";
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)kakanCol * TILE_NONFLOWER_MAX, 1);
		if (kakanCol) *h << "</span>";
	*h << "</span>";
		if (tlCode.red) *h << "<span" << haifudoraClass(tlCode.red) << ">";
		*h << HTtilecodelabel2.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1);
		if (tlCode.red) *h << "</span>";
	*h << "</tr></td></table>";
}
void haifu::tools::recordTile_Table(std::ostringstream* const p, std::ostringstream* const h, TILE tlCode) {
	*p << tilecodelabel.substr(((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX) * 2, 2) << " ";
	*h << "<td" << haifudoraClass(tlCode.red) << ">" <<
		HTtilecodelabel1.substr((int)tlCode.tile + (int)tlCode.red * TILE_NONFLOWER_MAX, 1) <<
		"</td>";
}

void haifu::tools::recordBlank_Table(std::ostringstream* const p, std::ostringstream* const h) {
	*p << "@ ";
	*h << "<td></td>";
}

void haifu::tools::haifuwritetsumohai(
	HaifuStreams* haifuP, HaifuStreams* HThaifuP,
	PLAYER_ID ActivePlayer, TILE tlCode,
	std::string PText, std::string HTText
	) {
		recordTile_Table(
			&haifuP->streamDat[ActivePlayer].tsumo,
			&HThaifuP->streamDat[ActivePlayer].tsumo,
			tlCode);
		haifuP->streamDat[ActivePlayer].tsumolabel << PText;
		HThaifuP->streamDat[ActivePlayer].tsumolabel << HTText;
}

void haifu::tools::haifuskipall(HaifuStreams* haifuP, HaifuStreams* HThaifuP, PLAYER_ID PassivePlayer) {
	static std::ostringstream* p[] = {
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
	static std::ostringstream* h[] = {
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
		*p[i] << "@ "; *h[i] << "<td></td>";
	}
}

/* ˆê”¼‘‘•ª‚Ì”v•ˆƒoƒbƒtƒ@‚ğ‰Šú‰» */
__declspec(dllexport) void haifu::haifubufinit() {
	std::ostringstream headerTxt; headerTxt.str("");
	headerTxt << "MiHaJong ";
	switch (GameStat.gameType) {
		case SanmaS: headerTxt << "””vO–ƒ"; break;
		case Sanma4: headerTxt << "llO–ƒ"; break;
		case Sanma:  headerTxt << "Ol‘Å‚¿"; break;
		case Yonma:  headerTxt << "ll‘Å‚¿"; break;
	}
	headerTxt << "”v•ˆƒf[ƒ^ Ver. " << MIHAJONG_VER;

	/* ƒvƒŒ[ƒ“ƒeƒLƒXƒg”v•ˆ */
	haifuBuffer.str("");
	haifuBuffer << headerTxt.str() << std::endl << std::endl <<
		"------------------------------------------------------------------------------" <<
		std::endl << std::endl;

	/* HTML”v•ˆ */
	HThaifuBuffer.str("");
	HThaifuBuffer <<  "<html>" << std::endl << "<head>" << std::endl <<
		"<title>" << headerTxt.str() << "</title>" << std::endl <<
		"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=Shift_JIS\">" << std::endl <<
		"<link rel=\"stylesheet\" href=\"haifu.css\" type=\"text/css\">" << std::endl <<
		"</head>" << std::endl << "<body>" << std::endl <<
		"<h1>" << headerTxt.str() << "</h1>" << std::endl << "<hr>" << std::endl;
}

/* ˆê‹Ç•ª‚Ì”v•ˆƒoƒbƒtƒ@‚ğ‰Šú‰» */
__declspec(dllexport) void haifu::haifuinit() {
	static HaifuStreams* const bufs[] = {&haifuP, &HThaifuP};
	for (int i = 0; i < 2; i++) {
		for (int p = 0; p < PLAYERS; p++) {
			bufs[i]->streamDat[p].haipai.str("");
			bufs[i]->streamDat[p].tsumo.str("");
			bufs[i]->streamDat[p].tsumolabel.str("");
			bufs[i]->streamDat[p].sutehai.str("");
			bufs[i]->streamDat[p].sutehailabel.str("");
			bufs[i]->streamDat[p].final.str("");
		}
		bufs[i]->dora.str("");
		bufs[i]->uraDora.str("");
		bufs[i]->aliceDora.str("");
		bufs[i]->aliceDoraMax.str("");
		bufs[i]->resultDesc.str("");
	}
}

/* ”z”v‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurechaipai(const GameTable* const gameStat) {
	for (int p = 0; p < PLAYERS; p++) {
		for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
			if (gameStat->Player[p].Hand[i].tile != NoTile) {
				tools::recordTile_Inline(
					&haifuP.streamDat[p].haipai,
					&HThaifuP.streamDat[p].haipai,
					gameStat->Player[p].Hand[i],
					false
				);
			}
		}
		origPoint[p] = gameStat->Player[p].PlayerScore;
	}
}

/* ƒhƒ‰‚ğ”v•ˆ‚É‹L˜^ */
void haifu::haifurecdora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.dora, &HThaifuP.dora, tmpDora);
}
__declspec(dllexport) void haifu::haifurecdora(int tmpDora) {
	haifurecdora((tileCode)tmpDora);
}

/* — ƒhƒ‰‚ğ”v•ˆ‚É‹L˜^ */
void haifu::haifurecuradora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.uraDora, &HThaifuP.uraDora, tmpDora);
}
__declspec(dllexport) void haifu::haifurecuradora(int tmpDora) {
	haifurecdora((tileCode)tmpDora);
}

/* ƒAƒŠƒXƒhƒ‰‚ğ”v•ˆ‚É‹L˜^ */
void haifu::haifurecalicedora(tileCode tmpDora) {
	tools::recordDoraStream(&haifuP.aliceDora, &HThaifuP.aliceDora, tmpDora);
}
__declspec(dllexport) void haifu::haifurecalicedora(int tmpDora) {
	haifurecalicedora((tileCode)tmpDora);
}

/* ƒAƒŠƒXƒhƒ‰‚ğXV */
__declspec(dllexport) void haifu::haifualicedoraupd() {
	if (haifuP.aliceDora.str().length() > haifuP.aliceDoraMax.str().length()) {
		haifuP.aliceDoraMax.str(haifuP.aliceDora.str());
		HThaifuP.aliceDoraMax.str(HThaifuP.aliceDora.str());
	}
}

/* ƒAƒŠƒXƒhƒ‰‰Šú‰» */
__declspec(dllexport) void haifu::haifuresetalicedora() {
	haifuP.aliceDora.str("");
	HThaifuP.aliceDora.str("");
}

/* ƒhƒ‰‚ÌƒZƒpƒŒ[ƒ^[ */
__declspec(dllexport) void haifu::haifurecdorap() {
	haifuP.dora << " ";
	haifuP.uraDora << " ";
	haifuP.aliceDora << " ";
	HThaifuP.dora << "</span> <span class=\"tile\">";
	HThaifuP.uraDora << "</span> <span class=\"tile\">";
	HThaifuP.aliceDora << "</span> <span class=\"tile\">";
}

/* –Ì‘Å‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecmota(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecmota(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecmota(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	// ƒcƒ‚‚Á‚Ä‚«‚½”v‚ğ‹L˜^
	if (gameStat->TianHuFlag) {
		// e‚Ì‚P„–Ú
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo);
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
	} else if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile == NoTile) {
		// –Â‚¢‚½’¼Œã (‰½‚à‚µ‚È‚¢)
	} else if ((DiscardTileIndex.id) == (NUM_OF_TILES_IN_HAND - 1)) {
		// ƒcƒ‚Ø‚è
		tools::recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << "« ";
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumo << "<td class=\"fallthru\">«</td>";
	} else {
		tools::haifuwritetsumohai(
			&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
			gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
			"@ ", "");
	}
	// Ì‚Ä‚½”v‚ğ‹L˜^
	tools::recordTile_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
}

/* •úe‚µ‚½‚©”Û‚©‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecfurikomi(const GameTable* const gameStat) {
	if (gameStat->Player[gameStat->CurrentPlayer.Active].RichiFlag.IppatsuFlag) {
		// —§’¼éŒ¾”v‚Ìê‡
		if (RonPlayers(gameStat) > 0) {
			// —§’¼éŒ¾”v‚Å‚ÌU‚è‚İ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "ØX ";
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>ƒŠ[ƒ`<br>ƒEƒ`</td>";
		} else {
			// —§’¼‚ª’Ê‚Á‚½ê‡
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "Ø°Á";
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>ƒŠ[ƒ`</td>";
		}
	} else {
		// ‚»‚êˆÈŠO‚Ìê‡
		if (RonPlayers(gameStat) > 0) {
			// ’Êí‚ÌU‚è‚İ
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "³Á ";
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td>ƒEƒ`</td>";
		} else {
			// ‰½‚à‚È‚©‚Á‚½ê‡
			haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "@ ";
			HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << "<td></td>";
		}
	}
}

/* ƒ`[‚µ‚½‚±‚Æ‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecchi(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, "Á° ", "ƒ`[");
}
/* ƒ|ƒ“‚µ‚½‚±‚Æ‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecpon(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, "Îßİ", "ƒ|ƒ“");
}
/* ©–Ì˜a‚µ‚½‚±‚Æ‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurectsumo(const GameTable* const gameStat) {
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active,
		gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
		"ÂÓ ", "ƒcƒ‚");
}
/* ‘å–¾È‚µ‚½‚±‚Æ‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecminkan(const GameTable* const gameStat) {
	tools::haifuskip(
		&haifuP, &HThaifuP,
		gameStat->CurrentPlayer.Passive, gameStat->CurrentPlayer.Active);
	tools::haifuwritetsumohai(
		&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive,
		gameStat->CurrentDiscard, "¶İ ", "ƒJƒ“");
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehai);
	tools::recordBlank_Table(
		&haifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel,
		&HThaifuP.streamDat[gameStat->CurrentPlayer.Passive].sutehailabel);
	// ”v•ˆ‚Ì‹Lqã‚ÍA—äã”v‚Ìæ“¾‚ÍŸ„‚Æ‚µ‚Äˆµ‚¤
	tools::haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Passive);
}

/* ƒJƒ“‚ª‚ ‚Á‚½ */
void haifu::tools::kan_sub::recordKanOrFlower(
	const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex,
	HaifuStreams* haifuP, HaifuStreams* HThaifuP
	) {
		if ((gameStat->TianHuFlag)||((DiscardTileIndex.id) != (NUM_OF_TILES_IN_HAND - 1))) {
			// e‚Ì‚P„–Ú‚Ìê‡‚©Aƒcƒ‚‚Á‚Ä‚«‚½”vˆÈŠO‚ğƒJƒ“‚µ‚½ê‡
			if (gameStat->TianHuFlag) {
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo);
				recordBlank_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumolabel);
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
				haifukanflag = true;
			} else if (gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1].tile ==
				gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id].tile) {
					// ƒcƒ‚‚Á‚Ä‚«‚½”v‚Æ“¯‚¶‚¾‚Á‚½
					recordTile_Table(
						&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
						gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
					haifukanflag = false;
			} else {
				haifuwritetsumohai(
					haifuP, HThaifuP, gameStat->CurrentPlayer.Active,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[NUM_OF_TILES_IN_HAND - 1],
					"@ ", "");
				recordTile_Table(
					&haifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].sutehai,
					gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
				haifukanflag = true;
			}
		} else {
			recordTile_Table(
				&haifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				&HThaifuP->streamDat[gameStat->CurrentPlayer.Active].tsumo,
				gameStat->Player[gameStat->CurrentPlayer.Active].Hand[DiscardTileIndex.id]);
			haifukanflag = false;
		}
}
/* g–kh‚ğ”v•ˆ‚É‹L˜^ */
void inline haifu::tools::kan_sub::recordChanKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt) {
	if (haifukanflag) {
		// e‚Ì‚P„–Ú‚Ìê‡‚©Aƒcƒ‚‚Á‚Ä‚«‚½”vˆÈŠO‚ğƒJƒ“‚µ‚½ê‡
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
	}
}
/* ”²‚«–k‚ª¬Œ÷‚µ‚½ê‡‚Ì”v•ˆˆ— */
void inline haifu::tools::kan_sub::recordKan(const GameTable* const gameStat, std::string pTxt, std::string hTxt) {
	if (haifukanflag == 1) {
		// e‚Ì‚P„–Ú‚Ìê‡‚©Aƒcƒ‚‚Á‚Ä‚«‚½”vˆÈŠO‚ğƒJƒ“‚µ‚½ê‡
		haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel << hTxt;
	} else {
		haifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << pTxt;
		HThaifuP.streamDat[gameStat->CurrentPlayer.Active].tsumolabel << hTxt;
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehai);
		recordBlank_Table(
			&haifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel,
			&HThaifuP.streamDat[gameStat->CurrentPlayer.Active].sutehailabel);
	}
		
	// ”v•ˆ‚Ì‹Lqã‚ÍA—äã”v‚Ìæ“¾‚ÍŸ„‚Æ‚µ‚Äˆµ‚¤
	haifuskipall(&haifuP, &HThaifuP, gameStat->CurrentPlayer.Active);
}

/* ”²‚«–k‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecnorth(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecnorth(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* g–kh‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecchanpei(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, "”²X", "<td>”²<br>ƒEƒ`</td>");
}
/* ”²‚«–k‚ª¬Œ÷‚µ‚½ê‡‚Ì”v•ˆˆ— */
__declspec(dllexport) void haifu::haifurecnorthproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, "”² ", "<td>”²</td>");
}

/* ˆÃÈ‚È‚¢‚µ‰ÁÈ‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecankan(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecankan(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecankan(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
}
/* È‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecchankan(const GameTable* const gameStat) {
	tools::kan_sub::recordChanKan(gameStat, "¶İX", "<td>ƒJƒ“<br>ƒEƒ`</td>");
}
/* ƒJƒ“‚ª¬Œ÷‚µ‚½ê‡‚Ì”v•ˆˆ— */
__declspec(dllexport) void haifu::haifureckanproc(const GameTable* const gameStat) {
	tools::kan_sub::recordKan(gameStat, "¶İ ", "<td>ƒJƒ“</td>");
}

/* ‰Ô”v‚ğ”v•ˆ‚É‹L˜^ */
__declspec(dllexport) void haifu::haifurecflower(const GameTable* const gameStat, int DiscardTileIndex) {
	haifurecflower(gameStat, DiscardTileNum::fromSingleInt(DiscardTileIndex));
}
void haifu::haifurecflower(const GameTable* const gameStat, const DiscardTileNum& DiscardTileIndex) {
	tools::kan_sub::recordKanOrFlower(gameStat, DiscardTileIndex, &haifuP, &HThaifuP);
	tools::kan_sub::recordKan(gameStat, "‰Ô ", "<td>‰Ô</td>");
}

/* hfwriter */
void haifu::tools::hfwriter::hfWriteHead(const GameTable* const gameStat,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	std::string ResultDesc, EndType RoundEndType) {
		haifuBuffer << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) haifuBuffer << " " << OrigHonba << "–{ê";
		haifuBuffer << " ƒhƒ‰F" << haifuP.dora.str();
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			haifuBuffer << "— ƒhƒ‰F" << haifuP.uraDora.str();
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			haifuBuffer << "ƒAƒŠƒXF" << haifuP.aliceDoraMax.str();
		haifuBuffer << std::endl << std::endl <<
			"Œ‹‰ÊF" << ResultDesc << std::endl << std::endl;

		HThaifuBuffer << "<h2>" << ::roundName(OrigTurn, gameStat);
		if (OrigHonba > 0) HThaifuBuffer << " " << OrigHonba <<"–{ê";
		HThaifuBuffer << " ƒhƒ‰F<span class=\"tile\">" <<
			HThaifuP.dora.str() << "</span>";
		if ((RoundEndType == Agari)&&(tmpUraFlag == 1)&&(RuleData::chkRuleApplied("uradora")))
			HThaifuBuffer << "— ƒhƒ‰F<span class=\"tile\">" <<
			haifuP.uraDora.str() << "</span>";
		if ((RoundEndType == Agari)&&(tmpAliceFlag == 1)&&(RuleData::chkRuleApplied("alice")))
			HThaifuBuffer << "ƒAƒŠƒXF<span class=\"tile\">" <<
			haifuP.aliceDoraMax.str() << "</span>";
		HThaifuBuffer << "</h2>" << std::endl <<
			"<p>Œ‹‰ÊF" << ResultDesc << "</p>" << std::endl <<
			"<table>" << std::endl << "<tr>";
		for (unsigned int i = 0u; i < cols; i++)
			HThaifuBuffer << "<td width=" << (100.0/((double)cols)) << "%></td>";
		HThaifuBuffer << "</tr>" << std::endl;
}

// ÅI”vp
void haifu::tools::hfwriter::finalformWriter::hfFinalForm(const GameTable* const gameStat, PLAYER_ID player, EndType RoundEndType) {
	// ÅI”vp(ƒè”v‚Ì‚İ)
	for (int i = 0; i < NUM_OF_TILES_IN_HAND; i++) {
		if (gameStat->Player[player].Hand[i].tile != NoTile) {
			if (i == NUM_OF_TILES_IN_HAND - 1) {
				if ((RoundEndType == Ryuukyoku)||(RoundEndType == Agari)||(RoundEndType == Chonbo)) {
					if (gameStat->TsumoAgariFlag) {
						haifuP.streamDat[player].final << " ƒcƒ‚";
						HThaifuP.streamDat[player].final << "</span> ƒcƒ‚<span class=\"tile\">";
					} else {
						haifuP.streamDat[player].final << " ƒƒ“";
						HThaifuP.streamDat[player].final << "</span> ƒƒ“<span class=\"tile\">";
					}
				}
				else haifuP.streamDat[player].final << " ";
			}
			recordTile_Inline(
				&haifuP.streamDat[player].final, &HThaifuP.streamDat[player].final,
				gameStat->Player[player].Hand[i], false);
		}
	}
}
void haifu::tools::hfwriter::finalformWriter::hfFlower(const GameTable* const gameStat, PLAYER_ID player) {
	if (gameStat->Player[player].FlowerFlag.Spring ||
		gameStat->Player[player].FlowerFlag.Summer ||
		gameStat->Player[player].FlowerFlag.Autumn ||
		gameStat->Player[player].FlowerFlag.Winter) {
			haifuP.streamDat[player].final << " ";
			HThaifuP.streamDat[player].final << "</span> <span class=\"tile\">";
			if (gameStat->Player[player].FlowerFlag.Spring) {
				haifuP.streamDat[player].final << "t";
				HThaifuP.streamDat[player].final << "@";
			}
			if (gameStat->Player[player].FlowerFlag.Summer) {
				haifuP.streamDat[player].final << "‰Ä";
				HThaifuP.streamDat[player].final << ";";
			}
			if (gameStat->Player[player].FlowerFlag.Autumn) {
				haifuP.streamDat[player].final << "H";
				HThaifuP.streamDat[player].final << ":";
			}
			if (gameStat->Player[player].FlowerFlag.Winter) {
				haifuP.streamDat[player].final << "“~";
				HThaifuP.streamDat[player].final << "]";
			}
	}
	if (gameStat->Player[player].FlowerFlag.Plum ||
		gameStat->Player[player].FlowerFlag.Orchid ||
		gameStat->Player[player].FlowerFlag.Chrys ||
		gameStat->Player[player].FlowerFlag.Bamboo) {
			haifuP.streamDat[player].final << " ";
			HThaifuP.streamDat[player].final << "</span> <span class=\"tile\">";
			if (gameStat->Player[player].FlowerFlag.Plum) {
				haifuP.streamDat[player].final << "”~";
				HThaifuP.streamDat[player].final << "-";
			}
			if (gameStat->Player[player].FlowerFlag.Orchid) {
				haifuP.streamDat[player].final << "—–";
				HThaifuP.streamDat[player].final << "^";
			}
			if (gameStat->Player[player].FlowerFlag.Chrys) {
				haifuP.streamDat[player].final << "‹e";
				HThaifuP.streamDat[player].final << "[";
			}
			if (gameStat->Player[player].FlowerFlag.Bamboo) {
				haifuP.streamDat[player].final << "’|";
				HThaifuP.streamDat[player].final << "\\";
			}
	}
}

void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfChii(PLAYER_ID player, meldCode meld) {
	// ƒ`[
	haifuP.streamDat[player].final << " ƒƒ`[";
	HThaifuP.streamDat[player].final << "</span> ƒ`[<span class=\"tile\">";
	TILE meldTile[3];
	switch (meld.mstat) {
	case meldSequenceExposedLower:
		meldTile[0].tile = meld.tile; meldTile[0].red = meld.red[0];
		meldTile[1].tile = tileCode(meld.tile + 1); meldTile[1].red = meld.red[1];
		meldTile[2].tile = tileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
		break;
	case meldSequenceExposedMiddle:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[0].tile = tileCode(meld.tile + 1); meldTile[0].red = meld.red[1];
		meldTile[2].tile = tileCode(meld.tile + 2); meldTile[2].red = meld.red[2];
	case meldSequenceExposedUpper:
		meldTile[1].tile = meld.tile; meldTile[1].red = meld.red[0];
		meldTile[2].tile = tileCode(meld.tile + 1); meldTile[2].red = meld.red[1];
		meldTile[0].tile = tileCode(meld.tile + 2); meldTile[0].red = meld.red[2];
	}
	for (int i = 0; i < 3; i++) {
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile[i], i == 0);
	}
}
inline void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon1(PLAYER_ID player, meldCode meld) {
	TILE meldTile = {meld.tile, meld.red[0]};
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
}
void haifu::tools::hfwriter::finalformWriter::MeldWriter::hfPon(PLAYER_ID player, meldCode meld) {
	int tiles, interrupt;
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldQuadExposedLeft: case meldQuadAddedLeft:
		haifuP.streamDat[player].final << " ƒ"; interrupt = 1; break;
	case meldTripletExposedCenter: case meldQuadExposedCenter: case meldQuadAddedCenter:
		haifuP.streamDat[player].final << " È"; interrupt = 2; break;
	case meldTripletExposedRight: case meldQuadExposedRight: case meldQuadAddedRight:
		haifuP.streamDat[player].final << " „"; interrupt = 8; break;
	case meldQuadConcealed:
		haifuP.streamDat[player].final << " "; interrupt = 7; break;
	}
	switch (meld.mstat) {
	case meldTripletExposedLeft: case meldTripletExposedCenter:
	case meldTripletExposedRight:
		tiles = 3;
		haifuP.streamDat[player].final << "ƒ|ƒ“";
		HThaifuP.streamDat[player].final << "</span> ƒ|ƒ“<span class=\"tile\">";
		break;
	case meldQuadExposedLeft: case meldQuadExposedCenter:
	case meldQuadExposedRight: case meldQuadConcealed:
		tiles = 4;
		haifuP.streamDat[player].final << "ƒJƒ“";
		HThaifuP.streamDat[player].final << "</span> ƒJƒ“<span class=\"tile\">";
		break;
	case meldQuadAddedLeft: case meldQuadAddedCenter:
	case meldQuadAddedRight:
		tiles = 3;
		haifuP.streamDat[player].final << "¤ƒJƒ“";
		HThaifuP.streamDat[player].final << "</span> ƒJƒ“<span class=\"tile\">";
		break;
	}
	for (int i = (meld.mstat == meldQuadConcealed ? 0 : 1); i < tiles; i++) {
		if (i == interrupt) hfPon1(player, meld);
		TILE meldTile = {meld.tile, meld.red[i]};
		recordTile_Inline(
			&haifuP.streamDat[player].final,
			&HThaifuP.streamDat[player].final,
			meldTile, false);
	}
	if (interrupt == 8) hfPon1(player, meld);
}

void haifu::tools::hfwriter::finalformWriter::hfExposedMeld(const GameTable* const gameStat, PLAYER_ID player) {
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

void haifu::tools::hfwriter::hfScoreWriteOut(const GameTable* const gameStat, PLAYER_ID player, seatAbsolute wind) {
	// “_”‚Ì•Ï“®
	std::ostringstream o;
	o << " " << origPoint[player].bignumtotext("", "¢");
	if (origPoint[player] != gameStat->Player[player].PlayerScore) // “_”‚ªˆê’v‚µ‚È‚¢‚È‚ç
		o << " ¨ " <<
			gameStat->Player[player].PlayerScore.bignumtotext("", "¢") << " (" <<
			((LargeNum)gameStat->Player[player].PlayerScore -
			origPoint[player]).bignumtotext("+", "-") <<
			")";
	if (RuleData::chkRuleApplied("chip")) // ƒ`ƒbƒv‚ ‚è‚Ì
		o << " ƒ`ƒbƒv: " <<
			((gameStat->Player[player].playerChip >= 0) ? "+" : "") <<
			(int)gameStat->Player[player].playerChip;

	//chatappend "*** "+getPlayerName(GameEnv, tmpcnt)+"("+windName(cnt)+") "+haifutmpscore+"\n"
					
	// o—Í
	haifuBuffer << windName(wind) << " " <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << std::endl;
	HThaifuBuffer << "<tr><td colspan=" << cols << " class=\"player\">" <<
		windName(wind) << " " <<
		EnvTable::Instantiate()->PlayerDat[player].PlayerName <<
		o.str() << "</td></tr>" << std::endl;
}

void haifu::tools::hfwriter::hfWriteOut(const GameTable* const gameStat, PLAYER_ID player) {
	haifuBuffer << "”z”vF" << haifuP.streamDat[player].haipai.str() <<
		std::endl << std::endl;
	HThaifuBuffer << "<tr><td class=\"label\">”z”v</td><td colspan=" << (cols - 1) <<
		"><span class=\"tile\">" << HThaifuP.streamDat[player].haipai.str() <<
		"</span></td></tr>" << std::endl << std::endl;
	if (!gameStat->TianHuFlag) { // “V˜a(‚Ü‚½‚Íe‚Ì\O•s“ƒ‚â‹ãí‹ã”v)‚Ìê‡‚ÍÈ—ª
		haifuBuffer <<
			"©–ÌF" << haifuP.streamDat[player].tsumo.str() << std::endl <<
			"@@@" << haifuP.streamDat[player].tsumolabel.str() << std::endl <<
			"‘Å”vF" << haifuP.streamDat[player].sutehai.str() << std::endl <<
			"@@@" << haifuP.streamDat[player].sutehailabel.str() << std::endl <<
			"”vpF" << haifuP.streamDat[player].final.str() << std::endl <<
			std::endl;
		HThaifuBuffer <<
			"<tr class=\"tile\"><td class=\"label\" rowspan=2>©–Ì</td>" <<
			HThaifuP.streamDat[player].tsumo.str() << "</tr>" << std::endl <<
			"<tr class=\"notice\">" <<
			HThaifuP.streamDat[player].tsumolabel.str() << "</tr>" << std::endl <<
			"<tr class=\"tile\"><td class=\"label\" rowspan=2>‘Å”v</td>" <<
			HThaifuP.streamDat[player].sutehai.str() << "</tr>" << std::endl <<
			"<tr class=\"notice\">" <<
			HThaifuP.streamDat[player].sutehailabel.str() << "</tr>" << std::endl <<
			"<tr><td class=\"label\">”vp</td><td colspan=" << (cols - 1) <<
			" class=\"hand\"><span class=\"tile\">" <<
			HThaifuP.streamDat[player].final.str() << "</span></td></tr>" << std::endl <<
			std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteFinalForms(const GameTable* const gameStat, int OrigTurn, EndType RoundEndType) {
	for (int i = 0; i < ACTUAL_PLAYERS; i++) {
		PLAYER_ID k = RelativePositionOf(i, OrigTurn % PLAYERS);
		if (chkGameType(gameStat, SanmaT))
			if (((OrigTurn % PLAYERS) + i) >= ACTUAL_PLAYERS)
				k = (k + 1) % PLAYERS;
		// •›˜I–Êq‚ğo—Í‚·‚é
		finalformWriter::hfFinalForm(gameStat, k, RoundEndType);
		finalformWriter::hfFlower(gameStat, k);
		finalformWriter::hfExposedMeld(gameStat, k);
		// “_–_ó‹µ‚ğ‘‚«o‚·
		hfScoreWriteOut(gameStat, k, (seatAbsolute)i);
		// FX‘‚«o‚µ
		if ((!chkGameType(gameStat, Sanma4))||(i < 3))
			hfWriteOut(gameStat, k);
		else haifuBuffer << std::endl;
	}
}

void haifu::tools::hfwriter::hfWriteBottom() {
	haifuBuffer <<
		"------------------------------------------------------------------------------" <<
		std::endl << std::endl;
	HThaifuBuffer << "</table>" << std::endl << "<hr>" << std::endl;
}

/* ”z”v‚ğƒoƒbƒtƒ@‚Éo—Í */
__declspec(dllexport) void haifu::haifuwritebuffer(
	const GameTable* const gameStat, void *,
	int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
	const char* ResultDesc, EndType RoundEndType
	) { /* ”z”v‚ğƒoƒbƒtƒ@‚Éo—Í */
		tools::hfwriter::hfWriteHead(
			gameStat, OrigTurn, OrigHonba, tmpUraFlag, tmpAliceFlag,
			ResultDesc, RoundEndType);
		tools::hfwriter::hfWriteFinalForms(
			gameStat, OrigTurn, RoundEndType);
		tools::hfwriter::hfWriteBottom();
}

/* ”v•ˆ‚ğ•Û‘¶ */
void haifu::haifusave(const GameTable* const gameStat) {
	std::string configPath = confpath::confPath();
	HThaifuBuffer << "</body>" << std::endl << "</html>" << std::endl; // Finalize HTML
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

	std::ofstream fileout;
	fileout.open((filename1.str() + std::string("_haifu_") +
		filename2.str() + std::string(".txt")).c_str());
	fileout << haifuBuffer.str(); fileout.close();
	/* ƒ`ƒƒƒbƒgƒƒO‚Í–¢À‘•‚Å‚· */
	fileout.open((filename1.str() + std::string("_haifu_") +
		filename2.str() + std::string(".htm")).c_str());
	fileout << HThaifuBuffer.str(); fileout.close();
}
__declspec(dllexport) void haifu::haifusave() {
	haifusave(&GameStat);
}
