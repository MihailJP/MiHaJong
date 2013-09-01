TARGETS=addyaku.html history.html license.html playing.html readme.html rulesyms.md rulesyms.html script.html

.PHONY: all clean

all: $(TARGETS)

.SUFFIXES: .md .html
.md.html:
	cat $^ | python ../auxtools/md2html.py > $@

rulesyms.md: ../mjcore/data/confitem.csv
	ruby ../auxtools/rulesyms.rb

clean:
	-rm -rf $(TARGETS)
