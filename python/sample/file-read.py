#!/usr/bin/python

# This script reads .csv file containing "freq, volt" format

import sys

if len(sys.argv) != 2:
    print 'Invalid arguments'
    print 'Usage: ' + sys.argv[0] + ' filename'
    exit(-1)

with open(sys.argv[1]) as f:
    for line in f:
        freq, vrms = [float(x) for x in line.split(',')]
        print freq, vrms


