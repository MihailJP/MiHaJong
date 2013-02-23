#include "gameloop.h"

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
	Sleep(1);
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
	Sleep(80);
	/* �h�a�̏��� */
	RoundEndType = ronhuproc(gameStat); // �h�a�̏���
	if (RoundEndType != Continuing) return RoundEndType;
	Sleep(1);
	/* �r�����ǂ̔��� */
	EndType round_abort_type = endround::checkroundabort(gameStat);
	if (round_abort_type != Continuing) return round_abort_type;
	/* �̔v���|���A�܂��͑喾�Ȃ���ꍇ�̏��� */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* �����������ꍇ�A�����v���[���[�ɏ��Ԃ��ڂ��Ė߂� */
	/* �E�F�C�g������ */
	Sleep(100);
	/* ���̃v���C���[���v�����̂� */
	tsumoproc(gameStat);
	// �Ŕv�֖߂�
	return Continuing;
}

/* �����̐i�s */
bool doTableRound(GameTable* const gameStat) {
	{
		CodeConv::tostringstream o;
		o << _T("�ǔԍ� [") << gameStat->GameRound << _T("] ���J�n���܂����B");
		info(o.str().c_str());
	}
	/* TODO: �`���b�g�̓���
	if (getGameMode(GameEnv) == GAMEMODE_CLIENT) {
		//sockput "8\n", SOCK_CHAT+0 // �`���b�g�𓯊�
	}
	*/
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
	int OrigHonba = gameStat->Honba, OrigTurn = gameStat->GameRound;
	endround::endround(gameStat, roundEndType, OrigTurn, OrigHonba);
	Sleep(5000);
	// �����I������
	/* return */ endround::nextRound(gameStat, roundEndType, OrigTurn);
	return false; // ���u��
}

namespace {

DWORD titlescreen() {
	mihajong_graphic::Transit(mihajong_graphic::sceneTitle);
	return mihajong_graphic::ui::WaitUI();
}

}

/* �Q�[���J�n���� */
void startgame(GameTypeID gameType) {
	while (true) {
		/* �ϐ��ނ̏����� */
		GameTable* gameStat = initializeGameTable(gameType);
		/* TODO: ��ʏ����� vanish */
		info(_T("�Q�[���������������܂����B"));
		mihajong_socket::server::rotation_reset();
		sound::Play(sound::IDs::musTitle); // �^�C�g���Ȃ𗬂�
		unsigned ClientNumber = 0u;
		start:
		switch (titlescreen()) { // �^�C�g�����
		case 4:
			mihajong_graphic::Transit(mihajong_graphic::sceneConfig);
			mihajong_graphic::ui::WaitUI();
			goto start;
		case 6:
			return;
		}
		auto PositionArray = SeatShuffler::shuffle(ClientNumber); // �e���߂̏���
		gameinit(&GameStat, gameType, ""/* TODO: �ʐM�ΐ�̎��͐ڑ���T�[�o�[IP�ɂ��邱�� */, PositionArray, ClientNumber); // �����̏���������

		/* �����̐i�s */
		bool endFlag = false;
		do {
			bool endFlag = doTableRound(gameStat);
		} while (!endFlag);
		// �����I����
		/* TODO: �I�����̏��� endgame GameStat, GameEnv, origTurn, origHonba */
		/* TODO: ���͑҂�
		buttonPressed = 0
		repeat
			redrscreen
			await 1000
			if (buttonPressed) {break}
		loop
		*/
	}
}
