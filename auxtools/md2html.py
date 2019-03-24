#!/usr/bin/env python
# -*- coding: utf-8 -*-

import markdown
import codecs
import sys
import re

sys.stdin = codecs.getreader('utf-8')(sys.stdin)
sys.stdout = codecs.getwriter('utf-8')(sys.stdout)
mdText = sys.stdin.read()

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<link rel=\"stylesheet\" href=\"manual.css\" type=\"text/css\">")
print("<title>" + re.compile('\\r?\\n').split(mdText)[0] + "</title>")
print("</head>")
print("<body>")

print(markdown.markdown(mdText.replace('.md', '.html'),
                        extensions=['tables', 'fenced_code', 'codehilite'],
                        output_format='html5'))

print("</body>")
print("</html>")
