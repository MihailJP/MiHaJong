#include "ini2map.h"

void INIParser::parseini(IniMapMap& inimap, const char* ini) {
	/*
	 *  CSV���p�[�X���Ĕz��̔z��I�u�W�F�N�g�ɑ������
	 *
	 *  ��������
	 *  �ECR�𖳎����܂�
	 *  �E�ŏ��̃Z�N�V�����錾�܂ł͖�������܂�
	 */
	inimap.clear(); // �܂����Z�b�g����
	std::string tmpstr; // ��Ɨp������
	std::string currentsection; // ���ݗL���ȃZ�N�V������
	std::string recordname; // ���ݗL���ȃ��R�[�h��
	RECORD section;
	char tmpletter[2] = {0, 0};
	bool started = false; bool firstchr = true; bool iscomment = false;
	bool issectionname = false; bool isrecordentity = false;
	for (const char* ptr = ini; *ptr != '\0'; ptr++) { // �|�C���^��for�Ƃ��g�������Ƃ���ւ�c�c
		if (*ptr == '\r') continue; // just ignore a CR
		else if (*ptr == '\n') { // new line
			if (firstchr) continue; // null line
			else if (issectionname) { // End of the section declaration
				if (!section.empty()) {
					inimap.insert(IniMapMap::value_type(currentsection, section)); section.clear();
				}
				currentsection = tmpstr; tmpstr.clear();
			}
			else if ((!iscomment) && (isrecordentity)) { // End of the record
				section.insert(RECORD::value_type(recordname, tmpstr)); tmpstr.clear(); recordname.clear();
			}
			else if (!iscomment) { // Invalid: just ignore it
				tmpstr.clear(); recordname.clear();
			}
			firstchr = true; iscomment = issectionname = isrecordentity = false;
			continue;
		}
		else if ((*ptr == ';') && (firstchr)) { // beginning of a comment
			firstchr = false; iscomment = true; continue; // mark as a comment
		}
		else if ((*ptr == '[') && (firstchr)) { // beginning of a section
			firstchr = false; started = true; issectionname = true; continue;
		}
		else if ((*ptr == ']') && (issectionname)) { // end of a section name
			firstchr = false; iscomment = true; continue; // ignore until newline encountered
		}
		else if ((!iscomment) && (started)) {
			if ((*ptr == '=') && (!issectionname) && (!isrecordentity)) { // delimiter
				isrecordentity = true; recordname = tmpstr; tmpstr.clear(); firstchr = false;
			}
			else { // data
				tmpletter[0] = *ptr; tmpstr += tmpletter; firstchr = false;
			}
		}
	}
	if ((!firstchr) && (!iscomment) && (isrecordentity)) { // End of the record, without an NL
		section.insert(RECORD::value_type(recordname, tmpstr)); tmpstr.clear(); recordname.clear();
	}
	if (!section.empty()) {
		inimap.insert(IniMapMap::value_type(currentsection, section)); section.clear();
	}
}
