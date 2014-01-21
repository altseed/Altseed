# This script edits a cmake file for ACE.
# The script appends fPIC to compile on 64bit *nix OS.

import sys
import os
import os.path

argvs = sys.argv
argc = len(argvs)

if argc < 2:
	sys.exit()

f = open(argvs[1])
lines = f.read()
f.close()

if '# ForACE' in lines:
	sys.exit()

lines = lines + "\n"
lines = lines + "# ForACE"
lines = lines + "if (MSVC)\n"
lines = lines + "else()\n"
lines = lines + "\tset(CMAKE_C_FLAGS \"${CMAKE_C_FLAGS} -fPIC\")"
lines = lines + "\tset(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -fPIC\")"
lines = lines + "endif()\n"

f = open(argvs[1], 'w')
f.write(lines)
f.close()