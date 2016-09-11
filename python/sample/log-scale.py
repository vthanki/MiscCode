#!/usr/bin/python

import math
import random
import sys

if len(sys.argv) != 5:
    print 'Invalid number of arguments'
    print 'usage: ' + sys.argv[0] + ' startfreq endfreq intervals filename'
    exit(-1)

startf = float(sys.argv[1])
endf = float(sys.argv[2])
intervals = int(sys.argv[3])
fname = sys.argv[4]

startl = math.log10(startf)
endl = math.log10(endf)

delta = (endl - startl) / (intervals - 1)

fHandle = open(fname, 'w+')

for i in range(0, intervals):
    freq = 10 ** startl
    fHandle.write(str(freq) + ', ' + str(random.randrange(0,1569842357,5) / 156984235789.0) + '\n')
    startl = startl + delta


fHandle.close()
