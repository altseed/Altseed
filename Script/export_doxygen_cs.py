import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Doxygen')
aceutils.cd(r'../Doxygen/')
aceutils.call(r'doxygen ../Script/Doxyfile_cs')
aceutils.copytree(r'Doxygen_CS/html/', targetDir+r'/Reference_CS')