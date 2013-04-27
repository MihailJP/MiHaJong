TARGETS = putchr mnzcalc
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

.PHONY: clean
clean:
	-rm *.hi *.o $(TARGETS)
