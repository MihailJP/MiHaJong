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
#ifndef _WIN32
#include "../keycode.h"
#endif /*_WIN32*/

namespace mihajong_graphic {

using namespace mihajong_structs;

// -------------------------------------------------------------------------

ResultScreen::ResultScreen(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	rankRenderer.fill(nullptr);
	titleRenderer = new HugeTextRenderer(manipulator->getDevice());
}

ResultScreen::~ResultScreen() {
	for (const auto& k : rankRenderer)
		if (k) delete k;
	delete titleRenderer;
}

void ResultScreen::Render() {
	clearWithGameTypeColor();

	titleRenderer->NewText(0, _T("終　　局"), adjX(272), 60, 1.0f, WidthRate(),
		(myTimer.elapsed() < 1000000) ? ((static_cast<unsigned int>(255 - (1000000 - myTimer.elapsed()) / 5000) << 24) | 0x00ffffff) : 0xffffffff);
	titleRenderer->Render();

	for (int i = (GameStatus::gameStat()->chkGameType(SanmaT) ? 1 : 0); i < Players; ++i)
		if ((myTimer.elapsed() >= (1000000 + i * 500000)) && (rankRenderer[i] == nullptr))
			rankRenderer[i] = new RankRenderer(caller->getDevice(), i);
	for (const auto& k : rankRenderer)
		if (k) k->Render();
}

#ifdef _WIN32
void ResultScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od)
#else /*_WIN32*/
void ResultScreen::KeyboardInput(const XEvent* od)
#endif /*_WIN32*/
{
#ifdef _WIN32
	const bool flag = ((myTimer.elapsed() > 3000000u) && (od->dwData));
	switch (od->dwOfs)
#else /*_WIN32*/
	const bool flag = ((myTimer.elapsed() > 3000000u) && (od->type == KeyPress));
	switch (od->xkey.keycode)
#endif /*_WIN32*/
	{
	case DIK_RETURN: case DIK_Z: case DIK_SPACE: // 決定
#ifdef _WIN32
		if (od->dwData)
#else /*_WIN32*/
		if (od->type == KeyPress)
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			if (flag)
				ui::UIEvent->set(0); // イベントをセット
			else
				myTimer.skipTo(3000000u);
		}
		break;
	}
}


