#!/usr/bin/python

import sys
import binascii

def getFileCRC(_path):
	try:
		blocksize = 1024 * 64
		f = open(_path,"rb")
		str = f.read(blocksize)
		crc = 0
		while(len(str) != 0):
			crc = binascii.crc32(str, crc)
			str = f.read(blocksize)
		f.close()
	except:
		print 'get file crc error!'
		return 0
	return crc

if __name__ == "__main__":
	print "crc32=0x%x" % getFileCRC(sys.argv[1])