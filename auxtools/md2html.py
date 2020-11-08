#!/usr/bin/env python
# -*- coding: utf-8 -*-

import markdown
import codecs
import sys
import re

def out(strDat):
    if sys.version_info.major < 3:
        print(strDat)
    else:
        sys.stdout.buffer.write(strDat.encode(encoding='utf-8') + b"\n")

mdText = "" # pre-declaration
if sys.version_info.major < 3:
    sys.stdin = codecs.getreader('utf-8')(sys.stdin)
    sys.stdout = codecs.getwriter('utf-8')(sys.stdout)
    mdText = sys.stdin.read()
else:
    mdText = sys.stdin.buffer.read().decode(encoding='utf-8')

titleText = re.compile('\\r?\\n').split(mdText)[0]

mdText = mdText.replace('.md', '.html')
mdText = markdown.markdown(mdText,
    extensions=['tables', 'fenced_code', 'codehilite'],
    extension_configs={
        'codehilite': {
            'guess_lang': False,
        },
    },
    output_format='html5')
mdText = mdText.replace("\ufdef ", "").replace("\ufdef", "")

out("<!DOCTYPE html>")
out("<html>")
out("<head>")
out("<link rel=\"stylesheet\" href=\"manual.css\" type=\"text/css\">")
out("<link rel=\"stylesheet\" href=\"pygments.css\" type=\"text/css\">")
out("<title>" + titleText + "</title>")
out("</head>")
out("<body>")
out(mdText)
out("</body>")
out("</html>")
