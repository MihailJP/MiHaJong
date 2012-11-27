#include "gameloop.h"


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
	/* TODO: redrscreen */
	/* �̑ł̏��� */
	DiscardTileNum DiscardTileIndex = getdahai(gameStat);
	if (DiscardTileIndex.type == DiscardTileNum::Disconnect)
		return Disconnect;
	/* �E�F�C�g������ */
	/* await 0 */
	EndType RoundEndType = procdahai(gameStat, DiscardTileIndex);
	if (RoundEndType != Continuing)
		return RoundEndType;
	/* await 80 */
	/* �h�a�̏��� */
	RoundEndType = ronhuproc(gameStat); // �h�a�̏���
	if (RoundEndType != Continuing) return RoundEndType;
	/* await 0 */
	/* �r�����ǂ̔��� */
	/* TODO: �������ڐA����
	checkroundabort GameStat, GameEnv
	if (stat >= 0) {RoundEndType = stat: break}
	*/
	/* �̔v���|���A�܂��͑喾�Ȃ���ꍇ�̏��� */
	if (executeFuuro(gameStat, DiscardTileIndex))
		return Continuing; /* �����������ꍇ�A�����v���[���[�ɏ��Ԃ��ڂ��Ė߂� */
	/* �E�F�C�g������ */
	/* await 100 */
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
		gameStat->PlayerID = gameStat->GameRound % PLAYERS;
	/* �j�񔪘A���̌�n�� */
	if (gameStat->AgariChain == -1) gameStat->AgariChain = 1;
	/* �����̏������Ɣz�v���s�Ȃ� */
	tableinit(gameStat);
	/* �z�v�I�����̏��� */
	for (PLAYER_ID i = 0; i < PLAYERS; i++) {
		gameStat->Player[i].HandStat = handUpright;
		lipai(gameStat, i);
	}
	gameStat->CurrentPlayer.Active = gameStat->GameRound % PLAYERS; // �ŏ��ɐe����̔v���s�Ȃ�
	info(_T("�z�v���������܂����B"));
	gameStat->TurnRound = 1; // �z�v���I�������1����
	/* �̑Ń��[�v */
	EndType roundEndType = Continuing;
	do {
		roundEndType = doTableTurn(gameStat);
	} while ((roundEndType == DrawRinshan) || (roundEndType == Continuing));
	int OrigHonba = gameStat->Honba, OrigTurn = gameStat->GameRound;
	/* TODO: �I�����̏��� endround GameStat, GameEnv, RoundEndType, OrigTurn, OrigHonba */
	/* TODO: await 5000 */
	// �����I������
	/* TODO: ������ڐA����
	nextRound GameStat, OrigTurn
	if (stat) {break}
	*/
	return false; // ���u��
}

/* �Q�[���J�n���� */
void startgame(gameTypeID gameType) {
	while (true) {
		/* �ϐ��ނ̏����� */
		EnvTable::TableEnvInit();
		GameTable* gameStat = initializeGameTable(gameType);
		/* TODO: ��ʏ����� vanish */
		info(_T("�Q�[���������������܂����B"));
		mihajong_socket::server::rotation_reset();
		sound::Play(sound::IDs::musTitle); // �^�C�g���Ȃ𗬂�
		/* TODO: gosub *selmode */ // �^�C�g�����
		/* TODO: gosub *chrshuffle */ // �e���߂̏���
		/* TODO: gosub *gameinit */ // �����̏���������

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
