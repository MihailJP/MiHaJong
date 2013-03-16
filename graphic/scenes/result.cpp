#include "result.h"

namespace mihajong_graphic {

using namespace mihajong_structs;

// -------------------------------------------------------------------------

ResultScreen::ResultScreen(ScreenManipulator* const manipulator) : SystemScreen(manipulator) {
	rankRenderer.fill(nullptr);
}

ResultScreen::~ResultScreen() {
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) delete *k;
}

void ResultScreen::Render() {
	clearWithGameTypeColor();
	for (int i = 0; i < Players; ++i)
		if ((myTimer.elapsed() >= (1000000 + i * 500000)) && (rankRenderer[i] == nullptr))
			rankRenderer[i] = new RankRenderer(caller->getDevice(), i);
	for (auto k = rankRenderer.begin(); k != rankRenderer.end(); ++k)
		if (*k) (*k)->Render();
}

void ResultScreen::KeyboardInput(LPDIDEVICEOBJECTDATA od) {
}

void ResultScreen::MouseInput(LPDIDEVICEOBJECTDATA od, int X, int Y) {
}

// -------------------------------------------------------------------------

ResultScreen::RankRenderer::RankRenderer(LPDIRECT3DDEVICE9 device, int id) {
	myDevice = device;
	myID = id;
	BaseY = (3 - id) * 150 + 300;
	nameRenderer = new SmallTextRenderer(device);
	rankRenderer = new HugeTextRenderer(device);
	scoreRenderer = new ScoreDigitRenderer(device);
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
	CodeConv::tostringstream o; o << (4 - myID);
	rankRenderer->NewText(0, o.str(), 60, BaseY, 0.625f);
	rankRenderer->Render();
}

void ResultScreen::RankRenderer::RenderNameScore() {
	const CodeConv::tstring nomen(_T("‚¢‚ë‚Í‚É‚Ù‚Ö‚Æ‚¿‚è‚Ê‚é‚ð‚í‚©‚æ"));
	const unsigned latitudoNominis = stringWidth(nomen);
	nameRenderer->NewText(0, nomen, 150, BaseY + 10, 3.0f,
		(latitudoNominis >= 30) ? 30.0f / (float)latitudoNominis : 1.0f);
	const CodeConv::tstring punctaticum(_T("‘f“_F12345678901234567890123456789012345678901234567890"));
	const unsigned latitudoPunctatici = stringWidth(punctaticum);
	nameRenderer->NewText(1, punctaticum, 150, BaseY + 70, 3.0f,
		(latitudoPunctatici >= 30) ? 30.0f / (float)latitudoPunctatici : 1.0f);
	nameRenderer->Render();
}

void ResultScreen::RankRenderer::RenderScore() {
	const CodeConv::tstring scoreTxt(_T("+1–œ2345"));
	const unsigned strWidth = [](const CodeConv::tstring& str) -> unsigned {
		const std::wstring ws =
#ifdef _UNICODE
			str;
#else
			CodeConv::ANSItoWIDE(str);
#endif
		return ws.length();
	} (scoreTxt);
	scoreRenderer->NewText(0, scoreTxt, 1000, BaseY + 10, 1.0f,
		(strWidth > 4u) ? (4.0f / (float)strWidth) : 1.0f);
	scoreRenderer->Render();
}

void ResultScreen::RankRenderer::Render() {
	RenderRank();
	RenderNameScore();
	RenderScore();
}

// -------------------------------------------------------------------------

}
