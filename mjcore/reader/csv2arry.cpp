#include "csv2arry.h"

void CSVReader::parsecsv (std::vector<std::vector<std::string> > & arr, const char* csv) {
	/*
	 *  CSVをパースして配列の配列オブジェクトに代入する
	 *
	 *  制限事項
	 *  ・CRを無視します
	 *  ・改行や引用符のクォートはできます
	 *  ・#で始まる行はコメントとして無視されます
	 */
	arr.clear(); // リセット
	std::string tmpstr; // 作業用文字列
	char tmpletter[2] = {0, 0};
	std::vector<std::string> record; // レコードデータ
	bool firstchr = true; bool quoted = false; bool iscomment = false; // フラグ
	for (const char* ptr = csv; *ptr != '\0'; ptr++) { // ポインタのforとか使ったことあれへん……
		if (*ptr == '\r') continue; // just ignore a CR
		else if ((*ptr == '\n') && (!quoted)) { // new line
			if (!iscomment) { // End of the record
				record.push_back(tmpstr); tmpstr.clear();
				arr.push_back(record); record.clear();
			}
			firstchr = true; iscomment = false;
			continue;
		}
		else if ((*ptr == '#') && (!quoted) && (!firstchr)) { // beginning of a comment
			firstchr = false; iscomment = true; continue; // mark as a comment
		}
		else if (!iscomment) {
			if ((tmpstr.empty()) && (*ptr == '"') && (!quoted)) { // beginning of a quoted text
				quoted = true; continue;
			}
			else if ((*ptr == ',') && (!quoted)) { // delimiter
				record.push_back(tmpstr); tmpstr.clear();
			}
			else if ((*ptr == '"') && (quoted)) {
				if ((*ptr + 1) == '"') {
					tmpstr += "\""; ++ptr; continue; // double quotes as a letter
				}
				else if (((*ptr + 1) == '\r') || ((*ptr + 1) == '\n') || ((*ptr + 1) == ',') || ((*ptr + 1) == '\0')) {
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
