import sys
import os
import os.path
import urllib

argvs = sys.argv
argc = len(argvs)

if argc >= 2:
	urllib.urlretrieve(argvs[1], os.path.basename(argvs[1]))

