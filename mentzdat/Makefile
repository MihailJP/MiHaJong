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

mentz.dat: putchr mnzcalc
	./mnzcalc > $@

.PHONY: clean
clean:
	-rm *.hi *.o $(TARGETS)
