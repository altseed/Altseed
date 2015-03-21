# coding: cp932

import os, os.path, sys

if len(sys.argv) != 4:
	exit() 

target = open(sys.argv[3],'w')
target.write('#pragma once\n')
target.write('\n')

def export( path, name ):
	file = open( path, 'rb' )
	data = file.read()
	file.close()

	target.write( 'const uint8_t ' + name +'[] = {\n' ) 

	count = 0
	for d in data:
		target.write( str(d) )
		target.write(',')
		
		count += 1
		if count % 100 == 0:
			target.write( '\n' )

	target.write('0,')
	target.write( '\n};\n\n' ) 

export( sys.argv[1], sys.argv[2] )

target.close()