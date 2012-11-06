#include "ruletbl.h"

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
std::array<std::string, RULE_PAGES> RuleData::pageCaption;

void RuleData::configinit_csv() { // �R���t�B�O�pCSV��ǂݍ���
	DWORD size = 0; const uint8_t* csv = nullptr;
	LoadFileInResource(IDR_CSV_TABL1, CSV_TABLE, size, csv);
	char *csvdat = new char[size + 4]; memset(csvdat, 0, size+4); memcpy_s(csvdat, size+4, csv, size);
	CSVReader::parsecsv(confdat, csvdat);
	delete[] csvdat;

	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		std::string nomenPartisRegulae = (*k)[8]; // ���[���^�O
		unsigned int numerusPartisRegulae = std::atoi((*k)[0].c_str()); // ���[���^�O
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // ������
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // �t����

		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) { // GameType���v�����ꍇ
			if ((atoi((*k)[0].c_str()) % RULES_IN_PAGE) == 0)
				pageCaption[atoi((*k)[0].c_str()) / RULES_IN_PAGE] = std::string((*k)[4]);
			ruletags[nomenPartisRegulae].clear(); inverse_ruletags[nomenPartisRegulae].clear();
			for (unsigned int index = 11; index < (*k).size(); ++index) {
				/*if ((*k)[index] == ">>>") { // ��΂��悤�Ɏw�肳��Ă���Ȃ�
					ruletags[nomenPartisRegulae].push_back("");
				}
				else*/ if (!(*k)[index].empty()) { // ���݂���Ȃ�
					ruletags[nomenPartisRegulae].push_back((*k)[index]); // ������
					inverse_ruletags[nomenPartisRegulae][(*k)[index]] = index - 11; // �t����
				}
			}
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) { // N/A�w�肪�������ꍇ
			nonapplicable.insert(nomenPartisRegulae); // ���X�g�ɒǉ�
		}
	}

	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleData::configinit_ini() { // �R���t�B�O������ϊ��pINI��ǂݍ���
	DWORD size = 0; const uint8_t* ini = nullptr;
	LoadFileInResource(IDR_INI_FIL1, INI_FILE, size, ini);
	char *inidat = new char[size + 4]; memset(inidat, 0, size+4); memcpy_s(inidat, size+4, ini, size);
	INIParser::parseini(confdict, inidat);
	delete[] inidat;
}

