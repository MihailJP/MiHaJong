#!/usr/bin/env ruby
# -*- coding: shift_jis -*-

def gametype (type)
	typecode = type.to_i
	if typecode & 0x0f == 0x0f then return "" end
	tmparr = []
	if typecode & 0x01 != 0 then tmparr += ["�l��"] end
	if typecode & 0x02 != 0 then tmparr += ["�O��"] end
	if typecode & 0x04 != 0 then tmparr += ["�l�l�O��"] end
	if typecode & 0x08 != 0 then tmparr += ["���v�O��"] end
	return "�q" + tmparr.join(", ") + "�r"
end

dir = Dir::pwd # ���X�̍�ƃf�B���N�g����ޔ�
Dir::chdir(File.expand_path(File.dirname(__FILE__))) # ���̃X�N���v�g�̂���f�B���N�g���Ɉړ�

require "csv"
csvdat = CSV.read("../mjcore/data/confitem.csv", encoding: "SJIS") # �ݒ���L�q����CSV���J��
target = open("../mihajong/rulesyms.txt", mode_enc = "wb") # �o�͐�(���s�R�[�h���Œ肷�邽�ߊ����ăo�C�i�����[�h�Ƃ���)

for row in csvdat # �e���ڂ��Ƃɏo�͂���
	target.print "�y", row[8], "�z", row[9], gametype(row[1]), "\r\n"
	target.print "\t", row[10], "\r\n"
	target.print "\t�ݒ�l: ", row[11..-1].keep_if{|s| s}.delete_if{|s| s == ">>>"}.join(", "), "\r\n\r\n"
end

Dir::chdir(dir) # ��ƃf�B���N�g�������ɖ߂�
