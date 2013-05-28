#!/usr/bin/python

import os
import binascii
import sys

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
	for r,d,f in os.walk("./"):
		for files in f:
			if files.endswith(".yuv"):
				a = getFileCRC(files)
				print "crc32(%s)=0x%08x" % (os.path.join(r, files), a)