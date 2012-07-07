#ifndef HAIFU_H
#define HAIFU_H

#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include "gametbl.h"
#include "largenum.h"
#include "func.h"
#include "envtbl.h"

// ���̂Ƃ��낱���ł����g���ĂȂ����牼�u���B
// �ڐA���i��ŕʂ̂Ƃ���ł��g���悤�Ȃ�ړ����܂�

/* �ǏI���R�[�h */
enum EndType : int { // �ǂ̏I�����R
	Ryuukyoku,       // ���ʂ̗���
	Agari,           // �N���̘a��
	Chonbo,          // ���a�ŏI��
	KyuushuKyuuhai,  // ����v
	Suukaikan,       // �l�J��
	TripleRon,       // �O�Ƙa
	SuufonRenda,     // �l���A��
	SuuchaRiichi,    // �l�l����
	NagashiMangan,   // ��������
	Disconnect,      // �ʐM�r��(�l�b�g���[�N�ΐ펞�̂�)
	Uukaikan,        // �܉�ڂ̞Ȃŗ��ǂ����ꍇ(�N�����l�Ȏq���v�̏ꍇ�̂�)
};

// �v���֌W�̃R�[�h�͂���p�̖��O��Ԃɕ����Ă����܂��傤���B
namespace haifu {
	__declspec(dllexport) void haifubufinit();
	__declspec(dllexport) void haifuinit();

	__declspec(dllexport) void haifurechaipai(const GameTable* const gameStat);

	void haifurecdora(tileCode tmpDora);
	__declspec(dllexport) void haifurecdora(int tmpDora);

	void haifurecuradora(tileCode tmpDora);
	__declspec(dllexport) void haifurecuradora(int tmpDora);

	void haifurecalicedora(tileCode tmpDora);
	__declspec(dllexport) void haifurecalicedora(int tmpDora);

	__declspec(dllexport) void haifualicedoraupd();

	__declspec(dllexport) void haifuresetalicedora();

	__declspec(dllexport) void haifurecdorap();

	__declspec(dllexport) void haifurecmota(const GameTable* const gameStat, int DiscardTileIndex);

	__declspec(dllexport) void haifurecfurikomi(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecchi(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecpon(const GameTable* const gameStat);
	__declspec(dllexport) void haifurectsumo(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecminkan(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecnorth(const GameTable* const gameStat, int DiscardTileIndex);
	__declspec(dllexport) void haifurecchanpei(const GameTable* const gameStat);
	__declspec(dllexport) void haifurecnorthproc(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecankan(const GameTable* const gameStat, int DiscardTileIndex);
	__declspec(dllexport) void haifurecchankan(const GameTable* const gameStat);
	__declspec(dllexport) void haifureckanproc(const GameTable* const gameStat);

	__declspec(dllexport) void haifurecflower(const GameTable* const gameStat, int DiscardTileIndex);

	__declspec(dllexport) void haifuwritebuffer(
		const GameTable* const gameStat, void *,
		int OrigTurn, int OrigHonba, int tmpUraFlag, int tmpAliceFlag,
		const char* ResultDesc, int RoundEndType);

	void haifusave(const GameTable* const gameStat);
	__declspec(dllexport) void haifusave();
}

#endif
