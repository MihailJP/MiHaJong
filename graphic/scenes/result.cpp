#ifdef _MSC_VER
#define NOMINMAX
#endif
#include "result.h"
#include <cmath>
#include <algorithm>
#include "../utils.h"
#include "../gametbl.h"
#include "../finscore.h"
#include "../rule.h"
#include "../pi.h"
#include "../event.h"
#include "../geometry.h"
#include "../../sound/sound.h"
#include "../../common/bgmid.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

// -------------------------------------------------------------------------

ResultScreen::ResultScreen(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	rankRenderer.fill(nullptr);
	titleRenderer = new HugeTextRenderer(manipulator->getDevice());
}

ResultScreen::~ResultScreen() {
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) delete *k;
	delete titleRenderer;
}

void ResultScreen::Render() {
	clearWithGameTypeColor();

	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;

	titleRenderer->NewText(0, _T("終　　局"), 272 * widthScale, 60, 1.0f, widthScale,
		(myTimer.elapsed() < 1000000) ? (((unsigned int)(255 - (1000000 - myTimer.elapsed()) / 5000) << 24) | 0x00ffffff) : 0xffffffff);
	titleRenderer->Render();

	for (int i = 0; i < Players; ++i)
		if ((myTimer.elapsed() >= (1000000 + i * 500000)) && (rankRenderer[i] == nullptr))
			rankRenderer[i] = new RankRenderer(caller->getDevice(), i);
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) (*k)->Render();
}

void ResultScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
	const bool flag = ((myTimer.elapsed() > 3000000u) && (od->dwData));
	switch (od->dwOfs) {
	case DIK_RETURN: case DIK_Z: case DIK_SPACE: // 決定
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			if (flag)
				ui::UIEvent->set(0); // イベントをセット
			else
				myTimer.skipTo(3000000u);
		}
		break;
	}
}

void ResultScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
	const bool flag = (myTimer.elapsed() > 3000000u);
	switch (od->dwOfs) {
	case DIMOFS_BUTTON0: // マウスの左ボタン
		if (od->dwData) {
			sound::Play(sound::IDs::sndClick);
			if (flag)
				ui::UIEvent->set(0); // イベントをセット
			else
				myTimer.skipTo(3000000u);
		}
		break;
	}
}

// -------------------------------------------------------------------------

ResultScreen::RankRenderer::RankRenderer(LPDIRECT3DDEVICE9 device, int id) {
	myDevice = device;
	myID = id;
	BaseY = (3 - id) * 150 + 350;
	PlayerRankList rank = utils::calcRank(GameStatus::retrGameStat());
	player = -1;
	for (PlayerID i = 0; i < (utils::chkGameType(GameStatus::retrGameStat(), SanmaT) ? 3 : 4); ++i)
		if (rank[i] == (4 - id)) player = i;
	nameRenderer = new SmallTextRenderer(device);
	rankRenderer = new HugeTextRenderer(device);
	scoreRenderer = new ScoreDigitRenderer(device);

	sound::Play(sound::IDs::sndType);
	if ((4 - id) == 1)
		sound::Play(sound::IDs::sndYakulst2);
	else
		sound::Play(sound::IDs::sndYakulst1);

	if ((4 - id) == 1)
		if (rank[GameStatus::gameStat()->PlayerID] == 1)
			sound::util::bgmplay(sound::IDs::musEnding);
		else if (rank[GameStatus::gameStat()->PlayerID] == (chkGameType(GameStatus::gameStat(), SanmaT) ? 3 : 4))
			sound::util::bgmplay(sound::IDs::musEnding3);
		else
			sound::util::bgmplay(sound::IDs::musEnding2);
}

ResultScreen::RankRenderer::~RankRenderer() {
	delete nameRenderer;
	delete rankRenderer;
	delete scoreRenderer;
}

unsigned ResultScreen::RankRenderer::stringWidth(const CodeConv::tstring& str) {
	std::wstring wstr(
#ifdef _UNICODE
		str
#else
		CodeConv::ANSItoWIDE(str)
#endif
		);
	unsigned Anzahl = 0;
	for (auto k = wstr.begin(); k != wstr.end(); ++k)
		Anzahl += (*k <= L'\x7f') ? 1 : 2;
	return Anzahl;
}

