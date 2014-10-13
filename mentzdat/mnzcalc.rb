#!/usr/bin/env ruby

# 向聴計算用 面子計算表構築プログラム

$result = [nil] * (5 ** 9 * 4)

class MeldData
	def valid?
		false
	end
	def inspect
		"#<Meld analysis>"
	end
	def +(other)
		raise TypeError.new("Type mismatch") unless other.kind_of? MeldData
		return other
	end
end
class InvalidMeld < MeldData
	def to_i # byteに直す
		255
	end
	def inspect
		"#<Meld analysis: invalid>"
	end
end
class NumsOfMeld < MeldData # 対子フラグ、面子数、面子候補数
	def valid?
		true
	end
	attr_reader :paired, :melds, :semimelds
	def check_paired(val)
		raise TypeError.new("Value is not boolean") unless (val.kind_of? TrueClass) || (val.kind_of? FalseClass)
		nil
	end
	def check_melds(val)
		raise TypeError.new("Value is not integer") unless (val.kind_of? Integer)
		raise RangeError.new("Value out of range") unless (val >= 0) && (val <= 4)
		nil
	end
	def check_semimelds(val)
		raise TypeError.new("Value is not integer") unless (val.kind_of? Integer)
		raise RangeError.new("Value out of range") unless (val >= 0) && (val <= 7)
		nil
	end
	def initialize(paired = false, melds = 0, semimelds = 0)
		check_paired(paired)
		check_melds(melds)
		check_semimelds(semimelds)
		@paired = paired
		@melds = melds
		@semimelds = semimelds
	end
	def paired=(val)
		check_paired(val)
		@paired = val
	end
	def melds=(val)
		check_melds(val)
		@melds = val
	end
	def semimelds=(val)
		check_semimelds(val)
		@semimelds = val
	end
	def to_a
		return [@paired, @melds, @semimelds]
	end
	private :check_paired, :check_melds, :check_semimelds
	def to_i # byteに直す
		return (@paired ? 128 : 0) + @melds * 16 + @semimelds
	end
	def inspect
		"#<Meld analysis: paired=#{@paired}, melds=#{@melds}, semimelds=#{@semimelds}>"
	end
	def +(other)
		raise TypeError.new("Type mismatch") unless other.kind_of? MeldData
		return self unless other.kind_of? NumsOfMeld
		return NumsOfMeld.new(@paired || other.paired, @melds + other.melds, @semimelds + other.semimelds)
	end
