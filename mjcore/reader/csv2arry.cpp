#include "csv2arry.h"

#include <cstring>

void CSVReader::parsecsv (CsvVecVec& arr, LPCTSTR csv) {
	/*
	 *  CSVをパースして配列の配列オブジェクトに代入する
	 *
	 *  制限事項
	 *  ・CRを無視します
	 *  ・改行や引用符のクォートはできます
	 *  ・#で始まる行はコメントとして無視されます
	 */
	arr.clear(); // リセット
	CodeConv::tstring tmpstr; // 作業用文字列
	TCHAR tmpletter[2] = {0, 0};
	RECORD record; // レコードデータ
	bool firstchr = true; bool quoted = false; bool iscomment = false; // フラグ
	for (LPCTSTR ptr = csv; *ptr != _T('\0'); ptr++) { // ポインタのforとか使ったことあれへん……
		if (*ptr == _T('\r')) continue; // just ignore a CR
		else if ((*ptr == _T('\n')) && (!quoted)) { // new line
			if (!iscomment) { // End of the record
				record.push_back(tmpstr); tmpstr.clear();
				arr.push_back(record); record.clear();
			}
			firstchr = true; iscomment = false;
			continue;
		}
		else if ((*ptr == _T('#')) && (!quoted) && (firstchr)) { // beginning of a comment
			firstchr = false; iscomment = true; continue; // mark as a comment
		}
		else if (!iscomment) {
			if ((tmpstr.empty()) && (*ptr == _T('"')) && (!quoted)) { // beginning of a quoted text
				quoted = true; continue;
			}
			else if ((*ptr == _T(',')) && (!quoted)) { // delimiter
				record.push_back(tmpstr); tmpstr.clear();
			}
			else if ((*ptr == _T('"')) && (quoted)) {
				if (*(ptr + 1) == _T('"')) {
					tmpstr += _T("\""); ++ptr; continue; // double quotes as a letter
				}
				else if ((*(ptr + 1) == _T('\r')) || (*(ptr + 1) == _T('\n')) || (*(ptr + 1) == _T(',')) || (*(ptr + 1) == _T('\0'))) {
					quoted = false; continue; // end of quote
				}
				else continue; // invalid? just ignore it
			}
			else { // data
				tmpletter[0] = *ptr; tmpstr += tmpletter;
			}
		}
	}
	if (!tmpstr.empty()) {record.push_back(tmpstr); tmpstr.clear();}
	if (!record.empty()) {arr.push_back(record); record.clear();}
}
