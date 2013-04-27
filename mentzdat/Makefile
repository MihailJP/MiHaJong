TARGETS = putchr mnzcalc mentz.dat
CC = gcc
CCPARAM = -O2
GHC = ghc
GHCPARAM = -O2 -threaded

.PHONY: all
all: $(TARGETS)

putchr: putchr.c
	$(CC) $(CCPARAM) -o $@ $^

mnzcalc: mnzcalc.hs
	$(GHC) $(GHCPARAM) -o $@ $^

mentz.dat.01: putchr mnzcalc
	./mnzcalc 1 > $@
mentz.dat.02: putchr mnzcalc
	./mnzcalc 2 > $@
mentz.dat.03: putchr mnzcalc
	./mnzcalc 3 > $@
mentz.dat.04: putchr mnzcalc
	./mnzcalc 4 > $@
mentz.dat.05: putchr mnzcalc
	./mnzcalc 5 > $@
mentz.dat.06: putchr mnzcalc
	./mnzcalc 6 > $@
mentz.dat.07: putchr mnzcalc
	./mnzcalc 7 > $@
mentz.dat.08: putchr mnzcalc
	./mnzcalc 8 > $@
mentz.dat.09: putchr mnzcalc
	./mnzcalc 9 > $@
mentz.dat.10: putchr mnzcalc
	./mnzcalc 10 > $@
mentz.dat.11: putchr mnzcalc
	./mnzcalc 11 > $@
mentz.dat.12: putchr mnzcalc
	./mnzcalc 12 > $@
mentz.dat.13: putchr mnzcalc
	./mnzcalc 13 > $@
mentz.dat.14: putchr mnzcalc
	./mnzcalc 14 > $@
mentz.dat.15: putchr mnzcalc
	./mnzcalc 15 > $@
mentz.dat.16: putchr mnzcalc
	./mnzcalc 16 > $@
mentz.dat.17: putchr mnzcalc
	./mnzcalc 17 > $@
mentz.dat.18: putchr mnzcalc
	./mnzcalc 18 > $@
mentz.dat.19: putchr mnzcalc
	./mnzcalc 19 > $@
mentz.dat.20: putchr mnzcalc
	./mnzcalc 20 > $@
mentz.dat.21: putchr mnzcalc
	./mnzcalc 21 > $@
mentz.dat.22: putchr mnzcalc
	./mnzcalc 22 > $@
mentz.dat.23: putchr mnzcalc
	./mnzcalc 23 > $@
mentz.dat.24: putchr mnzcalc
	./mnzcalc 24 > $@
mentz.dat.25: putchr mnzcalc
	./mnzcalc 25 > $@

mentz.dat: \
mentz.dat.01 mentz.dat.02 mentz.dat.03 mentz.dat.04 mentz.dat.05 \
mentz.dat.06 mentz.dat.07 mentz.dat.08 mentz.dat.09 mentz.dat.10 \
mentz.dat.11 mentz.dat.12 mentz.dat.13 mentz.dat.14 mentz.dat.15 \
mentz.dat.16 mentz.dat.17 mentz.dat.18 mentz.dat.19 mentz.dat.20 \
mentz.dat.21 mentz.dat.22 mentz.dat.23 mentz.dat.24 mentz.dat.25
	cat $^ > $@

.PHONY: clean
clean:
	-rm *.hi *.o $(TARGETS) mentz.dat.*
