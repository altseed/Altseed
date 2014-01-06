import sys
import os
import os.path

argvs = sys.argv
argc = len(argvs)

version = sys.version_info

if argc >= 2:
	if version[0] == 2:
		import urllib
		urllib.urlretrieve(argvs[1], os.path.basename(argvs[1]))
	elif version[0] == 3:
		import urllib.request
		urllib.request.urlretrieve(argvs[1], os.path.basename(argvs[1]))

