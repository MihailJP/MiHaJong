#!/usr/bin/env python

from pygments.formatters import HtmlFormatter

formatter = HtmlFormatter()
print(formatter.get_style_defs(['.codehilite pre']))
