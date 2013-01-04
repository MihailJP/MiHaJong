#include "ruletbl.h"

using namespace CodeConv;

struct GameTable;
extern GameTable GameStat;
inline bool chkGameType(const GameTable* const gameStat, gameTypeID gameType);

char RuleData::ruleConf[RULE_LINES][RULE_IN_LINE + 1];
RULETBL RuleData::Rules;
std::array<std::string, RULESIZE> RuleData::nametbl;
CSVReader::CsvVecVec RuleData::confdat;
INIParser::IniMapMap RuleData::confdict;
std::map<std::string, unsigned int> RuleData::inverse_nametbl;
std::map<std::string, std::vector<std::string> > RuleData::ruletags;
std::map<std::string, std::map<std::string, unsigned int> > RuleData::inverse_ruletags;
std::set<std::string> RuleData::nonapplicable;
const char RuleData::digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::array<tstring, RULE_PAGES> RuleData::pageCaption;

void RuleData::configinit_csv() { // �R���t�B�O�pCSV��ǂݍ���
	DWORD size = 0; const uint8_t* csv = nullptr;
	Compressed::file_confitem_csv* csvfile = new Compressed::file_confitem_csv();
	char *csvdat = new char[csvfile->getDataSize() + 4]; memset(csvdat, 0, csvfile->getDataSize()+4);
	memcpy_s(csvdat, csvfile->getDataSize()+4, csvfile->getData(), csvfile->getDataSize());
	CSVReader::parsecsv(confdat, fromUTF8(csvdat).c_str());
	delete[] csvdat; delete csvfile;

	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		std::string nomenPartisRegulae(toANSI((*k)[8])); // ���[���^�O
		unsigned int numerusPartisRegulae = _ttoi((*k)[0].c_str()); // ���[���^�O
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // ������
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // �t����

		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str()))) { // GameType���v�����ꍇ
			if ((_ttoi((*k)[0].c_str()) % RULES_IN_PAGE) == 0)
				pageCaption[_ttoi((*k)[0].c_str()) / RULES_IN_PAGE] = tstring((*k)[4]);
			ruletags[nomenPartisRegulae].clear(); inverse_ruletags[nomenPartisRegulae].clear();
			for (unsigned int index = 11; index < (*k).size(); ++index) {
				/*if ((*k)[index] == _T(">>>")) { // ��΂��悤�Ɏw�肳��Ă���Ȃ�
					ruletags[nomenPartisRegulae].push_back(_T(""));
				}
				else*/ if (!(*k)[index].empty()) { // ���݂���Ȃ�
					ruletags[nomenPartisRegulae].push_back(toANSI((*k)[index])); // ������
					inverse_ruletags[nomenPartisRegulae][toANSI((*k)[index])] = index - 11; // �t����
				}
			}
		}
		else if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str()))) { // N/A�w�肪�������ꍇ
			nonapplicable.insert(nomenPartisRegulae); // ���X�g�ɒǉ�
		}
	}

	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleData::configinit_ini() { // �R���t�B�O������ϊ��pINI��ǂݍ���
	DWORD size = 0; const uint8_t* ini = nullptr;

	Compressed::file_confitem_ini* inifile = new Compressed::file_confitem_ini();
	char *inidat = new char[inifile->getDataSize() + 4]; memset(inidat, 0, inifile->getDataSize()+4);
	memcpy_s(inidat, inifile->getDataSize()+4, inifile->getData(), inifile->getDataSize());
	INIParser::parseini(confdict, fromUTF8(inidat).c_str());
	delete[] inidat; delete inifile;
}