end
class NumsOfTile # 牌式にあたる型
	def check_index(i)
		raise TypeError.new("Index is not integer") unless i.kind_of? Integer
		raise IndexError.new("Index out of range") unless (i >= 1) && (i <= 9)
		nil
	end
	def check_val(v)
		raise TypeError.new("Value is not integer") unless v.kind_of? Integer
		raise RangeError.new("Value out of range") unless (v >= 0) && (v <= 4)
		nil
	end
	def initialize
		@vals = [0] * 9
	end
	def [](i) # NumsOfTileから値取り出し
		check_index(i)
		return @vals[i - 1]
	end
	def []=(i, x)
		check_index(i)
		check_val(x)
		@vals[i - 1] = x
	end
	def to_a # NumsOfTileを配列に変換
		return @vals.dup
	end
	private :check_index
	private :check_val
	def empty? # 牌がない場合true
		return (@vals.inject(0) {|acc, item| acc + item}) == 0
	end
	def tooMany? # 牌が多すぎる場合true
		return (@vals.inject(0) {|acc, item| acc + item}) > 14
	end
	def isolated?(i) # 浮き牌かどうか
		check_index(i)
		case i
		when 1 then
			return (@vals[0] == 1) && (@vals[1] == 0) && (@vals[2] == 0)
		when 9 then
			return (@vals[8] == 1) && (@vals[7] == 0) && (@vals[6] == 0)
		when 2 then
			return (@vals[1] == 1) && (@vals[0] == 0) && (@vals[2] == 0) && (@vals[3] == 0)
		when 8 then
			return (@vals[7] == 1) && (@vals[8] == 0) && (@vals[6] == 0) && (@vals[5] == 0)
		else
			return (@vals[i-1] == 1) && (@vals[i+0] == 0) && (@vals[i+1] == 0) && (@vals[i-2] == 0) && (@vals[i-3] == 0)
		end
	end
	def filter(&block)
		ans = NumsOfTile.new
		vals = self.dup.freeze
		for i in 1..9
			ans[i] = block.call(vals, i)
		end
		return ans
	end
	def filter!(&block)
		vals = self.dup.freeze
		for i in 1..9
			ans[i - 1] = block.call(vals, i)
		end
		return self
	end
	def dropIsolates # 浮き牌は除去しておく
		return self.filter {|nums, i| if nums.isolated?(i) then 0 else nums[i] end}
	end
	def dropIsolates!
		return self.filter! {|nums, i| if nums.isolated?(i) then 0 else nums[i] end}
	end
	def self.fromID(i) # 通し番号から変換
		raise TypeError.new("ID is not integer") unless i.kind_of? Integer
		raise IndexError.new("ID out of range") unless (i >= 0) && (i < (5 ** 9))
		vals = NumsOfTile.new
		for k in 0..8
			vals[k + 1] = (i % (5 ** (k + 1))) / (5 ** k)
		end
		return vals
	end
	def to_i # 通し番号に変換
		ans = 0
		for i in 0..8
			ans += (@vals[i] * 5 ** i)
		end
		return ans
	end
	def inspect
		"#<Number of tiles: #{@vals}>"
	end
	def ==(other)
		return false unless other.kind_of? NumsOfTile
		for i in 0..8
			return false unless @vals[i] == other[i + 1]
		end
		return true
	end
	def !=(other)
		return !(self == other)
	end
end

def takeTriplet(nums, mlds, i) # 刻子を取る
	pairFlag, melds, semimelds = mlds.to_a
	if nums[i] >= 3 then
		return [nums.filter {|p, q| if q == i then p[q] - 3 else p[q] end}, NumsOfMeld.new(pairFlag, melds + 1, semimelds)]
	else
		return [nums, mlds]
	end
end
def takeSequence(nums, mlds, i) # 順子を取る
	pairFlag, melds, semimelds = mlds.to_a
	if (nums[i] >= 1) && (nums[i + 1] >= 1) && (nums[i + 2] >= 1) then
		return [nums.filter {|p, q| if (q >= i) && (q <= (i + 2)) then p[q] - 1 else p[q] end}, NumsOfMeld.new(pairFlag, melds + 1, semimelds)]
	else
		return [nums, mlds]
	end
end
def takePair(nums, mlds, i) # 対子を取る
	pairFlag, melds, semimelds = mlds.to_a
	if nums[i] >= 2 then
		return [nums.filter {|p, q| if q == i then p[q] - 2 else p[q] end}, NumsOfMeld.new(true, melds, semimelds + 1)]
	else
		return [nums, mlds]
	end
end
def takeStep1(nums, mlds, i) # 両面と辺張を取る
	pairFlag, melds, semimelds = mlds.to_a
	if  (nums[i] >= 1) && (nums[i + 1] >= 1) then
		return [nums.filter {|p, q| if (q >= i) && (q <= (i + 1)) then p[q] - 1 else p[q] end}, NumsOfMeld.new(pairFlag, melds, semimelds + 1)]
	else
		return [nums, mlds]
	end
end
def takeStep2(nums, mlds, i) # 嵌張を取る
	pairFlag, melds, semimelds = mlds.to_a
	if  (nums[i] >= 1) && (nums[i + 2] >= 1) then
		return [nums.filter {|p, q| if (q == i) || (q == (i + 2)) then p[q] - 1 else p[q] end}, NumsOfMeld.new(pairFlag, melds, semimelds + 1)]
	else
		return [nums, mlds]
	end
