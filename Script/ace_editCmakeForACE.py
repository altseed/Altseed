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

# to glfw
if 'GLFW' in lines:
	lines = lines.replace('-Wall', '-Wall -fPIC')


lines = lines + "\n"
lines = lines + "# ForACE\n"
lines = lines + "if (MSVC)\n"
lines = lines + "else()\n"
lines = lines + "\tadd_definitions(-fPIC)\n"
lines = lines + "\tset(CMAKE_C_FLAGS \"${CMAKE_C_FLAGS} -fPIC\")\n"
lines = lines + "\tset(CMAKE_C_FLAGS_DEBUG \"${CMAKE_C_FLAGS_DEBUG} -fPIC\")\n"
lines = lines + "\tset(CMAKE_C_FLAGS_RELEASE \"${CMAKE_C_FLAGS_RELEASE} -fPIC\")\n"
lines = lines + "\tset(CMAKE_C_FLAGS_MINSIZEREL \"${CMAKE_C_FLAGS_MINSIZEREL} -fPIC\")\n"
lines = lines + "\tset(CMAKE_C_FLAGS_RELWITHDEBINFO \"${CMAKE_C_FLAGS_RELWITHDEBINFO} -fPIC\")\n"
lines = lines + "\tset(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -fPIC\")\n"
lines = lines + "endif()\n"

f = open(argvs[1], 'w')
f.write(lines)
f.close()
