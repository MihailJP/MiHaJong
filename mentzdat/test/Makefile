CXX = g++
CXXFLAGS = --std=c++11 --input-charset=cp932 -L../../mjcore/.libs -lpthread -lmihajong

.PHONY: all
all: unzip testprog

.PHONY: fetch
fetch: problems.zip

problems.zip:
	wget http://mahjong.ara3.net/etc/shanten/problems.zip

.PHONY: unzip
unzip: p_hon_10000.txt p_koku_10000.txt p_normal_10000.txt p_tin_10000.txt

p_hon_10000.txt: problems.zip
	unzip -n $^ $@
p_koku_10000.txt: problems.zip
	unzip -n $^ $@
p_normal_10000.txt: problems.zip
	unzip -n $^ $@
p_tin_10000.txt: problems.zip
	unzip -n $^ $@

testprog: testprog.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm testprog p_hon_10000.txt p_koku_10000.txt p_normal_10000.txt p_tin_10000.txt

.PHONY: distclean
distclean: clean
	-rm problems.zip
