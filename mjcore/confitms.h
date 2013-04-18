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

CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit_csv(Compressed::Data* csvfile) { // �R���t�B�O�pCSV��ǂݍ���
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

	for (auto k = confdat.begin(); k != confdat.end(); ++k) { // ���O�e�[�u��
		std::string nomenPartisRegulae(toANSI((*k)[8])); // ���[���^�O
		unsigned int numerusPartisRegulae = _ttoi((*k)[0].c_str()); // ���[���^�O
		nametbl[numerusPartisRegulae] = nomenPartisRegulae; // ������
		inverse_nametbl[nomenPartisRegulae] = numerusPartisRegulae; // �t����

		if (_ttoi((*k)[7].c_str())) // ���R����
			freeval_expr.insert(std::make_pair(nomenPartisRegulae, std::string(toANSI((*k)[11]))));

		if (((*k)[1].empty()) || (GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str())))) { // GameType���v�����ꍇ
			if ((_ttoi((*k)[0].c_str()) % PageBatch) == 0)
				pageCaption[_ttoi((*k)[0].c_str()) / PageBatch] = tstring((*k)[4]);
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
		else if ((!(*k)[1].empty()) && (GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str())))) { // N/A�w�肪�������ꍇ
			nonapplicable.insert(nomenPartisRegulae); // ���X�g�ɒǉ�
		}

		// ���[���ݒ��ʂ̃}�X�N�f�[�^
		if (GameStat.chkGameType((GameTypeID)_ttoi((*k)[1].c_str())))
			rulemask_expr[nomenPartisRegulae] = toANSI((*k)[3]);
		else if (GameStat.chkGameType((GameTypeID)_ttoi((*k)[2].c_str())))
			rulemask_expr[nomenPartisRegulae] = "";
	}
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit_ini(Compressed::Data* inifile) { // �R���t�B�O������ϊ��pINI��ǂݍ���
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
CONFDAT_TEMPLATE void CONFDAT_CLASS::configinit() { // �R���t�B�O�pCSV��ǂݍ���
	configinit_csv(); configinit_ini();
	for (int i = 0; i < Lines; i++) { // ������
		memset(ruleConf[i], 0, LineBatch + 1);
		memset(ruleConf[i], _T('-'), LineBatch);
		for (int j = 0; j < LineBatch; j++) {
			if ((i * LineBatch + j) >= NumOfItems) { // �ԍ������܂�
				ruleConf[i][j] = _T('\0');
				break;
			}
			if ((ruleConf[i][j] == _T('-')) && // ���ݒ�̂܂܂�
				(!nametbl[i * LineBatch + j].empty()) && // �󂫔Ԃł͂Ȃ���
				(nonapplicable.find(nametbl[i * LineBatch + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
				ruleConf[i][j] = _T('0'); // �f�t�H���g�ݒ�
		}
	}
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::parseRule() { // ���[���ݒ�𐔒l�ɕϊ�
	using namespace CodeConv;
	debug(_T("���[���ݒ��A�z�z��ɕϊ����܂��B"));
	for (int i = 0; i < NumOfItems; i++) {
		std::string::size_type idx = std::string(digit).find(
			ruleConf[i / LineBatch][i % LineBatch] );
		if (!(nametbl[i].empty()))
			Rules[nametbl[i]] = ((idx == tstring::npos) ? 0 : (uint8_t)idx);
	}
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::storeRule(const char** ruleTxt) { // UI��Core ���[���ݒ�]��
	debug(_T("���[���ݒ���R�A���W���[���ɏ�������"));
	for (int i = 0; i < Lines; i++)
		memcpy(ruleConf[i], ruleTxt[i], LineBatch);
	parseRule();
	info(_T("�ݒ肪���[�h����܂����B"));
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::exportRule(char** ruleTxt) { // Core��UI ���[���ݒ�]��
	debug(_T("���[���ݒ���R�A���W���[������ǂݏo��"));
	for (int i = 0; i < Lines; i++)
		memcpy(ruleTxt[i], ruleConf[i], LineBatch);
}

CONFDAT_TEMPLATE std::string CONFDAT_CLASS::chkRule(std::string RuleTag) { // ���[���ݒ�^�O���擾����
	if (freeval_expr.find(RuleTag) != freeval_expr.end()) // ���R���͂̎�
		return freeval_expr[RuleTag];
	else // �I�����ڂ̎�
		return getRuleItemTag(RuleTag, Rules[RuleTag]);
}
CONFDAT_TEMPLATE std::string CONFDAT_CLASS::chkRule(uint16_t RuleID) { // ���[���ݒ�^�O���擾����
	return chkRule(nametbl[RuleID]);
}
CONFDAT_TEMPLATE bool CONFDAT_CLASS::chkRule(std::string RuleTag, std::string Expectation) { // ���[���ݒ�
	return getRuleItemTag(RuleTag, Rules[RuleTag]) == Expectation;
}
CONFDAT_TEMPLATE bool CONFDAT_CLASS::chkRuleApplied(std::string RuleTag) { // ���[���ݒ�
	return (!chkRule(RuleTag, "no")) && (!chkRule(RuleTag, "N/A")) && (!chkRule(RuleTag, "continue"));
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::getRule(uint16_t RuleID) { // ���[���ݒ���擾����(���d�l)
	return Rules[nametbl[RuleID]];
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::getRuleSize(uint16_t RuleID) { // ���[�����ڂ̃A�C�e����
	return ruletags[nametbl[RuleID]].size();
}

CONFDAT_TEMPLATE void CONFDAT_CLASS::getRuleName(LPTSTR const txt, unsigned bufsize, uint16_t RuleID) {
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
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
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ���O�e�[�u��
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
			if (GameStat.chkGameType(SanmaS)) _tcscpy_s(txt, bufsize, _T("���v�O���ł͐ݒ�ł��܂���"));
			else if (GameStat.chkGameType(SanmaX)) _tcscpy_s(txt, bufsize, _T("�O�l�ł��ł͐ݒ�ł��܂���"));
			else if (GameStat.chkGameType(Yonma)) _tcscpy_s(txt, bufsize, _T("�l�l�ł��ł͐ݒ�ł��܂���"));
			else _tcscpy_s(txt, bufsize, _T(""));
#else
			if (GameStat.chkGameType(SanmaS)) _tcsncpy(txt, _T("���v�O���ł͐ݒ�ł��܂���"), bufsize);
			else if (GameStat.chkGameType(SanmaX)) _tcsncpy(txt, _T("�O�l�ł��ł͐ݒ�ł��܂���"), bufsize);
			else if (GameStat.chkGameType(Yonma)) _tcsncpy(txt, _T("�l�l�ł��ł͐ݒ�ł��܂���"), bufsize);
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

CONFDAT_TEMPLATE int CONFDAT_CLASS::loadConfigFile(const char* const filename) {
	using namespace CodeConv;
#ifdef _MSC_VER
	errno_t err;
#endif
	FILE* conffile;
#ifdef _MSC_VER
	if (err = fopen_s(&conffile, filename, "r")) { // �I�[�v�����A���s������
#else
	if ((conffile = fopen(filename, "r")) == nullptr) { // �I�[�v�����A���s������
#endif
		tostringstream o;
		o << _T("�ݒ�t�@�C���̃I�[�v���Ɏ��s���܂����B");
#ifdef _MSC_VER
		o << _T("�G���[�R�[�h [") << err << _T("]");
#endif
		error(o.str().c_str());
		//fclose(conffile); // �t�@�C������� �������ł�conffile�͂ʂ�ۂȂ̂ŕs�v
		return -1;
	} else { // �������I�[�v�����ꂽ��
		fseek(conffile, 0, SEEK_END); long filesize = ftell(conffile); rewind(conffile); // �t�@�C���T�C�Y���擾
		long bufsize = (filesize | (sizeof(int) - 1)) + 1;
		char* const filedat = new char[bufsize]; // �o�b�t�@���m��
		memset(filedat, 0, bufsize); // �o�b�t�@���[���N���A
#ifdef _MSC_VER
		fread_s(filedat, bufsize, sizeof(char), filesize, conffile); // �ǂݍ���
#else
		fread(filedat, sizeof(char), filesize, conffile); // �ǂݍ���
#endif
		{
			INIParser::IniMapMap config_ini; // INI�p�[�X���ʂ��i�[����u�}�b�v�̃}�b�v�v
			INIParser::parseini(config_ini, fromUTF8(filedat).c_str()); // INI���p�[�X����
			for (int i = 0; i < Lines; i++) { // ������
				memset(ruleConf[i], 0, LineBatch + 1);
				if (((i + 1) * Lines) > NumOfItems)
					memset(ruleConf[i], _T('-'), NumOfItems % LineBatch);
				else
					memset(ruleConf[i], _T('-'), LineBatch);
			}
			auto& config_rules = config_ini[mySectionName];
			for (auto k = config_rules.begin(); k != config_rules.end(); ++k) { // rules�Z�N�V�����ɂ���
				if (inverse_nametbl.find(toANSI(k->first)) != inverse_nametbl.end()) { // �L�[����������
					const std::string& rulename = toANSI(k->first); // �ʖ�������
					const uint16_t ruleid = inverse_nametbl[rulename]; // �ԍ��ɕϊ�
					if (nonapplicable.find(rulename) != nonapplicable.end()) { // N/A�������΂���
						tostringstream o; o << _T("�L�[ [") << EnsureTStr(rulename) << _T("] �͐ݒ�ł��܂���B�������܂��B");
						warn(o.str().c_str());
					}
					else if (freeval_expr.find(rulename) != freeval_expr.end()) { // ���R���͂̎�
						CodeConv::tstring val(k->second);
						std::string s =
#ifdef _UNICODE
							CodeConv::toANSI(val);
#else
							val;
#endif
						freeval_expr[rulename] = s.substr(0, getRuleStrBufLen(rulename));
					}
					else if(inverse_ruletags[rulename].find(toANSI(k->second)) != inverse_ruletags[rulename].end()) { // ��������Ă���ݒ�Ȃ�
						ruleConf[ruleid / LineBatch][ruleid % LineBatch] =
							digit[inverse_ruletags[rulename][toANSI(k->second)]]; // �ݒ肷��
					}
					else {
						tostringstream o; o << _T("�L�[ [") << EnsureTStr(rulename) << _T("] �ɑ΂���l [") << k->second << 
							_T("] �͎�������Ă��܂���B�f�t�H���g�ݒ���g���܂��B");
						warn(o.str().c_str());
						ruleConf[ruleid / LineBatch][ruleid % LineBatch] = digit[0]; // �f�t�H���g�ݒ�Ƃ���
					}
				} else { // �Ȃ�������
					tostringstream o; o << _T("�L�[ [") << k->first << _T("] �͖�������܂�");
					warn(o.str().c_str());
				}
			}
			for (int i = 0; i < Lines; i++) { // �ă`�F�b�N
				for (int j = 0; j < LineBatch; j++) {
					if ((i * LineBatch + j) >= NumOfItems) { // �ԍ������܂�
						ruleConf[i][j] = _T('\0');
						break;
					}
					if ((ruleConf[i][j] == _T('-')) && // ���ݒ�̂܂܂�
						(!nametbl[i * LineBatch + j].empty()) && // �󂫔Ԃł͂Ȃ���
						(nonapplicable.find(nametbl[i * LineBatch + j]) == nonapplicable.end())) // N/A�ł͂Ȃ��Ȃ�
						ruleConf[i][j] = _T('0'); // �f�t�H���g�ݒ�
				}
			}
			debug(_T("���݂̐ݒ�̓����\��")); for (int i = 0; i < Lines; i++) debug(EnsureTStr(ruleConf[i]));
			parseRule(); // �f�[�^�ϊ�
		}
		delete[] filedat; // �o�b�t�@�����
		fclose(conffile); // �t�@�C�������
		info(tstring(tstring(_T("�ݒ�t�@�C�� [")) + tstring(EnsureTStr(filename)) + tstring(_T("] ��ǂݍ��݂܂����B"))).c_str());
		return 0;
	}
}
CONFDAT_TEMPLATE int CONFDAT_CLASS::saveConfigFile(const char* const filename) {
	using namespace CodeConv;
	debug(_T("���݂̐ݒ�̓����\��")); for (int i = 0; i < Lines; i++) debug(EnsureTStr(ruleConf[i]));
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
		file << toUTF8(_T("[")) << toUTF8(mySectionName) << toUTF8(_T("]")) << std::endl; // �Z�N�V������
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
#ifdef _WIN32
		MessageBox(nullptr, EnsureTStr(e.what()).c_str(), _T("�������ݎ��s"), MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
#else /*_WIN32*/
		/* TODO: �������ӏ� */
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
	for (auto k = confdat.begin(); k != confdat.end(); k++) { // ��������͂̕�
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
	if (freeval_expr.find(RuleTag) != freeval_expr.end()) // ���R���͂̎�
		freeval_expr[RuleTag] = data.substr(0, getRuleStrBufLen(RuleTag));
	// ���R���͍��ڂłȂ���Ή������Ȃ�
}
CONFDAT_TEMPLATE void CONFDAT_CLASS::setFreeStr(uint16_t RuleID, std::string data) {
	setFreeStr(nametbl[RuleID], data);
}