void ResultScreen::RankRenderer::RenderRank() {
	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;
	CodeConv::tostringstream o; o << (4 - myID);
	D3DCOLOR txtcolor;
	switch (4 - myID) {
		case 1:  txtcolor = 0xffffd700; break;
		case 2:  txtcolor = 0xffffffff; break;
		case 3:  txtcolor = 0xffb87333; break;
		default: txtcolor = 0xff9955ee; break;
	}
	rankRenderer->NewText(0, o.str(), 60 * widthScale, BaseY, 0.625f, widthScale, txtcolor);
	rankRenderer->Render();
}

void ResultScreen::RankRenderer::RenderNameScore() {
	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;
	const uint64_t tempus = myTimer.elapsed();
	const D3DCOLOR aColor = D3DCOLOR_ARGB((tempus >= animTime) ? 255 :
		(255 - (int)(200.0 - ((double)tempus / (double)animTime * 200.0))),
		255, 255, 255);
	const D3DCOLOR color = (player != GameStatus::gameStat()->PlayerID) ? 0xffffffff :
		D3DCOLOR_ARGB(255, 255,
		(int)(200.0 - sin((double)myTimer.elapsed() * M_PI / 500000.0) * 50.0),
		(int)(200.0 - sin((double)myTimer.elapsed() * M_PI / 500000.0) * 50.0)
		);
	const CodeConv::tstring nomen(utils::getName(player));
	const unsigned latitudoNominis = stringWidth(nomen);
	nameRenderer->NewText(0, nomen, 150 * widthScale, BaseY + 10, 3.0f,
		((latitudoNominis >= 30) ? 30.0f / (float)latitudoNominis : 1.0f) * widthScale,
		aColor & color);

	CodeConv::tostringstream punctaticum_;
	punctaticum_ << _T("素点：") <<
		(GameStatus::gameStat()->Player[player].PlayerScore.bignumtotext(_T(""), _T("△")));
	const int chipVal = GameStatus::gameStat()->Player[player].playerChip;
	if (!rules::chkRule("chip", "no")) {
		if (chipVal > 0)
			punctaticum_ << _T(" チップ：＋") << chipVal;
		else if (chipVal < 0)
			punctaticum_ << _T(" チップ：△") << (-chipVal);
		else
			punctaticum_ << _T(" チップ：0");
	}
	if (!rules::chkRule("yakitori", "no")) {
		if (GameStatus::gameStat()->Player[player].YakitoriFlag)
			punctaticum_ << _T(" 焼鳥");
	}
	const CodeConv::tstring punctaticum(punctaticum_.str());
	const unsigned latitudoPunctatici = stringWidth(punctaticum);
	nameRenderer->NewText(1, punctaticum, 150 * widthScale, BaseY + 70, 3.0f,
		((latitudoPunctatici >= 30) ? 30.0f / (float)latitudoPunctatici : 1.0f) * widthScale,
		aColor & color);
	nameRenderer->Render();
}

void ResultScreen::RankRenderer::RenderScore() {
	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;
	const unsigned widthLimit = 4u;
	const uint64_t tempus = myTimer.elapsed();
	const D3DCOLOR aColor = D3DCOLOR_ARGB((tempus >= animTime) ? 255 :
		(255 - (int)(200.0 - ((double)tempus / ((double)animTime / 200.0)))),
		255, 255, 255);
	const float scale = (tempus >= animTime) ? 1.0f :
		1.0f + pow((float)(animTime - tempus) / (float)animTime * 3.5f, 2);
	const LargeNum point(FinalScoreDat::getData(player));
	const CodeConv::tstring scoreTxt(point.bignumtotext(_T("+"), _T("△")));
	const D3DCOLOR color =
		(point > LargeNum::fromInt(0)) ? 0xffccffcc :
		(point < LargeNum::fromInt(0)) ? 0xffffcccc : 0xffffffcc;
	const unsigned strWidth = [](const CodeConv::tstring& str) -> unsigned {
		const std::wstring ws =
#ifdef _UNICODE
			str;
#else
			CodeConv::ANSItoWIDE(str);
#endif
		return ws.length();
	} (scoreTxt);
	scoreRenderer->NewText(0, scoreTxt,
		(1000 + 96 * std::max((signed)widthLimit - (signed)strWidth, 0) -
		(int)(96.0f * (float)std::min(strWidth, widthLimit) * (scale - 1.0))) * widthScale,
		BaseY + 10 - (int)(64.0f * (scale - 1.0)),
		scale,
		((float)widthLimit / (float)std::max(strWidth, widthLimit)) * widthScale,
		aColor & color);
	scoreRenderer->Render();
}

void ResultScreen::RankRenderer::Render() {
	RenderRank();
	RenderNameScore();
	RenderScore();
}

// -------------------------------------------------------------------------

}
