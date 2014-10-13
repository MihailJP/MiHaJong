TARGETS = mentz.dat

.PHONY: all
all: $(TARGETS)

mentz.dat: mnzcalc.rb
	./mnzcalc.rb > $@

.PHONY: clean
clean:
	-rm $(TARGETS)