__declspec(dllexport) void RuleData::configinit() { // �R���t�B�O�pCSV��ǂݍ���
	configinit_csv(); configinit_ini();
	for (int i = 0; i < RULE_LINES; i++) { // ������
		memset(ruleConf[i], 0, RULE_IN_LINE + 1);
		memset(ruleConf[i], '-', RULE_IN_LINE);
		for (int j = 0; j < RULE_IN_LINE; j++) {
			if ((i * RULE_IN_LINE + j) >= RULESIZE) { // �ԍ������܂�
				ruleConf[i][j] = '\0';
				break;
			}
			if ((ruleConf[i][j] == '-') && // ���ݒ�̂܂܂�
				(!nametbl[i * RULE_IN_LINE + j].empty()) && // �󂫔Ԃł͂Ȃ���
				(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
				ruleConf[i][j] = '0'; // �f�t�H���g�ݒ�
		}
	}
}

void RuleData::parseRule() { // ���[���ݒ�𐔒l�ɕϊ�
	debug("���[���ݒ��A�z�z��ɕϊ����܂��B");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // HSP��C++ ���[���ݒ�]��
	debug("HSP��C++ ���[���]��");
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info("�ݒ肪���[�h����܂����B");
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // C++��HSP ���[���ݒ�]��
	debug("C++��HSP ���[���]��");
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

std::string RuleData::chkRule(std::string RuleTag) { // ���[���ݒ�^�O���擾����
	return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ���[���ݒ�
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
__declspec(dllexport) int RuleData::chkRule_hsp(const char* const RuleTag, const char* const Expectation) { // ���[���ݒ�`�F�b�N(HSP����̔���p)
	return (getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation) ? 1 : 0;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ���[���ݒ�
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
uint8_t RuleData::getRule(int RuleID) { // ���[���ݒ���擾����[OBSOLETE]
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int getRule(int RuleID) { // ���[���ݒ���擾����[OBSOLETE]
	return (int)RuleData::getRule(RuleID);
}
__declspec(dllexport) int RuleData::getRuleSize(int RuleID) { // ���[�����ڂ̃A�C�e����
	return ruletags[nametbl[RuleID]].size();
}

__declspec(dllexport) void RuleData::getRuleName(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if ((chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) ||
			(chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str())))) {
				strcpy_s(txt, bufsize, ((*k)[9]).c_str());
				return;
		}
	}
	strcpy_s(txt, bufsize, "");
}

__declspec(dllexport) void RuleData::getRuleDescription(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			strcpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) strcpy_s(txt, bufsize, "���v�O���ł͐ݒ�ł��܂���");
			else if (chkGameType(&GameStat, SanmaX)) strcpy_s(txt, bufsize, "�O�l�ł��ł͐ݒ�ł��܂���");
			else if (chkGameType(&GameStat, Yonma)) strcpy_s(txt, bufsize, "�l�l�ł��ł͐ݒ�ł��܂���");
			else strcpy_s(txt, bufsize, "");
			return;
		}
	}
	strcpy_s(txt, bufsize, "");
}

std::string RuleData::getRuleItemTag(int RuleID, int index) {
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

__declspec(dllexport) void RuleData::getRuleTxt(char* const txt, int bufsize, int RuleID, int index) {
	const std::string tag = getRuleItemTag(RuleID, index);
	if ((confdict.find("dictionary") != confdict.end()) &&
		(confdict["dictionary"].find(tag) != confdict["dictionary"].end()))
		strcpy_s(txt, bufsize, confdict["dictionary"][tag].c_str());
	else strcpy_s(txt, bufsize, tag.c_str());
}

__declspec(dllexport) int RuleData::loadConfigFile(const char* const filename) {
	errno_t err; FILE* conffile;
	if (err = fopen_s(&conffile, filename, "r")) { // �I�[�v�����A���s������
		std::ostringstream o;
		o << "�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂����B�G���[�R�[�h [" << err << "]";
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
			INIParser::parseini(config_ini, filedat); // INI���p�[�X����
			for (int i = 0; i < RULE_LINES; i++) { // ������
				memset(ruleConf[i], 0, RULE_IN_LINE + 1);
				if (((i + 1) * RULE_LINES) > RULESIZE)
					memset(ruleConf[i], '-', RULESIZE % RULE_IN_LINE);
				else
					memset(ruleConf[i], '-', RULE_IN_LINE);
			}
			auto& config_rules = config_ini["rules"];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rules�Z�N�V�����ɂ���
				if (inverse_nametbl.find(k->first) != inverse_nametbl.end()) { // �L�[����������
					const std::string& rulename = k->first; // �ʖ�������
					const unsigned int ruleid = inverse_nametbl[rulename]; // �ԍ��ɕϊ�
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/A�������΂���
						std::ostringstream o; o << "�L�[ [" << rulename << "] �͐ݒ�ł��܂���B�������܂��B";
						warn(o.str().c_str());
					}
					else if(inverse_ruletags[rulename].find(k->second) != inverse_ruletags[rulename].end()) { // ��������Ă���ݒ�Ȃ�
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] =
							digit[inverse_ruletags[rulename][k->second]]; // �ݒ肷��
					}
					else {
						std::ostringstream o; o << "�L�[ [" << rulename << "] �ɑ΂���l [" << k->second << 
							"] �͎�������Ă��܂���B�f�t�H���g�ݒ���g���܂��B";
						warn(o.str().c_str());
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] = digit[0]; // �f�t�H���g�ݒ�Ƃ���
					}
				} else { // �Ȃ�������
					std::ostringstream o; o << "�L�[ [" << k->first << "] �͖�������܂�";
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < RULE_LINES; i++) { // �ă`�F�b�N
				for (int j = 0; j < RULE_IN_LINE; j++) {
					if ((i * RULE_IN_LINE + j) >= RULESIZE) { // �ԍ������܂�
						ruleConf[i][j] = '\0';
						break;
					}
					if ((ruleConf[i][j] == '-') && // ���ݒ�̂܂܂�
						(!nametbl[i * RULE_IN_LINE + j].empty()) && // �󂫔Ԃł͂Ȃ���
						(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
						ruleConf[i][j] = '0'; // �f�t�H���g�ݒ�
				}
			}
			debug("���݂̃��[���ݒ�̓����\��"); for (int i = 0; i < RULE_LINES; i++) debug(ruleConf[i]);
			parseRule(); // �f�[�^�ϊ�
		}
		delete[] filedat; // �o�b�t�@�����
		fclose(conffile); // �t�@�C�������
		info(std::string(std::string("�ݒ�t�@�C�� [") + std::string(filename) + std::string("] ��ǂݍ��݂܂����B")).c_str());
		return 0;
	}
}


__declspec(dllexport) int RuleData::saveConfigFile(const char* const filename) {
	debug("���݂̃��[���ݒ�̓����\��"); for (int i = 0; i < RULE_LINES; i++) debug(ruleConf[i]);
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
		file << "[rules]" << std::endl; // �Z�N�V������
		chkerr(file, filename); // ���s�������O�𓊂���
		for (auto k = nametbl.begin(); k != nametbl.end(); ++k) {
			if ((!(k->empty())) && (nonapplicable.find(*k) == nonapplicable.end())) { // �L���Ȃ�
				file << *k << "=" << chkRule(*k) << std::endl; // �ݒ�f�[�^����������
				chkerr(file, filename); // ���s�������O�𓊂���
			}
		}
		info(std::string("�t�@�C�� [") + std::string(filename) + std::string("] �ɐݒ��ۑ����܂����B").c_str());
		return 0;
	}
	catch (std::runtime_error& e) { // �������ݎ��s�I�I
		error(e.what());
		MessageBox(nullptr, e.what(), "�������ݎ��s", MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
		return -1;
	}
}
std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
		if ((*k)[8] != RuleTag) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str())))
				return (*k)[3];
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str())))
				return "";
	}
	return "";
}

__declspec(dllexport) int RuleData::reqFailed(int ruleID, const int* const ruleStat) {
	auto checker = new ReqChecker();
	bool flag = checker->reqFailed(nametbl[ruleID], getRuleMaskExpr(nametbl[ruleID]), ruleStat);
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

__declspec(dllexport) void RuleData::getPageCaption(char* const caption, int bufsize, int page) {
	strcpy_s(caption, bufsize, pageCaption[page].c_str());
}
