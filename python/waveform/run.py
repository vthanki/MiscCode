#!/usr/bin/python

# Usage: ./run.py freq

from device import Device
from waveform import Waveform
import sys

w1 = Waveform()
w1.loadFromFile(sys.argv[1])

w2 = Waveform()
w2.loadFromFile(sys.argv[2])


w1.dumpData()
print w1.numSamples, w2.numSamples
w2.dumpData()

w3 = w1.getDeviation(w2)
print w1.numSamples, w2.numSamples, w3.numSamples
w3.dumpData()
