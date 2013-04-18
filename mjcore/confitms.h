#pragma once

#include <cstdio>
#include <cstdint>
#include <string>
#include <map>
#include <set>
#include <array>
#include <functional>
#include <fstream>
#include "decomp.h"
#include "reader/csv2arry.h"
#include "reader/ini2map.h"
#include "logging.h"
#include "../common/strcode.h"
#include "gametbl.h"

// -------------------------------------------------------------------------

extern GameTable GameStat;

typedef std::map<std::string, int8_t> RULETBL;

#ifndef CONFDAT_TEMPLATE
#define CONFDAT_TEMPLATE template <size_t NumOfItems, size_t PageBatch, size_t LineBatch>
#endif
#ifndef CONFDAT_CLASS
#define CONFDAT_CLASS ConfigData<NumOfItems, PageBatch, LineBatch>
#endif

// -------------------------------------------------------------------------

CONFDAT_TEMPLATE class ConfigData {
protected:
	static const unsigned Pages = (NumOfItems + PageBatch - 1) / PageBatch;
	static const unsigned Lines = (NumOfItems + LineBatch - 1) / LineBatch;
protected:
	CodeConv::tstring mySectionName;
protected:
	char ruleConf[Lines][LineBatch + 1];
	RULETBL Rules;
	std::array<std::string, NumOfItems> nametbl;
	CSVReader::CsvVecVec confdat;
	INIParser::IniMapMap confdict;
	std::map<std::string, unsigned int> inverse_nametbl;
	std::map<std::string, std::vector<std::string> > ruletags;
	std::map<std::string, std::map<std::string, unsigned int> > inverse_ruletags;
	std::set<std::string> nonapplicable;
	std::map<std::string, std::string> rulemask_expr;
	std::array<CodeConv::tstring, Pages> pageCaption;
	std::map<std::string, std::string> freeval_expr;
	static const char digit[];
protected:
	virtual void configinit_csv() = 0;
	void configinit_csv(Compressed::Data* csvfile);
	virtual void configinit_ini() = 0;
	void configinit_ini(Compressed::Data* inifile);
	virtual void parseRule();
public:
	virtual void configinit();
public:
	void storeRule(const char** ruleTxt);
	void exportRule(char** ruleTxt);
public:
	std::string chkRule(std::string RuleTag);
	std::string chkRule(uint16_t RuleID);
	bool chkRule(std::string RuleTag, std::string Expectation);
	bool chkRuleApplied(std::string RuleTag);
	inline int getRule(std::string RuleTag) {return Rules[RuleTag];}
	int getRule(uint16_t RuleID);
	int getRuleSize(uint16_t RuleID);
public:
	void getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	void getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID);
	void getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index);
	std::string getRuleItemTag(uint16_t RuleID, int index);
	std::string getRuleItemTag(std::string RuleTag, int index);
public:
	int loadConfigFile(const char* const filename);
	int saveConfigFile(const char* const filename);
public:
	std::string getRuleMaskExpr(const std::string& RuleTag);
	virtual bool reqFailed(uint16_t ruleID, const int* const ruleStat);
public:
	void getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page);
	const char* ruleDigit() {return digit;}
	void forEachRule(std::function<void (std::string, std::string)> f);
public:
	unsigned getRuleStrBufLen(uint16_t RuleID);
	unsigned getRuleStrBufLen(std::string RuleTag);
public:
	void setFreeStr(std::string RuleTag, std::string data);
	void setFreeStr(uint16_t RuleID, std::string data);
public:
	CONFDAT_CLASS(CodeConv::tstring sectionName = _T("rules")) {
		mySectionName = sectionName;
	}
	virtual ~ConfigData() {}
};

// -------------------------------------------------------------------------

