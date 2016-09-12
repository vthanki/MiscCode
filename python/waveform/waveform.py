#!/usr/bin/python

from device import Device
from ctypes import *
import time
import sys
import math

class Waveform:
    frequencies = list()
    voltRms = list()

    def __init__(self, startFreq, endFreq, numSamples):
        self.device = Device()
        self.device.enableInput(5)
        self.device.enableOutput(1.41)
        self.numSamples = numSamples

        startl = math.log10(startFreq)
        endl = math.log10(endFreq)

        delta = (endl - startl) / (numSamples - 1)

        for i in range(0, numSamples):
            freq = 10 ** startl
            rms = self.device.readRMS(freq, int(freq*32))
            if math.isnan(rms) == False:
                self.frequencies.append(freq)
                self.voltRms.append(rms)

            startl = startl + delta

    def writeToFile(self, fileName):
        fHandle = open(fileName, 'w+')
        for freq, vrms in zip(self.frequencies, self.voltRms):
            fHandle.write(str(freq) + ', ' + str(vrms) + '\n')
        fHandle.close()

    def loadFromFile(self, fileName):
        with open(fileName) as f:
            for line in f: # read rest of lines
                freq, vrms = [float(x) for x in line.split(',')]
                self.frequencies.append(freq)
                self.voltRms.append(vrms)

    def dumpData(self):
        for freq, vrms in zip(self.frequencies, self.voltRms):
            print 'Freq:', freq, 'Vrms:', vrms
