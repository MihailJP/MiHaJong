#pragma once

/* �ǏI���R�[�h */
enum EndType : int { // �ǂ̏I�����R
	DrawRinshan = -2,// �������������ɖ߂�
	Continuing,      // �p����
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