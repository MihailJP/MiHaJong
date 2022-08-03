TARGETS=addyaku.html history.html license.html playing.html pygments.css readme.html rulesyms.md rulesyms.html script.html

.PHONY: all clean

all: $(TARGETS)

.SUFFIXES: .md .html
.md.html:
	cat $^ | $(PYTHON) ../auxtools/md2html.py > $@

rulesyms.md: ../mjcore/data/confitem.csv
	ruby ../auxtools/rulesyms.rb

pygments.css:
	$(PYTHON) ../auxtools/makecss.py > $@

clean:
	-rm -rf $(TARGETS)
