#include "gameloop.h"

#ifndef _WIN32
#include <unistd.h>
#endif /*_WIN32*/
#include "../common/strcode.h"
#include "logging.h"
#include "envtbl.h"
#include "../socket/socket.h"
#include "../sound/sound.h"
#include "../common/bgmid.h"
#include "tileutil.h"
#include "mouda.h"
#include "fuuro.h"
#include "prepare.h"
#include "../graphic/graphic.h"
#include "result.h"
#include "ruletbl.h"
#include "remote.h"
#include "chat.h"

/* �����̐i�s */
EndType doTableTurn(GameTable* const gameStat) {
	{
		CodeConv::tostringstream o;
		o << _T("�v���C���[ [") << (int)gameStat->CurrentPlayer.Active << _T("] �̃c���Ԃł��B");
		info(o.str().c_str());
	}
	/* �ϐ��̏����� */
	calcdoukasen(gameStat); // ���ΐ��̈ʒu���X�V
	if (!gameStat->KangFlag.kangFlag) {
		/* �A���Ȃ̉񐔂Ɠ��Șa�t���O�����Z�b�g */
		gameStat->KangFlag.chainFlag = 0;
		gameStat->KangFlag.topFlag = false;
	}
	gameStat->CurrentDiscard.tile = NoTile;
	gameStat->CurrentDiscard.red = Normal;
	/* �E�H�b�`���[�h�̎��͎��_���c���Ԃ̐l�Ɉڂ� */
	if (EnvTable::Instantiate()->WatchModeFlag)
		gameStat->PlayerID = gameStat->CurrentPlayer.Active;
	/* �ĕ`�� */
	mihajong_graphic::GameStatus::updateGameStat(gameStat);
	/* �̑ł̏��� */
	DiscardTileNum DiscardTileIndex = getdahai(gameStat);
	if (DiscardTileIndex.type == DiscardTileNum::Disconnect)
		return Disconnect;
	/* �E�F�C�g������ */
#ifdef _WIN32
	Sleep(1);
#else /*_WIN32*/
	usleep(1000);
#endif /*_WIN32*/
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
#ifdef _WIN32
	Sleep(80);
#else /*_WIN32*/
	usleep(80000);
#endif /*_WIN32*/
	/* �h�a�̏��� */
	RoundEndType = ronhuproc(gameStat); // �h�a�̏���
	if (RoundEndType != Continuing) return RoundEndType;
#ifdef _WIN32
	Sleep(1);
#else /*_WIN32*/
	usleep(1000);
#endif /*_WIN32*/
	/* �r�����ǂ̔��� */
	EndType round_abort_type = endround::checkroundabort(gameStat);
	if (round_abort_type != Continuing) return round_abort_type;
	/* �̔v���|���A�܂��͑喾�Ȃ���ꍇ�̏��� */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* �����������ꍇ�A�����v���[���[�ɏ��Ԃ��ڂ��Ė߂� */
	/* �E�F�C�g������ */
#ifdef _WIN32
	Sleep(100);
#else /*_WIN32*/
	usleep(100000);
#endif /*_WIN32*/
	/* ���̃v���C���[���v�����̂� */
	tsumoproc(gameStat);
	// �Ŕv�֖߂�
	return Continuing;
}

/* �����̐i�s */
bool doTableRound(GameTable* const gameStat, int& OrigTurn, int& OrigHonba) {
	{
		CodeConv::tostringstream o;
		o << _T("�ǔԍ� [") << gameStat->GameRound << _T("] ���J�n���܂����B");
		info(o.str().c_str());
	}
	/* �`���b�g�̓��� */
	if (EnvTable::Instantiate()->GameMode == EnvTable::Client)
		mihajong_socket::puts(SOCK_CHAT,
#if defined(_WIN32)
			_T("8\r\n"));
#else
			_T("8\n"));
#endif
	/* �E�H�b�`���[�h�̎��͎��_��e�Ɉڂ� */
	gameStat->CurrentPlayer.Active = -1;
	if (EnvTable::Instantiate()->WatchModeFlag)
		gameStat->PlayerID = gameStat->GameRound % Players;
	/* �j�񔪘A���̌�n�� */
	if (gameStat->AgariChain == -1) gameStat->AgariChain = 1;
	/* �����̏������Ɣz�v���s�Ȃ� */
	tableinit(gameStat);
	/* �z�v�I�����̏��� */
	for (PlayerID i = 0; i < Players; i++) {
		gameStat->Player[i].HandStat = handUpright;
		lipai(gameStat, i);
	}
	gameStat->CurrentPlayer.Active = gameStat->GameRound % Players; // �ŏ��ɐe����̔v���s�Ȃ�
	info(_T("�z�v���������܂����B"));
	gameStat->TurnRound = 1; // �z�v���I�������1����
	/* �̑Ń��[�v */
	EndType roundEndType = Continuing;
	do {
		roundEndType = doTableTurn(gameStat);
	} while ((roundEndType == DrawRinshan) || (roundEndType == Continuing));
	OrigHonba = gameStat->Honba; OrigTurn = gameStat->GameRound;
	endround::endround(gameStat, roundEndType, OrigTurn, OrigHonba);
	mihajong_graphic::ui::WaitUIWithTimeout(5000);
	// �����I������
	return endround::nextRound(gameStat, roundEndType, OrigTurn);
}

namespace {

#ifdef _WIN32
DWORD titlescreen() {
#else /*_WIN32*/
uint32_t titlescreen() {
#endif /*_WIN32*/
	mihajong_graphic::Transit(mihajong_graphic::sceneTitle);
	return mihajong_graphic::ui::WaitUI();
}

}

/* �Q�[���J�n���� */
void startgame(GameTypeID gameType) {
	while (true) {
		/* �ϐ��ނ̏����� */
		GameTable* gameStat = initializeGameTable(gameType);
		info(_T("�Q�[���������������܂����B"));
		mihajong_socket::server::rotation_reset();
		RuleData::applyPreference(); // ���ݒ�𔽉f
		sound::Play(sound::IDs::musTitle); // �^�C�g���Ȃ𗬂�
		unsigned ClientNumber = 0u;
	start:
		std::string serverAddr;
		const unsigned short gamePort =
			gameStat->chkGameType(Sanma) ? 50010 :
			gameStat->chkGameType(Sanma4) ? 50030 :
			gameStat->chkGameType(SanmaS) ? 50060 : 50000;
		switch (titlescreen()) { // �^�C�g�����
		case 1:
			EnvTable::Instantiate()->PlayerDat[0].PlayerName =
				CodeConv::tstring(_T("[A]")) + CodeConv::EnsureTStr(RuleData::chkPreference("name"));
			EnvTable::Instantiate()->PlayerDat[1].PlayerName = _T("[b]COM1");
			EnvTable::Instantiate()->PlayerDat[2].PlayerName = _T("[c]COM2");
			EnvTable::Instantiate()->PlayerDat[3].PlayerName = _T("[d]COM3");
			serverAddr = "";
			break;
		case 2:
			RemoteConnection::startServer(serverAddr, gamePort);
			break;
		case 3:
			RemoteConnection::startClient(serverAddr, ClientNumber, gamePort);
			if (EnvTable::Instantiate()->GameMode == EnvTable::Standalone)
				goto start; // �ڑ����s�̎��͖߂�
			break;
		case 4:
			mihajong_graphic::Transit(mihajong_graphic::sceneConfig);
			mihajong_graphic::ui::WaitUI();
			goto start;
		case 5:
			mihajong_graphic::Transit(mihajong_graphic::sceneSetting);
			mihajong_graphic::ui::WaitUI();
			RuleData::applyPreference(); // ���ݒ�𔽉f
			goto start;
		case 6:
			return;
		}
		auto PositionArray = SeatShuffler::shuffle(ClientNumber); // �e���߂̏���
		gameinit(&GameStat, gameType, serverAddr, PositionArray, ClientNumber); // �����̏���������

		/* �����̐i�s */
		bool endFlag = false; int OrigTurn = 0, OrigHonba = 0;
		do {
			endFlag = doTableRound(gameStat, OrigTurn, OrigHonba);
		} while (!endFlag);
		// �����I����
		gameResult(gameStat, OrigTurn, OrigHonba);
	}
}