__declspec(dllexport) void RuleData::configinit() { // �R���t�B�O�pCSV��ǂݍ���
	configinit_csv(); configinit_ini();
	for (int i = 0; i < RULE_LINES; i++) { // ������
		memset(ruleConf[i], 0, RULE_IN_LINE + 1);
		memset(ruleConf[i], _T('-'), RULE_IN_LINE);
		for (int j = 0; j < RULE_IN_LINE; j++) {
			if ((i * RULE_IN_LINE + j) >= RULESIZE) { // �ԍ������܂�
				ruleConf[i][j] = _T('\0');
				break;
			}
			if ((ruleConf[i][j] == _T('-')) && // ���ݒ�̂܂܂�
				(!nametbl[i * RULE_IN_LINE + j].empty()) && // �󂫔Ԃł͂Ȃ���
				(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
				ruleConf[i][j] = _T('0'); // �f�t�H���g�ݒ�
		}
	}
}

void RuleData::parseRule() { // ���[���ݒ�𐔒l�ɕϊ�
	debug(_T("���[���ݒ��A�z�z��ɕϊ����܂��B"));
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == tstring::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // UI��Core ���[���ݒ�]��
	debug(_T("���[���ݒ���R�A���W���[���ɏ�������"));
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info(_T("�ݒ肪���[�h����܂����B"));
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // Core��UI ���[���ݒ�]��
	debug(_T("���[���ݒ���R�A���W���[������ǂݏo��"));
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

std::string RuleData::chkRule(std::string RuleTag) { // ���[���ݒ�^�O���擾����
	return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ���[���ݒ�
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
BOOL RuleData::chkRule(const char* RuleTag, const char* Expectation) { // ���[���ݒ�
	return chkRule(std::string(RuleTag), std::string(Expectation)) ? TRUE : FALSE;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ���[���ݒ�
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
int RuleData::getRule(uint16_t RuleID) { // ���[���ݒ���擾����(���d�l)
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int RuleData::getRuleSize(uint16_t RuleID) { // ���[�����ڂ̃A�C�e����
	return ruletags[nametbl[RuleID]].size();
}

__declspec(dllexport) void RuleData::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if ((chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str()))) ||
			(chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str())))) {
				_tcscpy_s(txt, bufsize, ((*k)[9]).c_str());
				return;
		}
	}
	_tcscpy_s(txt, bufsize, _T(""));
}

__declspec(dllexport) void RuleData::getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str()))) {
			_tcscpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) _tcscpy_s(txt, bufsize, _T("���v�O���ł͐ݒ�ł��܂���"));
			else if (chkGameType(&GameStat, SanmaX)) _tcscpy_s(txt, bufsize, _T("�O�l�ł��ł͐ݒ�ł��܂���"));
			else if (chkGameType(&GameStat, Yonma)) _tcscpy_s(txt, bufsize, _T("�l�l�ł��ł͐ݒ�ł��܂���"));
			else _tcscpy_s(txt, bufsize, _T(""));
			return;
		}
	}
	_tcscpy_s(txt, bufsize, _T(""));
}

std::string RuleData::getRuleItemTag(uint16_t RuleID, int index) {
	return getRuleItemTag(nametbl[RuleID], index);
}
std::string RuleData::getRuleItemTag(std::string RuleTag, int index) {
	if (RuleTag.empty()) // �󕶎���Ȃ�
		return std::string("");
	else if (nonapplicable.find(RuleTag) != nonapplicable.end()) // N/A�Ƃ���ꍇ
		return std::string("N/A");
	else if (ruletags.find(RuleTag) == ruletags.end()) // ���[�����ڃ^�O�����݂��Ȃ��ꍇ
		return std::string("");
	else if (ruletags[RuleTag].empty()) // ���g����Ȃ�
		return std::string("");
	else // �����łȂ����
		return ruletags[RuleTag][index];
}

__declspec(dllexport) void RuleData::getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	const std::string tag = getRuleItemTag(RuleID, index);
	if ((confdict.find(_T("dictionary")) != confdict.end()) &&
		(confdict[_T("dictionary")].find(EnsureTStr(tag)) != confdict[_T("dictionary")].end()))
		_tcscpy_s(txt, bufsize, confdict[_T("dictionary")][EnsureTStr(tag)].c_str());
	else _tcscpy_s(txt, bufsize, EnsureTStr(tag).c_str());
}

