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

void RuleData::configinit_csv() { // コンフィグ用CSVを読み込む
	DWORD size = 0; const uint8_t* csv = nullptr;
	LoadFileInResource(IDR_CSV_TABL1, CSV_TABLE, size, csv);
	char *csvdat = new char[size + 4]; memset(csvdat, 0, size+4); memcpy_s(csvdat, size+4, csv, size);
	CSVReader::parsecsv(confdat, csvdat);
	delete[] csvdat;

	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		std::string nomenPartisRegulae = (*k)[8]; // ルールタグ
		unsigned int numerusPartisRegulae = std::atoi((*k)[0].c_str()); // ルールタグ
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // 順方向
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // 逆方向

		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) { // GameType合致した場合
			if ((atoi((*k)[0].c_str()) % RULES_IN_PAGE) == 0)
				pageCaption[atoi((*k)[0].c_str()) / RULES_IN_PAGE] = std::string((*k)[4]);
			ruletags[nomenPartisRegulae].clear(); inverse_ruletags[nomenPartisRegulae].clear();
			for (unsigned int index = 11; index < (*k).size(); ++index) {
				/*if ((*k)[index] == ">>>") { // 飛ばすように指定されているなら
					ruletags[nomenPartisRegulae].push_back("");
				}
				else*/ if (!(*k)[index].empty()) { // 存在するなら
					ruletags[nomenPartisRegulae].push_back((*k)[index]); // 順方向
					inverse_ruletags[nomenPartisRegulae][(*k)[index]] = index - 11; // 逆方向
				}
			}
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) { // N/A指定があった場合
			nonapplicable.insert(nomenPartisRegulae); // リストに追加
		}
	}

	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleData::configinit_ini() { // コンフィグ文字列変換用INIを読み込む
	DWORD size = 0; const uint8_t* ini = nullptr;
	LoadFileInResource(IDR_INI_FIL1, INI_FILE, size, ini);
	char *inidat = new char[size + 4]; memset(inidat, 0, size+4); memcpy_s(inidat, size+4, ini, size);
	INIParser::parseini(confdict, inidat);
	delete[] inidat;
}

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	configinit_csv(); configinit_ini();
	for (int i = 0; i < RULE_LINES; i++) { // 初期化
		memset(ruleConf[i], 0, RULE_IN_LINE + 1);
		memset(ruleConf[i], '-', RULE_IN_LINE);
		for (int j = 0; j < RULE_IN_LINE; j++) {
			if ((i * RULE_IN_LINE + j) >= RULESIZE) { // 番号ここまで
				ruleConf[i][j] = '\0';
				break;
			}
			if ((ruleConf[i][j] == '-') && // 未設定のままで
				(!nametbl[i * RULE_IN_LINE + j].empty()) && // 空き番ではなくて
				(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/Aではないなら
				ruleConf[i][j] = '0'; // デフォルト設定
		}
	}
}

void RuleData::parseRule() { // ルール設定を数値に変換
	debug("ルール設定を連想配列に変換します。");
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == std::string::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // HSP→C++ ルール設定転送
	debug("HSP→C++ ルール転送");
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info("設定がロードされました。");
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // C++→HSP ルール設定転送
	debug("C++→HSP ルール転送");
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

std::string RuleData::chkRule(std::string RuleTag) { // ルール設定タグを取得する
	return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ルール設定
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
__declspec(dllexport) int RuleData::chkRule_hsp(const char* const RuleTag, const char* const Expectation) { // ルール設定チェック(HSPからの判定用)
	return (getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation) ? 1 : 0;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ルール設定
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
uint8_t RuleData::getRule(int RuleID) { // ルール設定を取得する[OBSOLETE]
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int getRule(int RuleID) { // ルール設定を取得する[OBSOLETE]
	return (int)RuleData::getRule(RuleID);
}
__declspec(dllexport) int RuleData::getRuleSize(int RuleID) { // ルール項目のアイテム数
	return ruletags[nametbl[RuleID]].size();
}

__declspec(dllexport) void RuleData::getRuleName(char* const txt, int bufsize, int RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
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
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (std::atoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[1].c_str()))) {
			strcpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)std::atoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) strcpy_s(txt, bufsize, "数牌三麻では設定できません");
			else if (chkGameType(&GameStat, SanmaX)) strcpy_s(txt, bufsize, "三人打ちでは設定できません");
			else if (chkGameType(&GameStat, Yonma)) strcpy_s(txt, bufsize, "四人打ちでは設定できません");
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
	if (RuleTag.empty()) // 空文字列なら
		return std::string("");
	else if (nonapplicable.find(RuleTag) != nonapplicable.end()) // N/Aとする場合
		return std::string("N/A");
	else if (ruletags.find(RuleTag) == ruletags.end()) // ルール項目タグが存在しない場合
		return std::string("");
	else if (ruletags[RuleTag].empty()) // 中身が空なら
		return std::string("");
	else // そうでなければ
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
	if (err = fopen_s(&conffile, filename, "r")) { // オープンし、失敗したら
		std::ostringstream o;
		o << "設定ファイルのオープンに失敗しました。エラーコード [" << err << "]";
		error(o.str().c_str());
		fclose(conffile); // ファイルを閉じる
		return -1;
	} else { // 正しくオープンされたら
		fseek(conffile, 0, SEEK_END); long filesize = ftell(conffile); rewind(conffile); // ファイルサイズを取得
		long bufsize = (filesize | (sizeof(int) - 1)) + 1;
		char* const filedat = new char[bufsize]; // バッファを確保
		memset(filedat, 0, bufsize); // バッファをゼロクリア
		fread_s(filedat, bufsize, sizeof(char), filesize, conffile); // 読み込み
		{
			INIParser::IniMapMap config_ini; // INIパース結果を格納する「マップのマップ」
			INIParser::parseini(config_ini, filedat); // INIをパースする
			for (int i = 0; i < RULE_LINES; i++) { // 初期化
				memset(ruleConf[i], 0, RULE_IN_LINE + 1);
				if (((i + 1) * RULE_LINES) > RULESIZE)
					memset(ruleConf[i], '-', RULESIZE % RULE_IN_LINE);
				else
					memset(ruleConf[i], '-', RULE_IN_LINE);
			}
			auto& config_rules = config_ini["rules"];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rulesセクションについて
				if (inverse_nametbl.find(k->first) != inverse_nametbl.end()) { // キーがあったら
					const std::string& rulename = k->first; // 別名をつける
					const unsigned int ruleid = inverse_nametbl[rulename]; // 番号に変換
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/Aだったばあい
						std::ostringstream o; o << "キー [" << rulename << "] は設定できません。無視します。";
						warn(o.str().c_str());
					}
					else if(inverse_ruletags[rulename].find(k->second) != inverse_ruletags[rulename].end()) { // 実装されている設定なら
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] =
							digit[inverse_ruletags[rulename][k->second]]; // 設定する
					}
					else {
						std::ostringstream o; o << "キー [" << rulename << "] に対する値 [" << k->second << 
							"] は実装されていません。デフォルト設定を使います。";
						warn(o.str().c_str());
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] = digit[0]; // デフォルト設定とする
					}
				} else { // なかったら
					std::ostringstream o; o << "キー [" << k->first << "] は無視されます";
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < RULE_LINES; i++) { // 再チェック
				for (int j = 0; j < RULE_IN_LINE; j++) {
					if ((i * RULE_IN_LINE + j) >= RULESIZE) { // 番号ここまで
						ruleConf[i][j] = '\0';
						break;
					}
					if ((ruleConf[i][j] == '-') && // 未設定のままで
						(!nametbl[i * RULE_IN_LINE + j].empty()) && // 空き番ではなくて
						(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/Aではないなら
						ruleConf[i][j] = '0'; // デフォルト設定
				}
			}
			debug("現在のルール設定の内部表現"); for (int i = 0; i < RULE_LINES; i++) debug(ruleConf[i]);
			parseRule(); // データ変換
		}
		delete[] filedat; // バッファを解放
		fclose(conffile); // ファイルを閉じる
		info(std::string(std::string("設定ファイル [") + std::string(filename) + std::string("] を読み込みました。")).c_str());
		return 0;
	}
}


__declspec(dllexport) int RuleData::saveConfigFile(const char* const filename) {
	debug("現在のルール設定の内部表現"); for (int i = 0; i < RULE_LINES; i++) debug(ruleConf[i]);
	std::ofstream file; // デフォルトコンストラクタで初期化
	auto chkerr = [](std::ofstream& file, const char* const filename) -> void {
		if (file.bad()) throw std::runtime_error(std::string("ファイル [") + std::string(filename) +
			std::string("] の書き込み時に致命的エラーが発生しました。設定は保存されません。")); // 失敗したら例外を投げる
		else if (file.fail()) throw std::runtime_error(std::string("ファイル [") + std::string(filename) +
			std::string("] の書き込みに失敗しました。設定は保存されません。")); // 失敗したら例外を投げる
	};
	try {
		file.open(filename, std::ios::out | std::ios::trunc); // 上書きテキストモードで、開く
		if (!file) throw std::runtime_error(std::string("ファイル [") + std::string(filename) +
			std::string("] を書き込みモードで開けません。設定は保存されません。")); // 失敗したら例外を投げる
		file << "[rules]" << std::endl; // セクション名
		chkerr(file, filename); // 失敗したら例外を投げる
		for (auto k = nametbl.begin(); k != nametbl.end(); ++k) {
			if ((!(k->empty())) && (nonapplicable.find(*k) == nonapplicable.end())) { // 有効なら
				file << *k << "=" << chkRule(*k) << std::endl; // 設定データを書き込み
				chkerr(file, filename); // 失敗したら例外を投げる
			}
		}
		info(std::string("ファイル [") + std::string(filename) + std::string("] に設定を保存しました。").c_str());
		return 0;
	}
	catch (std::runtime_error& e) { // 書き込み失敗！！
		error(e.what());
		MessageBox(nullptr, e.what(), "書き込み失敗", MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
		return -1;
	}
}
std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
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
