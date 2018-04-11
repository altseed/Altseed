import aceutils

aceutils.cdToScript()
aceutils.mkdir('../Doxygen')
aceutils.cd(r'../Doxygen/')
aceutils.call(r'doxygen ../Script/Doxyfile_core_XML')