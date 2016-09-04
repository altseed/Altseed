# -*- coding: utf-8 -*-

import aceutils
import os.path

aceutils.cdToScript()

def getCppFiles(path,base):
	files = aceutils.get_files(path)
	files = [ base + p[len(path):] for p in files]
	files = [ p.replace('\\','/') for p in files]
	files = [ p for p in files if os.path.splitext(p)[1] in {'.c','.cpp','.cxx'}]
	return files

def getCMakeArray(files,name):
	s = 'set(' + name + '\r\n'
	for file in files:
		s += ('\t' + file + '\n')
	s += ')'
	return s

def isWin(file):
	return file.count('_Win.cpp') > 0

def isX11(file):
	return file.count('_X11.cpp') > 0

def isDX11(file):
	return file.count('_DX11.cpp') > 0 or file.count('DirectXToolKit') > 0

def isGL(file):
	return file.count('_GL.cpp') > 0

def isCommon(file):
	return not isWin(file) and not isX11(file) and not isDX11(file) and not isGL(file)


common_src = getCppFiles('../Dev/asd_cpp/common/','asd_cpp/common/')
core_src = getCppFiles('../Dev/asd_cpp/core/','asd_cpp/core/')
engine_src = getCppFiles('../Dev/asd_cpp/engine/','asd_cpp/engine/')
unittest_gtest_src = getCppFiles('../Dev/unitTest_cpp_gtest/','unitTest_cpp_gtest/')
unittest_engine_gtest_src = getCppFiles('../Dev/unitTest_Engine_cpp_gtest/','unitTest_Engine_cpp_gtest/')

fontGenerator_src = getCppFiles('../Dev/FontGenerator/FontGeneratorCore/','FontGenerator/FontGeneratorCore/')

core_win_src = [f for f in core_src if isWin(f)]
core_x11_src = [f for f in core_src if isX11(f)]
core_dx11_src = [f for f in core_src if isDX11(f)]
core_gl_src = [f for f in core_src if isGL(f)]
core_common_src = [f for f in core_src if isCommon(f)]

exported = ''

def newline():
	global exported
	exported += '\r\n'

exported += getCMakeArray(common_src,'common_to_engine_src')
newline()
newline()

exported += getCMakeArray(core_common_src,'core_src')
newline()
newline()

exported += getCMakeArray(core_dx11_src,'dx11_src')
newline()
newline()

exported += getCMakeArray(core_gl_src,'gl_src')
newline()
newline()

exported += getCMakeArray(core_win_src,'win_src')
newline()
newline()

exported += getCMakeArray(core_x11_src,'x11_src')
newline()
newline()

exported += getCMakeArray(engine_src,'engine_src')
newline()
newline()

exported += getCMakeArray(fontGenerator_src, 'fontGenerator_src')
newline()
newline()

f = open('../Dev/CMakeFiles.txt','w')

f.write(exported)

f.close()
