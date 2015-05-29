import aceutils

aceutils.cdToScript()
aceutils.cd(r'../')

aceutils.call(r'doxygen Script/acecsDoxyfile')
aceutils.copytree(r'Reference_CS/html/', targetDir+r'/Reference_CS')