CONFDAT_TEMPLATE const char CONFDAT_CLASS::digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit_csv(Compressed::Data* csvfile) { // コンフィグ用CSVを読み込む
	using namespace CodeConv;
#ifdef _WIN32
	DWORD size = 0;
#else /*_WIN32*/
	size_t size = 0;
#endif /*_WIN32*/
	const uint8_t* csv = nullptr;
	char *csvdat = new char[csvfile->getDataSize() + 4]; memset(csvdat, 0, csvfile->getDataSize()+4);
#ifdef _MSC_VER
	memcpy_s(csvdat, csvfile->getDataSize()+4, csvfile->getData(), csvfile->getDataSize());
#else
	memcpy(csvdat, csvfile->getData(), csvfile->getDataSize());
#endif
	CSVReader::parsecsv(confdat, fromUTF8(csvdat).c_str());
	delete[] csvdat;

	for (auto k = confdat.begin(); k != confdat.end(); ++k) { // 名前テーブル
		std::string nomenPartisRegulae(toANSI((*k)[8])); // ルールタグ
		unsigned int numerusPartisRegulae = _ttoi((*k)[0].c_str()); // ルールタグ
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // 順方向
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // 逆方向

		if (_ttoi((*k)[7].c_str())) // 自由入力
			freeval_expr.insert(std::make_pair(nomenPartisRegulae, std::string(toANSI((*k)[11]))));

		if (((*k)[1].empty()) || (GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str())))) { // GameType合致した場合
			if ((_ttoi((*k)[0].c_str()) % PageBatch) == 0)
				pageCaption[_ttoi((*k)[0].c_str()) / PageBatch] = tstring((*k)[4]);
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
		else if ((!(*k)[1].empty()) && (GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str())))) { // N/A指定があった場合
			nonapplicable.insert(nomenPartisRegulae); // リストに追加
		}

		// ルール設定画面のマスクデータ
		if (GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str())))
			rulemask_expr[nomenPartisRegulae] = toANSI((*k)[3]);
		else if (GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str())))
			rulemask_expr[nomenPartisRegulae] = "";
	}
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit_ini(Compressed::Data* inifile) { // コンフィグ文字列変換用INIを読み込む
	using namespace CodeConv;
#ifdef _WIN32
	DWORD size = 0;
#else /*_WIN32*/
	size_t size = 0;
#endif /*_WIN32*/
	const uint8_t* ini = nullptr;
	char *inidat = new char[inifile->getDataSize() + 4]; memset(inidat, 0, inifile->getDataSize()+4);
#ifdef _MSC_VER
	memcpy_s(inidat, inifile->getDataSize()+4, inifile->getData(), inifile->getDataSize());
#else
	memcpy(inidat, inifile->getData(), inifile->getDataSize());
#endif
	INIParser::parseini(confdict, fromUTF8(inidat).c_str());
	delete[] inidat;
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit() { // コンフィグ用CSVを読み込む
	configinit_csv(); configinit_ini();
	for (int i = 0; i < Lines; i++) { // 初期化
		memset(ruleConf[i], 0, LineBatch + 1);
		memset(ruleConf[i], _T('-'), LineBatch);
		for (int j = 0; j < LineBatch; j++) {
			if ((i * LineBatch + j) >= NumOfItems) { // 番号ここまで
				ruleConf[i][j] = _T('\0');
				break;
			}
			if ((ruleConf[i][j] == _T('-')) && // 未設定のままで
				(!nametbl[i * LineBatch + j].empty()) && // 空き番ではなくて
				(nonapplicable.find(nametbl[i * LineBatch + j]) == nonapplicable.end())) // N/Aではないなら
				ruleConf[i][j] = _T('0'); // デフォルト設定
		}
	}
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::parseRule() { // ルール設定を数値に変換
	using namespace CodeConv;
	debug(_T("ルール設定を連想配列に変換します。"));
	for (int i = 0; i < NumOfItems; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / LineBatch][i % LineBatch] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == tstring::npos) ? 0 : (uint8_t)idx);
	}
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::storeRule(const char** ruleTxt) { // UI→Core ルール設定転送
	debug(_T("ルール設定をコアモジュールに書き込み"));
	for (int i = 0; i < Lines; i++)
		memcpy(ruleConf[i], ruleTxt[i], LineBatch);
	parseRule();
	info(_T("設定がロードされました。"));
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::exportRule(char** ruleTxt) { // Core→UI ルール設定転送
	debug(_T("ルール設定をコアモジュールから読み出し"));
	for (int i = 0; i < Lines; i++)
		memcpy(ruleTxt[i], ruleConf[i], LineBatch);
}

CONFDAT_TEMPLATE std::string CONFDAT_CLASS::chkRule(std::string RuleTag) { // ルール設定タグを取得する
	if (freeval_expr.find(RuleTag) != freeval_expr.end()) // 自由入力の時
		return freeval_expr[RuleTag];
	else // 選択項目の時
		return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
CONFDAT_TEMPLATE std::string CONFDAT_CLASS::chkRule(uint16_t RuleID) { // ルール設定タグを取得する
	return chkRule(nametbl[RuleID]);
}
CONFDAT_TEMPLATE bool CONFDAT_CLASS::chkRule(std::string RuleTag, std::string Expectation) { // ルール設定
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
CONFDAT_TEMPLATE bool CONFDAT_CLASS::chkRuleApplied(std::string RuleTag) { // ルール設定
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::getRule(uint16_t RuleID) { // ルール設定を取得する(旧仕様)
	return Rules[nametbl[RuleID]];
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::getRuleSize(uint16_t RuleID) { // ルール項目のアイテム数
	return ruletags[nametbl[RuleID]].size();
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if (((*k)[1].empty()) || ((*k)[2].empty()) ||
			(GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str()))) ||
			(GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str())))) {
#ifdef _MSC_VER
				_tcscpy_s(txt, bufsize, ((*k)[9]).c_str());
#else
				_tcsncpy(txt, ((*k)[9]).c_str(), bufsize);
#endif
				return;
		}
	}
#ifdef _MSC_VER
	_tcscpy_s(txt, bufsize, _T(""));
#else
	_tcsncpy(txt, _T(""), bufsize);
#endif
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::getRuleDescription(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 名前テーブル
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if (((*k)[1].empty()) || (GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str())))) {
#ifdef _MSC_VER
			_tcscpy_s(txt, bufsize, ((*k)[10]).c_str()); return;
#else
			_tcsncpy(txt, ((*k)[10]).c_str(), bufsize); return;
#endif
		}
		else if (GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str()))) {
#ifdef _MSC_VER
			if (GameStat.chkGameType(SanmaS)) _tcscpy_s(txt, bufsize, _T("数牌三麻では設定できません"));
			else if (GameStat.chkGameType(SanmaX)) _tcscpy_s(txt, bufsize, _T("三人打ちでは設定できません"));
			else if (GameStat.chkGameType(Yonma)) _tcscpy_s(txt, bufsize, _T("四人打ちでは設定できません"));
			else _tcscpy_s(txt, bufsize, _T(""));
#else
			if (GameStat.chkGameType(SanmaS)) _tcsncpy(txt, _T("数牌三麻では設定できません"), bufsize);
			else if (GameStat.chkGameType(SanmaX)) _tcsncpy(txt, _T("三人打ちでは設定できません"), bufsize);
			else if (GameStat.chkGameType(Yonma)) _tcsncpy(txt, _T("四人打ちでは設定できません"), bufsize);
			else _tcsncpy(txt, _T(""), bufsize);
#endif
			return;
		}
	}
