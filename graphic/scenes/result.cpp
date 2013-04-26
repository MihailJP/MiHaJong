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
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) delete *k;
	delete titleRenderer;
}

void ResultScreen::Render() {
	clearWithGameTypeColor();

	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;

	titleRenderer->NewText(0, _T("�I�@�@��"), 272 * widthScale, 60, 1.0f, widthScale,
		(myTimer.elapsed() < 1000000) ? (((unsigned int)(255 - (1000000 - myTimer.elapsed()) / 5000) << 24) | 0x00ffffff) : 0xffffffff);
	titleRenderer->Render();

	for (int i = (GameStatus::gameStat()->chkGameType(SanmaT) ? 1 : 0); i < Players; ++i)
		if ((myTimer.elapsed() >= (1000000 + i * 500000)) && (rankRenderer[i] == nullptr))
			rankRenderer[i] = new RankRenderer(caller->getDevice(), i);
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) (*k)->Render();
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
	case DIK_RETURN: case DIK_Z: case DIK_SPACE: // ����
#ifdef _WIN32
		if (od->dwData)
#else /*_WIN32*/
		if (od->type == KeyPress)
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			if (flag)
				ui::UIEvent->set(0); // �C�x���g���Z�b�g
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
	case DIMOFS_BUTTON0: // �}�E�X�̍��{�^��
		if (od->dwData)
#else /*_WIN32*/
	case ButtonPress: // �}�E�X�N���b�N
		if (od->xbutton.button == Button1)
#endif /*_WIN32*/
		{
			sound::Play(sound::IDs::sndClick);
			if (flag)
				ui::UIEvent->set(0); // �C�x���g���Z�b�g
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
	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;
	CodeConv::tostringstream o; o << (4 - myID);
	ArgbColor txtcolor;
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
	const ArgbColor aColor = (uint32_t)((tempus >= animTime) ? 255 :
		(255 - (int)(200.0 - ((double)tempus / (double)animTime * 200.0))))
		<< 24 | 0x00ffffff;
	const ArgbColor color = (player != GameStatus::gameStat()->PlayerID) ? 0xffffffff :
		0xffff0000 |
		((uint32_t)(200.0 - sin((double)myTimer.elapsed() * M_PI / 500000.0) * 50.0) << 8) |
		(uint32_t)(200.0 - sin((double)myTimer.elapsed() * M_PI / 500000.0) * 50.0);
	const CodeConv::tstring nomen(utils::getName(player));
	const unsigned latitudoNominis = nameRenderer->strWidthByCols(nomen);
	nameRenderer->NewText(0, nomen, 150 * widthScale, BaseY + 10, 3.0f,
		((latitudoNominis >= 30) ? 30.0f / (float)latitudoNominis : 1.0f) * widthScale,
		aColor & color);

	CodeConv::tostringstream punctaticum_;
	punctaticum_ << _T("�f�_�F") <<
		(GameStatus::gameStat()->Player[player].PlayerScore.bignumtotext(_T(""), _T("��")));
	const int chipVal = GameStatus::gameStat()->Player[player].playerChip;
	if (!rules::chkRule("chip", "no")) {
		if (chipVal > 0)
			punctaticum_ << _T(" �`�b�v�F�{") << chipVal;
		else if (chipVal < 0)
			punctaticum_ << _T(" �`�b�v�F��") << (-chipVal);
		else
			punctaticum_ << _T(" �`�b�v�F0");
	}
	if (!rules::chkRule("yakitori", "no")) {
		if (GameStatus::gameStat()->Player[player].YakitoriFlag)
			punctaticum_ << _T(" �Ē�");
	}
	const CodeConv::tstring punctaticum(punctaticum_.str());
	const unsigned latitudoPunctatici = nameRenderer->strWidthByCols(punctaticum);
	nameRenderer->NewText(1, punctaticum, 150 * widthScale, BaseY + 70, 3.0f,
		((latitudoPunctatici >= 30) ? 30.0f / (float)latitudoPunctatici : 1.0f) * widthScale,
		aColor & color);
	nameRenderer->Render();
}

void ResultScreen::RankRenderer::RenderScore() {
	const float widthScale = (float)Geometry::WindowWidth * 0.75 / (float)Geometry::WindowHeight;
	const unsigned widthLimit = 4u;
	const uint64_t tempus = myTimer.elapsed();
	const ArgbColor aColor = (uint32_t)((tempus >= animTime) ? 255 :
		(255 - (int)(200.0 - ((double)tempus / ((double)animTime / 200.0)))))
		<< 24 | 0x00ffffff;
	const float scale = (tempus >= animTime) ? 1.0f :
		1.0f + pow((float)(animTime - tempus) / (float)animTime * 3.5f, 2);
	const LargeNum point(FinalScoreDat::getData(player));
	const CodeConv::tstring scoreTxt(point.bignumtotext(_T("+"), _T("��")));
	const ArgbColor color =
		(point > LargeNum::fromInt(0)) ? 0xffccffcc :
		(point < LargeNum::fromInt(0)) ? 0xffffcccc : 0xffffffcc;
	const unsigned strWidth = scoreRenderer->strWidthByCols(scoreTxt) / 2u;
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
