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

void RuleData::configinit_csv() { // コンフィグ用CSVを読み込む
	DWORD size = 0; const uint8_t* csv = nullptr;
	Compressed::file_confitem_csv* csvfile = new Compressed::file_confitem_csv();
	char *csvdat = new char[csvfile->getDataSize() + 4]; memset(csvdat, 0, csvfile->getDataSize()+4);
	memcpy_s(csvdat, csvfile->getDataSize()+4, csvfile->getData(), csvfile->getDataSize());
	CSVReader::parsecsv(confdat, fromUTF8(csvdat).c_str());
	delete[] csvdat; delete csvfile;

	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		std::string nomenPartisRegulae(toANSI((*k)[8])); // ルールタグ
		unsigned int numerusPartisRegulae = _ttoi((*k)[0].c_str()); // ルールタグ
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // 順方向
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // 逆方向

		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str()))) { // GameType合致した場合
			if ((_ttoi((*k)[0].c_str()) % RULES_IN_PAGE) == 0)
				pageCaption[_ttoi((*k)[0].c_str()) / RULES_IN_PAGE] = tstring((*k)[4]);
			ruletags[nomenPartisRegulae].clear(); inverse_ruletags[nomenPartisRegulae].clear();
			for (unsigned int index = 11; index < (*k).size(); ++index) {
				/*if ((*k)[index] == _T(">>>")) { // 飛ばすように指定されているなら
					ruletags[nomenPartisRegulae].push_back(_T(""));
				}
				else*/ if (!(*k)[index].empty()) { // 存在するなら
					ruletags[nomenPartisRegulae].push_back(toANSI((*k)[index])); // 順方向
					inverse_ruletags[nomenPartisRegulae][toANSI((*k)[index])] = index - 11; // 逆方向
				}
			}
		}
		else if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str()))) { // N/A指定があった場合
			nonapplicable.insert(nomenPartisRegulae); // リストに追加
		}
	}

	assert(inverse_ruletags["game_length"]["east_wind_game"] == 1);
	assert(ruletags["game_length"][1] == "east_wind_game");
}

void RuleData::configinit_ini() { // コンフィグ文字列変換用INIを読み込む
	DWORD size = 0; const uint8_t* ini = nullptr;

	Compressed::file_confitem_ini* inifile = new Compressed::file_confitem_ini();
	char *inidat = new char[inifile->getDataSize() + 4]; memset(inidat, 0, inifile->getDataSize()+4);
	memcpy_s(inidat, inifile->getDataSize()+4, inifile->getData(), inifile->getDataSize());
	INIParser::parseini(confdict, fromUTF8(inidat).c_str());
	delete[] inidat; delete inifile;
}

__declspec(dllexport) void RuleData::configinit() { // コンフィグ用CSVを読み込む
	configinit_csv(); configinit_ini();
	for (int i = 0; i < RULE_LINES; i++) { // 初期化
		memset(ruleConf[i], 0, RULE_IN_LINE + 1);
		memset(ruleConf[i], _T('-'), RULE_IN_LINE);
		for (int j = 0; j < RULE_IN_LINE; j++) {
			if ((i * RULE_IN_LINE + j) >= RULESIZE) { // 番号ここまで
				ruleConf[i][j] = _T('\0');
				break;
			}
			if ((ruleConf[i][j] == _T('-')) && // 未設定のままで
				(!nametbl[i * RULE_IN_LINE + j].empty()) && // 空き番ではなくて
				(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/Aではないなら
				ruleConf[i][j] = _T('0'); // デフォルト設定
		}
	}
}

void RuleData::parseRule() { // ルール設定を数値に変換
	debug(_T("ルール設定を連想配列に変換します。"));
	for (int i = 0; i < RULESIZE; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / RULE_IN_LINE][i % RULE_IN_LINE] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == tstring::npos) ? 0 : (uint8_t)idx);
	}
}

__declspec(dllexport) void RuleData::storeRule(const char** ruleTxt) { // UI→Core ルール設定転送
	debug(_T("ルール設定をコアモジュールに書き込み"));
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleConf[i], ruleTxt[i], RULE_IN_LINE);
	parseRule();
	info(_T("設定がロードされました。"));
}

__declspec(dllexport) void RuleData::exportRule(char** ruleTxt) { // Core→UI ルール設定転送
	debug(_T("ルール設定をコアモジュールから読み出し"));
	for (int i = 0; i < RULE_LINES; i++)
		memcpy(ruleTxt[i], ruleConf[i], RULE_IN_LINE);
}

std::string RuleData::chkRule(std::string RuleTag) { // ルール設定タグを取得する
	return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
bool RuleData::chkRule(std::string RuleTag, std::string Expectation) { // ルール設定
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
BOOL RuleData::chkRule(const char* RuleTag, const char* Expectation) { // ルール設定
	return chkRule(std::string(RuleTag), std::string(Expectation)) ? TRUE : FALSE;
}
bool RuleData::chkRuleApplied(std::string RuleTag) { // ルール設定
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
int RuleData::getRule(uint16_t RuleID) { // ルール設定を取得する(旧仕様)
	return Rules[nametbl[RuleID]];
}
__declspec(dllexport) int RuleData::getRuleSize(uint16_t RuleID) { // ルール項目のアイテム数
	return ruletags[nametbl[RuleID]].size();
}

__declspec(dllexport) void RuleData::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
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
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[1].c_str()))) {
			_tcscpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
		}
		else if (chkGameType(&GameStat, (gameTypeID)_ttoi((*k)[2].c_str()))) {
			if (chkGameType(&GameStat, SanmaS)) _tcscpy_s(txt, bufsize, _T("数牌三麻では設定できません"));
			else if (chkGameType(&GameStat, SanmaX)) _tcscpy_s(txt, bufsize, _T("三人打ちでは設定できません"));
			else if (chkGameType(&GameStat, Yonma)) _tcscpy_s(txt, bufsize, _T("四人打ちでは設定できません"));
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

__declspec(dllexport) void RuleData::getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	const std::string tag = getRuleItemTag(RuleID, index);
	if ((confdict.find(_T("dictionary")) != confdict.end()) &&
		(confdict[_T("dictionary")].find(EnsureTStr(tag)) != confdict[_T("dictionary")].end()))
		_tcscpy_s(txt, bufsize, confdict[_T("dictionary")][EnsureTStr(tag)].c_str());
	else _tcscpy_s(txt, bufsize, EnsureTStr(tag).c_str());
}

