#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

def gametype (type)
	typecode = type.to_i
	if typecode & 0x0f == 0x0f then return "" end
	tmparr = []
	if typecode & 0x01 != 0 then tmparr += ["四麻"] end
	if typecode & 0x02 != 0 then tmparr += ["三麻"] end
	if typecode & 0x04 != 0 then tmparr += ["四人三麻"] end
	if typecode & 0x08 != 0 then tmparr += ["数牌三麻"] end
	return "〈" + tmparr.join(", ") + "〉"
end

dir = Dir::pwd # 元々の作業ディレクトリを退避
Dir::chdir(File.expand_path(File.dirname(__FILE__))) # このスクリプトのあるディレクトリに移動

require "csv"
csvdat = CSV.read("../mjcore/data/confitem.csv", encoding: "UTF-8") # 設定を記述したCSVを開く
target = open("../mihajong/rulesyms.txt", mode_enc = "wb") # 出力先(改行コードを固定するため敢えてバイナリモードとする)

for row in csvdat # 各項目ごとに出力する
	target.print "【", row[8], "】", row[9], gametype(row[1]), "\r\n"
	target.print "\t", row[10], "\r\n"
	target.print "\t設定値: ", row[11..-1].keep_if{|s| s}.delete_if{|s| s == ">>>"}.join(", "), "\r\n\r\n"
end

Dir::chdir(dir) # 作業ディレクトリを元に戻す
