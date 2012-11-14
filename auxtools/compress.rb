#!/usr/bin/env ruby
# -*- coding: UTF-8 -*-

require "lzma"; require "digest/sha2"

if ARGV.size < 1 then
	$stderr.print("Usage: " + $0 + " src-file\n")
	exit(1)
end

srcfile = ARGV[0]
targetfile = srcfile + ".lzma"

compressed = nil
File.open(srcfile, "rb") {|source|
	compressed = LZMA.compress(source.read())
}
File.open(targetfile, "wb") {|target|
	target.print(compressed)
}