__declspec(dllexport) int RuleData::loadConfigFile(const char* const filename) {
	errno_t err; FILE* conffile;
	if (err = fopen_s(&conffile, filename, "r")) { // オープンし、失敗したら
		tostringstream o;
		o << _T("設定ファイルのオープンに失敗しました。エラーコード [") << err << _T("]");
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
			INIParser::parseini(config_ini, fromUTF8(filedat).c_str()); // INIをパースする
			for (int i = 0; i < RULE_LINES; i++) { // 初期化
				memset(ruleConf[i], 0, RULE_IN_LINE + 1);
				if (((i + 1) * RULE_LINES) > RULESIZE)
					memset(ruleConf[i], _T('-'), RULESIZE % RULE_IN_LINE);
				else
					memset(ruleConf[i], _T('-'), RULE_IN_LINE);
			}
			auto& config_rules = config_ini[_T("rules")];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rulesセクションについて
				if (inverse_nametbl.find(toANSI(k->first)) != inverse_nametbl.end()) { // キーがあったら
					const std::string& rulename = toANSI(k->first); // 別名をつける
					const uint16_t ruleid = inverse_nametbl[rulename]; // 番号に変換
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/Aだったばあい
						tostringstream o; o << _T("キー [") << EnsureTStr(rulename) << _T("] は設定できません。無視します。");
						warn(o.str().c_str());
					}
					else if(inverse_ruletags[rulename].find(toANSI(k->second)) != inverse_ruletags[rulename].end()) { // 実装されている設定なら
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] =
							digit[inverse_ruletags[rulename][toANSI(k->second)]]; // 設定する
					}
					else {
						tostringstream o; o << _T("キー [") << EnsureTStr(rulename) << _T("] に対する値 [") << k->second << 
							_T("] は実装されていません。デフォルト設定を使います。");
						warn(o.str().c_str());
						ruleConf[ruleid / RULE_IN_LINE][ruleid % RULE_IN_LINE] = digit[0]; // デフォルト設定とする
					}
				} else { // なかったら
					tostringstream o; o << _T("キー [") << k->first << _T("] は無視されます");
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < RULE_LINES; i++) { // 再チェック
				for (int j = 0; j < RULE_IN_LINE; j++) {
					if ((i * RULE_IN_LINE + j) >= RULESIZE) { // 番号ここまで
						ruleConf[i][j] = _T('\0');
						break;
					}
					if ((ruleConf[i][j] == _T('-')) && // 未設定のままで
						(!nametbl[i * RULE_IN_LINE + j].empty()) && // 空き番ではなくて
						(nonapplicable.find(nametbl[i * RULE_IN_LINE + j]) == nonapplicable.end())) // N/Aではないなら
						ruleConf[i][j] = _T('0'); // デフォルト設定
				}
			}
			debug(_T("現在のルール設定の内部表現")); for (int i = 0; i < RULE_LINES; i++) debug(EnsureTStr(ruleConf[i]));
			parseRule(); // データ変換
		}
		delete[] filedat; // バッファを解放
		fclose(conffile); // ファイルを閉じる
		info(tstring(tstring(_T("設定ファイル [")) + tstring(EnsureTStr(filename)) + tstring(_T("] を読み込みました。"))).c_str());
		return 0;
	}
}


__declspec(dllexport) int RuleData::saveConfigFile(const char* const filename) {
	debug(_T("現在のルール設定の内部表現")); for (int i = 0; i < RULE_LINES; i++) debug(EnsureTStr(ruleConf[i]));
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
		file << toUTF8(_T("[rules]")) << std::endl; // セクション名
		chkerr(file, filename); // 失敗したら例外を投げる
		for (auto k = nametbl.begin(); k != nametbl.end(); ++k) {
			if ((!(k->empty())) && (nonapplicable.find(*k) == nonapplicable.end())) { // 有効なら
				file << toUTF8(EnsureTStr(*k)) << toUTF8(_T("=")) << toUTF8(EnsureTStr(chkRule(*k))) << std::endl; // 設定データを書き込み
				chkerr(file, filename); // 失敗したら例外を投げる
			}
		}
		info(tstring(_T("ファイル [")) + tstring(EnsureTStr(filename)) + tstring(_T("] に設定を保存しました。")).c_str());
		return 0;
	}
	catch (std::runtime_error& e) { // 書き込み失敗！！
		error(EnsureTStr(e.what()));
		MessageBox(nullptr, EnsureTStr(e.what()).c_str(), _T("書き込み失敗"), MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
		return -1;
	}
}
std::string RuleData::getRuleMaskExpr(const std::string& RuleTag) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
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