#ifdef _WIN32
void ResultScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y)
#else /*_WIN32*/
void ResultScreen::MouseInput(const XEvent* od, int X, int Y)
#endif /*_WIN32*/
{
	const bool flag = (myTimer.elapsed() > 3000000u);
#ifdef _WIN32
	switch (od->dwOfs)
#else /*_WIN32*/
	switch (od->type)
#endif /*_WIN32*/
	{
#ifdef _WIN32
	case DIMOFS_BUTTON0: // マウスの左ボタン
		if (od->dwData)
#else /*_WIN32*/
	case ButtonPress: // マウスクリック
		if (od->xbutton.button == Button1)
#endif /*_WIN32*/
		{
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

ResultScreen::RankRenderer::RankRenderer(DevicePtr device, int id) {
	myDevice = device;
	myID = id;
	BaseY = (3 - id) * 150 + 350;
	PlayerRankList rank = utils::calcRank(GameStatus::retrGameStat());
	player = -1;
	for (PlayerID i = 0; i < (GameStatus::retrGameStat()->chkGameType(SanmaT) ? 3 : 4); ++i)
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
		else if (rank[GameStatus::gameStat()->PlayerID] == (GameStatus::gameStat()->chkGameType(SanmaT) ? 3 : 4))
			sound::util::bgmplay(sound::IDs::musEnding3);
		else
			sound::util::bgmplay(sound::IDs::musEnding2);
}

ResultScreen::RankRenderer::~RankRenderer() {
	delete nameRenderer;
	delete rankRenderer;
	delete scoreRenderer;
}

void ResultScreen::RankRenderer::RenderRank() {
	CodeConv::tostringstream o; o << (4 - myID);
	ArgbColor txtcolor;
	switch (4 - myID) {
		case 1:  txtcolor = 0xffffd700; break;
		case 2:  txtcolor = 0xffffffff; break;
		case 3:  txtcolor = 0xffb87333; break;
		default: txtcolor = 0xff9955ee; break;
	}
	rankRenderer->NewText(0, o.str(), adjX(60), BaseY, 0.625f, WidthRate(), txtcolor);
	rankRenderer->Render();
}

void ResultScreen::RankRenderer::RenderNameScore() {
	const uint64_t tempus = myTimer.elapsed();
	const ArgbColor aColor = static_cast<uint32_t>((tempus >= animTime) ? 255 :
		(255 - static_cast<int>(200.0 - (static_cast<double>(tempus) / static_cast<double>(animTime) * 200.0))))
		<< 24 | 0x00ffffff;
	const ArgbColor color = (player != GameStatus::gameStat()->PlayerID) ? 0xffffffff :
		0xffff0000 |
		(static_cast<uint32_t>(200.0 - sin(static_cast<double>(myTimer.elapsed()) * M_PI / 500000.0) * 50.0) << 8) |
		static_cast<uint32_t>(200.0 - sin(static_cast<double>(myTimer.elapsed()) * M_PI / 500000.0) * 50.0);
	const CodeConv::tstring nomen(utils::getName(player));
	const unsigned latitudoNominis = nameRenderer->strWidthByCols(nomen);
	nameRenderer->NewText(0, nomen, adjX(150), BaseY + 10, 3.0f,
		((latitudoNominis >= 30) ? 30.0f / static_cast<float>(latitudoNominis) : 1.0f) * WidthRate(),
		aColor & color);

	CodeConv::tostringstream punctaticum_;
	punctaticum_ << _T("素点：") <<
		(GameStatus::gameStat()->Player[player].PlayerScore.to_str(_T(""), _T("△")));
	const int chipVal = GameStatus::gameStat()->Player[player].playerChip;
	if ((GameStatus::gameStat()->gameType & RichiMJ) && (!rules::chkRule("chip", "no"))) {
		if (chipVal > 0)
			punctaticum_ << _T(" チップ：＋") << chipVal;
		else if (chipVal < 0)
			punctaticum_ << _T(" チップ：△") << (-chipVal);
		else
			punctaticum_ << _T(" チップ：0");
	}
	if ((GameStatus::gameStat()->gameType & RichiMJ) && (!rules::chkRule("yakitori", "no"))) {
		if (GameStatus::gameStat()->Player[player].YakitoriFlag)
			punctaticum_ << _T(" 焼鳥");
	}
	const CodeConv::tstring punctaticum(punctaticum_.str());
	const unsigned latitudoPunctatici = nameRenderer->strWidthByCols(punctaticum);
	nameRenderer->NewText(1, punctaticum, adjX(150), BaseY + 70, 3.0f,
		((latitudoPunctatici >= 30) ? 30.0f / static_cast<float>(latitudoPunctatici) : 1.0f) * WidthRate(),
		aColor & color);
	nameRenderer->Render();
}

void ResultScreen::RankRenderer::RenderScore() {
	constexpr unsigned widthLimit = 4u;
	const uint64_t tempus = myTimer.elapsed();
	const ArgbColor aColor = static_cast<uint32_t>((tempus >= animTime) ? 255 :
		(255 - static_cast<int>(200.0 - (static_cast<double>(tempus) / (static_cast<double>(animTime) / 200.0)))))
		<< 24 | 0x00ffffff;
	const float scale = (tempus >= animTime) ? 1.0f :
		1.0f + pow(static_cast<float>(animTime - tempus) / static_cast<float>(animTime) * 3.5f, 2);
	const LargeNum point(FinalScoreDat::getData(player));
	const CodeConv::tstring scoreTxt(point.to_str(_T("+"), _T("△")));
	const ArgbColor color =
		(point > 0) ? 0xffccffcc :
		(point < 0) ? 0xffffcccc : 0xffffffcc;
	const unsigned strWidth = scoreRenderer->strWidthByCols(scoreTxt) / 2u;
	scoreRenderer->NewText(0, scoreTxt,
		adjX(1000 + 96 * std::max(static_cast<signed>(widthLimit) - static_cast<signed>(strWidth), 0) -
		static_cast<int>(96.0f * static_cast<float>(std::min(strWidth, widthLimit) * (scale - 1.0f)))),
		BaseY + 10 - static_cast<int>(64.0f * (scale - 1.0f)),
		scale,
		(static_cast<float>(widthLimit) / static_cast<float>(std::max(strWidth, widthLimit))) * WidthRate(),
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
