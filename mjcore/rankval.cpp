#include "rankval.h"

#include <cstring>
#include <cstdlib>
#include "decomp.h"
#include "reader/csv2arry.h"
#include "../common/strcode.h"

/* �R���X�g���N�^(�V���O���g��) */
RankVal::RankVal() {
	Compressed::file_rankval_csv rankvalCsv; // �ݒ�t�@�C��
	char* csvDatUtf8 = new char[rankvalCsv.getDataSize() + 4]; // �f�[�^�o�b�t�@(UTF-8�`��)
	memset(csvDatUtf8, 0, rankvalCsv.getDataSize() + 4);
	memcpy(csvDatUtf8, reinterpret_cast<const char*>(rankvalCsv.getData()), rankvalCsv.getDataSize()); // �f�[�^�ǂݍ���
	CSVReader::CsvVecVec parsedCsv; // �p�[�X���ꂽCSV�������ɓ���
	CodeConv::tstring csvDat(CodeConv::fromUTF8(csvDatUtf8)); // �����R�[�h�ϊ�(���̃f�[�^�̏ꍇ���܂�Ӗ���������)
	delete[] csvDatUtf8;
	CSVReader::parsecsv(parsedCsv, csvDat.c_str()); // CSV���p�[�X

	/* ���l�ɒ����Ċi�[ */
	for (int i = 1 /* �ŏ��̍s�͌��o���Ȃ̂Ŕ�΂� */; i < parsedCsv.size(); ++i) {
		RankValSet values;
		for (int j = 0; j < Players + 1; ++j)
			for (int k = 0; k < Players - 1; ++k)
				values[j][k] = _ttoi(parsedCsv[i][j * (Players - 1) + k + 1].c_str());
		const std::string key(CodeConv::toANSI(parsedCsv[i][0]));
		rankValueMap.insert(std::make_pair(key, values));
	}
}

/* �V���O���g�� �A�N�Z�T */
RankVal* RankVal::Instantiate() {
	static RankVal myInstance;
	return &myInstance;
}

/* �l���擾 */
int RankVal::getRankVal(const GameTable* gameStat, const std::string& ruletag, unsigned playersAboveBase, unsigned rank) const {
	using namespace CodeConv;
	const std::string ruleTagVal =
		std::string(gameStat->chkGameType(SanmaT) ? "T:" : "Q:") + ruletag;
	if (rankValueMap.find(ruleTagVal) == rankValueMap.end()) { /* Index error */
		tstring msg =
			tstring(_T("getRankVal: �Ή����Ă��Ȃ����[���^�O�ł� [")) +
			EnsureTStr(ruletag) + tstring(_T("]"));
		warn(msg.c_str());
		return 0;
	} else if (playersAboveBase > Players) { /* Index error */
		tostringstream o; o << _T("getRankVal: playersAboveBase ����������������܂��� [") << playersAboveBase << _T(']');
		warn(o.str().c_str());
		return 0;
	} else if ((rank <= 0) || (rank > Players)) { /* Index error */
		tostringstream o; o << _T("getRankVal: rank ����������������܂��� [") << rank << _T(']');
		warn(o.str().c_str());
		return 0;
	} else if (rank == 1) { /* �g�b�v�̏ꍇ */
		return -1 * (
			rankValueMap.at(ruleTagVal)[playersAboveBase][0] +
			rankValueMap.at(ruleTagVal)[playersAboveBase][1] +
			rankValueMap.at(ruleTagVal)[playersAboveBase][2]);
	} else { /* �g�b�v�ȊO�̏ꍇ */
		//return rankValueMap[ruleTagVal][playersAboveBase][rank - 2]; // �Ȃ���operator[]���ƃG���[�ɂȂ�B�Ӗ��s���B
		return rankValueMap.at(ruleTagVal)[playersAboveBase][rank - 2];
	}
}
