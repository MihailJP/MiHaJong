#!/usr/bin/env python

# This script gives the second chance for the systems without 'ruby-lzma' gem.
# Ruby gem 'ruby-lzma' does not seem to be installable under Windows, except Cygwin.

# If you are using Python 3.x, there is the 'lzma' library came with the Python distribution.
# If you are using Python 2.x, you need 'pylzma' library installed.

# First, determine which library for LZMA manipulation.
import sys

def isVer2():
  try:
    return (sys.version_info.major == 2)
  except AttributeError:
    return (sys.version_info[0] == 2)

def initdll():
  try:
    import lzma; return "lzma"
  except ImportError:
    try:
      import pylzma; return "pylzma"
    except ImportError:
      sys.stderr.write("Error: neither 'lzma' nor 'pylzma' was found.\n")
      if isVer2():
        sys.stderr.write("Please install 'pylzma' using:\n\teasy_install pylzma\n")
      else:
        sys.stderr.write("Please check your Python installation.\n")
      sys.exit(2)

# Next, check the commandline arguments.
def checkargs():
  if len(sys.argv) != 2:
    sys.stderr.write("Usage: python " + sys.argv[0] + " source-file\n")
    sys.exit(1)
  else:
    return sys.argv[1]

# Process
def compress(module, filename):
  file = open(filename, "rb")
  target = open(filename + ".lzma", "wb")
  uncompressed_dat = file.read()
  if module == "lzma":
    import lzma, struct
    compressed_dat = lzma.compress(uncompressed_dat, format=lzma.FORMAT_ALONE)
    header = compressed_dat[0:5]
    header += struct.pack('<Q', len(uncompressed_dat))
    target.write(header + compressed_dat[13:])
  elif module == "pylzma":
    import pylzma, struct
    raw_lzma = pylzma.compressfile(uncompressed_dat, eos=0)
    header = raw_lzma.read(5)
    header += struct.pack('<Q', len(uncompressed_dat))
    target.write(header + raw_lzma.read())
  else:
    file.close(); target.close()
    raise ValueError("Invalid module name: " + module)
  file.close(); target.close()

# Run
lzmalib = initdll()
sourcefile = checkargs()
compress(lzmalib, sourcefile)