#ifdef _MSC_VER
	_tcscpy_s(txt, bufsize, _T(""));
#else
	_tcsncpy(txt, _T(""), bufsize);
#endif
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::getRuleTxt(LPTSTR const txt, unsigned bufsize, uint16_t RuleID, uint8_t index) {
	using namespace CodeConv;
	const std::string tag = getRuleItemTag(RuleID, index);
	if ((confdict.find(_T("dictionary")) != confdict.end()) &&
		(confdict[_T("dictionary")].find(EnsureTStr(tag)) != confdict[_T("dictionary")].end()))
#ifdef _MSC_VER
		_tcscpy_s(txt, bufsize, confdict[_T("dictionary")][EnsureTStr(tag)].c_str());
	else _tcscpy_s(txt, bufsize, EnsureTStr(tag).c_str());
#else
		_tcsncpy(txt, confdict[_T("dictionary")][EnsureTStr(tag)].c_str(), bufsize);
	else _tcsncpy(txt, EnsureTStr(tag).c_str(), bufsize);
#endif
}
CONFDAT_TEMPLATE std::string CONFDAT_CLASS::getRuleItemTag(uint16_t RuleID, int index) {
	return getRuleItemTag(nametbl[RuleID], index);
}
CONFDAT_TEMPLATE std::string CONFDAT_CLASS::getRuleItemTag(std::string RuleTag, int index) {
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

CONFDAT_TEMPLATE int CONFDAT_CLASS::loadConfigFile(const char* const filename) {
	using namespace CodeConv;
#ifdef _MSC_VER
	errno_t err;
#endif
	FILE* conffile;
#ifdef _MSC_VER
	if (err = fopen_s(&conffile, filename, "r")) { // オープンし、失敗したら
#else
	if ((conffile = fopen(filename, "r")) == nullptr) { // オープンし、失敗したら
#endif
		tostringstream o;
		o << _T("設定ファイルのオープンに失敗しました。");
#ifdef _MSC_VER
		o << _T("エラーコード [") << err << _T("]");
#endif
		error(o.str().c_str());
		//fclose(conffile); // ファイルを閉じる ←ここではconffileはぬるぽなので不要
		return -1;
	} else { // 正しくオープンされたら
		fseek(conffile, 0, SEEK_END); long filesize = ftell(conffile); rewind(conffile); // ファイルサイズを取得
		long bufsize = (filesize | (sizeof(int) - 1)) + 1;
		char* const filedat = new char[bufsize]; // バッファを確保
		memset(filedat, 0, bufsize); // バッファをゼロクリア
#ifdef _MSC_VER
		fread_s(filedat, bufsize, sizeof(char), filesize, conffile); // 読み込み
#else
		fread(filedat, sizeof(char), filesize, conffile); // 読み込み
#endif
		{
			INIParser::IniMapMap config_ini; // INIパース結果を格納する「マップのマップ」
			INIParser::parseini(config_ini, fromUTF8(filedat).c_str()); // INIをパースする
			for (int i = 0; i < Lines; i++) { // 初期化
				memset(ruleConf[i], 0, LineBatch + 1);
				if (((i + 1) * Lines) > NumOfItems)
					memset(ruleConf[i], _T('-'), NumOfItems % LineBatch);
				else
					memset(ruleConf[i], _T('-'), LineBatch);
			}
			auto& config_rules = config_ini[mySectionName];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rulesセクションについて
				if (inverse_nametbl.find(toANSI(k->first)) != inverse_nametbl.end()) { // キーがあったら
					const std::string& rulename = toANSI(k->first); // 別名をつける
					const uint16_t ruleid = inverse_nametbl[rulename]; // 番号に変換
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/Aだったばあい
						tostringstream o; o << _T("キー [") << EnsureTStr(rulename) << _T("] は設定できません。無視します。");
						warn(o.str().c_str());
					}
					else if (freeval_expr.find(rulename) != freeval_expr.end()) { // 自由入力の時
						CodeConv::tstring val(k->second);
						std::string s =
#ifdef _UNICODE
							CodeConv::toANSI(val);
#else
							val;
#endif
						freeval_expr[rulename] = s.substr(0, getRuleStrBufLen(rulename));
					}
					else if(inverse_ruletags[rulename].find(toANSI(k->second)) != inverse_ruletags[rulename].end()) { // 実装されている設定なら
						ruleConf[ruleid / LineBatch][ruleid % LineBatch] =
							digit[inverse_ruletags[rulename][toANSI(k->second)]]; // 設定する
					}
					else {
						tostringstream o; o << _T("キー [") << EnsureTStr(rulename) << _T("] に対する値 [") << k->second << 
							_T("] は実装されていません。デフォルト設定を使います。");
						warn(o.str().c_str());
						ruleConf[ruleid / LineBatch][ruleid % LineBatch] = digit[0]; // デフォルト設定とする
					}
				} else { // なかったら
					tostringstream o; o << _T("キー [") << k->first << _T("] は無視されます");
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < Lines; i++) { // 再チェック
				for (int j = 0; j < LineBatch; j++) {
					if ((i * LineBatch + j) >= NumOfItems) { // 番号ここまで
						ruleConf[i][j] = _T('\0');
						break;
					}
					if ((ruleConf[i][j] == _T('-')) && // 未設定のままで
						(!nametbl[i * LineBatch + j].empty()) && // 空き番ではなくて
						(nonapplicable.find(nametbl[i * LineBatch + j]) == nonapplicable.end())) // N/Aではないなら
						ruleConf[i][j] = _T('0'); // デフォルト設定
				}
			}
			debug(_T("現在の設定の内部表現")); for (int i = 0; i < Lines; i++) debug(EnsureTStr(ruleConf[i]));
			parseRule(); // データ変換
		}
		delete[] filedat; // バッファを解放
		fclose(conffile); // ファイルを閉じる
		info(tstring(tstring(_T("設定ファイル [")) + tstring(EnsureTStr(filename)) + tstring(_T("] を読み込みました。"))).c_str());
		return 0;
	}
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::saveConfigFile(const char* const filename) {
	using namespace CodeConv;
	debug(_T("現在の設定の内部表現")); for (int i = 0; i < Lines; i++) debug(EnsureTStr(ruleConf[i]));
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
		file << toUTF8(_T("[")) << toUTF8(mySectionName) << toUTF8(_T("]")) << std::endl; // セクション名
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
#ifdef _WIN32
		MessageBox(nullptr, EnsureTStr(e.what()).c_str(), _T("書き込み失敗"), MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
#else /*_WIN32*/
		/* TODO: 未実装箇所 */
#endif /*_WIN32*/
		return -1;
	}
}

CONFDAT_TEMPLATE std::string CONFDAT_CLASS::getRuleMaskExpr(const std::string& RuleTag) {
	if (rulemask_expr.find(RuleTag) != rulemask_expr.end())
		return rulemask_expr[RuleTag];
	else
		return "";
}
CONFDAT_TEMPLATE bool CONFDAT_CLASS::reqFailed(uint16_t ruleID, const int* const ruleStat) {
	return false;
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::getPageCaption(LPTSTR const caption, unsigned bufsize, uint8_t page) {
#ifdef _MSC_VER
	_tcscpy_s(caption, bufsize, pageCaption[page].c_str());
#else
	_tcsncpy(caption, pageCaption[page].c_str(), bufsize);
#endif
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::forEachRule(std::function<void (std::string, std::string)> f) {
	for (auto k = inverse_nametbl.begin(); k != inverse_nametbl.end(); ++k)
		f(k->first, chkRule(k->first));
}

CONFDAT_TEMPLATE unsigned CONFDAT_CLASS::getRuleStrBufLen(uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // 文字列入力の幅
		if (_ttoi((*k)[0].c_str()) != RuleID) continue;
		if (((*k)[1].empty()) || ((*k)[2].empty()) ||
			(GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str()))) ||
			(GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str()))))
			return _ttoi(((*k)[7]).c_str());
	}
	return 0;
}
CONFDAT_TEMPLATE unsigned CONFDAT_CLASS::getRuleStrBufLen(std::string RuleTag) {
	if (inverse_nametbl.find(RuleTag) != inverse_nametbl.end())
		return getRuleStrBufLen(inverse_nametbl[RuleTag]);
	else
		return 0;
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::setFreeStr(std::string RuleTag, std::string data) {
	if (freeval_expr.find(RuleTag) != freeval_expr.end()) // 自由入力の時
		freeval_expr[RuleTag] = data.substr(0, getRuleStrBufLen(RuleTag));
	// 自由入力項目でなければ何もしない
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::setFreeStr(uint16_t RuleID, std::string data) {
	setFreeStr(nametbl[RuleID], data);
}