__declspec(dllexport) int RuleData::loadConfigFile(const char* const filename) {
	errno_t err; FILE* conffile;
	if (err = fopen_s(&conffile, filename, "r")) { // �I�[�v�����A���s������
		tostringstream o;
		o << _T("�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂����B�G���[�R�[�h [") << err << _T("]");
		error(o.str().c_str());
		fclose(conffile); // �t�@�C�������
		return -1;
	} else { // �������I�[�v�����ꂽ��
		fseek(conffile, 0, SEEK_END); long filesize = ftell(conffile); rewind(conffile); // �t�@�C���T�C�Y���擾
		long bufsize = (filesize | (sizeof(int) - 1)) + 1;
		char* const filedat = new char[bufsize]; // �o�b�t�@���m��
		memset(filedat, 0, bufsize); // �o�b�t�@���[���N���A
		fread_s(filedat, bufsize, sizeof(char), filesize, conffile); // �ǂݍ���
		{
			INIParser::IniMapMap config_ini; // INI�p�[�X���ʂ��i�[����u�}�b�v�̃}�b�v�v
			INIParser::parseini(config_ini, fromUTF8(filedat).c_str()); // INI���p�[�X����
			for (int i = 0; i < RULE_LINES; i++) { // ������
				memset(ruleConf[i], 0, RULE_IN_LINE + 1);
				if (((i + 1) * RULE_LINES) > RULESIZE)
					memset(ruleConf[i], _T('-'), RULESIZE % RULE_IN_LINE);
				else
					memset(ruleConf[i], _T('-'), RULE_IN_LINE);
			}
			auto& config_rules = config_ini[_T("rules")];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rules�Z�N�V�����ɂ���
				if (inverse_nametbl.find(toANSI(k->first)) != inverse_nametbl.end()) { // �L�[����������
					const std::string& rulename = toANSI(k->first); // �ʖ�������
					const uint16_t ruleid = inverse_nametbl[rulename]; // �ԍ��ɕϊ�
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/A�������΂���
						tostringstream o; o << _T("�L�[ [") << EnsureTStr(rulename) << _T("] �͐ݒ�ł��܂���B�������܂��B");
						warn(o.str().c_str());
					}
					else if(inverse_ruletags[rulename].find(toANSI(k->second)) != inverse_ruletags[rulename].end()) { // ��������Ă���ݒ�Ȃ�
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] =
							digit[inverse_ruletags[rulename][toANSI(k->second)]]; // �ݒ肷��
					}
					else {
						tostringstream o; o << _T("�L�[ [") << EnsureTStr(rulename) << _T("] �ɑ΂���l [") << k->second << 
							_T("] �͎�������Ă��܂���B�f�t�H���g�ݒ���g���܂��B");
						warn(o.str().c_str());
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] = digit[0]; // �f�t�H���g�ݒ�Ƃ���
					}
				} else { // �Ȃ�������
					tostringstream o; o << _T("�L�[ [") << k->first << _T("] �͖�������܂�");
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < RULE_LINES; i++) { // �ă`�F�b�N
				for (int j = 0; j < RULE_IN_LINE; j++) {
					if ((i * RULE_IN_LINE + j) >= RULESIZE) { // �ԍ������܂�
						ruleConf[i][j] = _T('\0');
						break;
					}
					if ((ruleConf[i][j] == _T('-')) && // ���ݒ�̂܂܂�
						(!nametbl[i * RULE_IN_LINE + j].empty()) && // �󂫔Ԃł͂Ȃ���
						(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
						ruleConf[i][j] = _T('0'); // �f�t�H���g�ݒ�
				}
			}
			debug(_T("���݂̃��[���ݒ�̓����\��")); for (int i = 0; i < RULE_LINES; i++) debug(EnsureTStr(ruleConf[i]));
			parseRule(); // �f�[�^�ϊ�
		}
		delete[] filedat; // �o�b�t�@�����
		fclose(conffile); // �t�@�C�������
		info(tstring(tstring(_T("�ݒ�t�@�C�� [")) + tstring(EnsureTStr(filename)) + tstring(_T("] ��ǂݍ��݂܂����B"))).c_str());
		return 0;
	}
}