end

def maxMeld(mode, caput, nummeld1, nummeld2) # 最大のものを計算する
	(nums1, mlds1) = nummeld1
	(nums2, mlds2) = nummeld2
	(pairFlag1, melds1, semimelds1) = mlds1.to_a
	(pairFlag2, melds2, semimelds2) = mlds2.to_a
	if caput then
		if (melds1 * mode + semimelds1) < (melds2 * mode + semimelds2) then
			return [nums2, mlds2]
		elsif ((melds1 * mode + semimelds1) == (melds2 * mode + semimelds2)) && (not pairFlag1) && pairFlag2 then
			return [nums2, mlds2]
		else
			return [nums1, mlds1]
		end
	else
		if (not pairFlag1) && pairFlag2 then
			return [nums2, mlds2]
		elsif ((melds1 * mode + semimelds1) < (melds2 * mode + semimelds2)) && ((!!pairFlag1) == (!!pairFlag2)) then
			return [nums2, mlds2]
		else
			return [nums1, mlds1]
		end
	end
end
def indexOffset(mode, caput)
	if mode == 2 then
		return caput ? 1 : 0
	elsif mode == 10 then
		return caput ? 3 : 2
	else
		throw ArgumentError.new("mode must be either 2 or 10")
	end
end
def calculate(mode, caput, nums, mlds) # 計算（再帰処理）
	index = nums.to_i * 4 + indexOffset(mode, caput)
	if !($result[index].nil?) then
		return [NumsOfTile.new, $result[index] + mlds] # メモ化したデータ
	elsif nums.dropIsolates.empty? then
		$result[index] = mlds
		return [NumsOfTile.new, mlds]
	elsif nums[1] == 0 then
		ans = calculate(mode, caput, NumsOfTile::fromID(nums.to_i / 5), mlds)
		$result[index] = ans[1]
		return ans
	else
		meldlist = []
		for i in 1..9
			p = takeTriplet(nums.dropIsolates, mlds, i)
			meldlist += (p[0] != nums) ? [calculate(mode, caput, p[0], p[1])] : [p]
		end
		for i in 1..7
			p = takeSequence(nums.dropIsolates, mlds, i)
			meldlist += (p[0] != nums) ? [calculate(mode, caput, p[0], p[1])] : [p]
		end
		for i in 1..9
			p = takePair(nums.dropIsolates, mlds, i)
			meldlist += (p[0] != nums) ? [calculate(mode, caput, p[0], p[1])] : [p]
		end
		for i in 1..8
			p = takeStep1(nums.dropIsolates, mlds, i)
			meldlist += (p[0] != nums) ? [calculate(mode, caput, p[0], p[1])] : [p]
		end
		for i in 1..7
			p = takeStep2(nums.dropIsolates, mlds, i)
			meldlist += (p[0] != nums) ? [calculate(mode, caput, p[0], p[1])] : [p]
		end
		ans = meldlist.inject([NumsOfTile.new, NumsOfMeld.new]) {|val1, val2| maxMeld(mode, caput, val1, val2)}
		$result[index] = ans[1]
		return ans
	end
end

def calcMentz(mode, caput, nums)
	index = nums.to_i * 4 + indexOffset(mode, caput)
	if nums.tooMany? then
		$result[index] = InvalidMeld.new
		$result[index]
	else
		calculate(mode, caput, nums, NumsOfMeld.new)[1]
	end
end

for i in 0...(5**9) # 計算処理
	calcMentz(2, false, NumsOfTile::fromID(i))
	calcMentz(2, true, NumsOfTile::fromID(i))
	calcMentz(10, false, NumsOfTile::fromID(i))
	calcMentz(10, true, NumsOfTile::fromID(i))
end

bytes = $result.map {|i| i.to_i}
STDOUT.binmode
STDOUT.write(bytes.pack("C*"))
