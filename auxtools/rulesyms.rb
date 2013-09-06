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
target = open("../doc/rulesyms.md", mode_enc = "wt") # 出力先

target.print <<FINIS
MiHaJong ルール設定取得用シンボルリスト
======================================

これは、AIスクリプトから現在のルール設定を取得するときに使う文字列シンボルのリストです。


FINIS

for row in csvdat # 各項目ごとに出力する
	target.print "## 【\`", row[8], "\`】 ", row[9], gametype(row[1]), " ##\n"
	target.print "", row[10], "\n\n"
	target.print "* 設定値: \`", row[11..-1].keep_if{|s| s}.delete_if{|s| s == ">>>"}.join("\`, \`"), "`\n\n\n"
end

target.print <<FINIS
----------------------------------------------------------------------------

_Finis_
FINIS

Dir::chdir(dir) # 作業ディレクトリを元に戻す