__declspec(dllexport) int RuleData::saveConfigFile(const char* const filename) {
	debug(_T("���݂̃��[���ݒ�̓����\��")); for (int i = 0; i < RULE_LINES; i++) debug(EnsureTStr(ruleConf[i]));
	std::ofstream file; // �f�t�H���g�R���X�g���N�^�ŏ�����
	auto chkerr = [](std::ofstream& file, const char* const filename) -> void {
		if (file.bad()) throw std::runtime_error(std::string("�t�@�C�� [") + std::string(filename) +
			std::string("] �̏������ݎ��ɒv���I�G���[���������܂����B�ݒ�͕ۑ�����܂���B")); // ���s�������O�𓊂���
		else if (file.fail()) throw std::runtime_error(std::string("�t�@�C�� [") + std::string(filename) +
			std::string("] �̏������݂Ɏ��s���܂����B�ݒ�͕ۑ�����܂���B")); // ���s�������O�𓊂���
	};
	try {
		file.open(filename, std::ios::out | std::ios::trunc); // �㏑���e�L�X�g���[�h�ŁA�J��
		if (!file) throw std::runtime_error(std::string("�t�@�C�� [") + std::string(filename) +
			std::string("] ���������݃��[�h�ŊJ���܂���B�ݒ�͕ۑ�����܂���B")); // ���s�������O�𓊂���
		file << toUTF8(_T("[rules]")) << std::endl; // �Z�N�V������
		chkerr(file, filename); // ���s�������O�𓊂���
		for (auto k = nametbl.begin(); k != nametbl.end(); ++k) {
			if ((!(k->empty())) && (nonapplicable.find(*k) == nonapplicable.end())) { // �L���Ȃ�
				file << toUTF8(EnsureTStr(*k)) << toUTF8(_T("=")) << toUTF8(EnsureTStr(chkRule(*k))) << std::endl; // �ݒ�f�[�^����������
				chkerr(file, filename); // ���s�������O�𓊂���
			}
		}
		info(tstring(_T("�t�@�C�� [")) + tstring(EnsureTStr(filename)) + tstring(_T("] �ɐݒ��ۑ����܂����B")).c_str());
		return 0;
	}
	catch (std::runtime_error& e) { // �������ݎ��s�I�I
		error(EnsureTStr(e.what()));
		MessageBox(nullptr, EnsureTStr(e.what()).c_str(), _T("�������ݎ��s"), MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
		return -1;
	}
}
std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if (toANSI((*k)[8]) != RuleTag) continue;
		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str())))
				return toANSI((*k)[3]);
		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str())))
				return "";
	}
	return "";
}

__declspec(dllexport) BOOL RuleData::reqFailed(uint16_t RuleID, const int* const ruleStat) {
	auto checker = new ReqChecker();
	bool flag = checker->reqFailed(nametbl[RuleID], getRuleMaskExpr(nametbl[RuleID]), ruleStat);
	delete checker;
	return flag ? 1 : 0;
}

// -------------------------------------------------------------------------

RuleData::ReqChecker::ReqChecker () {
	myState = luaL_newstate();
	lua_register(myState, "chk", check);
}
RuleData::ReqChecker::~ReqChecker () {
	lua_close(myState);
}
const int* RuleData::ReqChecker::ourRuleStat = nullptr;

int RuleData::ReqChecker::check (lua_State* L) {
	int n = lua_gettop(L);
	if (n < 2) luaL_error(L, "Not enough arguments");
	const char* ruleTag = lua_tostring(L, 1);
	const char* itemTag = lua_tostring(L, 2);
	//lua_pushboolean(L, chkRule(ruleTag, itemTag));
	if (inverse_nametbl.find(ruleTag) == inverse_nametbl.end())
		return luaL_error(L, "Unrecognized rule key: %s", ruleTag);
	else if (inverse_ruletags[ruleTag].find(itemTag) == inverse_ruletags[ruleTag].end())
		return luaL_error(L, "Unrecognized rule value: %s", itemTag);
	lua_pushboolean(L, ourRuleStat[inverse_nametbl[ruleTag]] == inverse_ruletags[ruleTag][itemTag]);
	return 1;
}

bool RuleData::ReqChecker::reqFailed
	(const std::string& ruleTag, const std::string& expression, const int* const ruleStat)
{
	if (expression.empty()) {
		return false;
	}
	else {
		ourRuleStat = ruleStat;
		std::string expr = std::string("result = ") + expression;
		if (luaL_dostring(myState, expr.c_str())) return true;
		lua_getglobal(myState, "result");
		bool ans = lua_toboolean(myState, -1);
		lua_pop(myState, 1);
		return ans;
	}
}

// -------------------------------------------------------------------------

__declspec(dllexport) void RuleData::getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page) {
	_tcscpy_s(caption, bufsize, pageCaption[page].c_str());
}
