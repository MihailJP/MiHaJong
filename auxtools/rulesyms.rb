#!/usr/bin/env ruby
# -*- coding: shift_jis -*-

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

require "csv"
csvdat = CSV.read("../mjcore/data/confitem.csv", encoding: "SJIS")
target = open("../mihajong/rulesyms.txt", mode_enc = "wb")

for row in csvdat
	target.print "【", row[8], "】", row[9], gametype(row[1]), "\r\n"
	target.print "\t", row[10], "\r\n"
	target.print "\t設定値: ", row[11..-1].keep_if{|s| s}.delete_if{|s| s == ">>>"}.join(", "), "\r\n\r\n"
end
