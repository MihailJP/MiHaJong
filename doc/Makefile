TARGETS=addyaku.html history.html license.html playing.html readme.html rulesyms.md rulesyms.html script.html

.PHONY: all clean

all: $(TARGETS)

.SUFFIXES: .md .html
.md.html:
	(echo -e '<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>' && \
	markdown_py -o html5 -x tables -x fenced_code -x codehilite $^ && \
	echo -e '\n</body></html>') > $@

rulesyms.md: ../mjcore/data/confitem.csv
	ruby ../auxtools/rulesyms.rb

clean:
	-rm -rf $(TARGETS)
