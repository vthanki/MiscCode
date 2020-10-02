#!/usr/bin/python

from device import Device
from ctypes import *
import time
import sys
import math


class Waveform:

    def __init__(self, startFreq=0, endFreq=0, numSamples=0):
        if numSamples == 0:
            self.numSamples = numSamples
            self.frequencies = []
            self.voltRms = []
        else:
            self.frequencies = []
            self.voltRms = []
            self.device = Device()
            self.device.enableInput(5)
            self.device.enableOutput(1.41)
            self.numSamples = numSamples

            startl = math.log10(startFreq)
            endl = math.log10(endFreq)

            delta = (endl - startl) / (numSamples - 1)

            for i in range(0, numSamples):
                freq = 10 ** startl
                rms = self.device.readRMS(freq, int(freq * 32))
                if not math.isnan(rms):
                    self.frequencies.append(freq)
                    self.voltRms.append(rms)

                startl += delta

    def writeToFile(self, fileName):
        fHandle = open(fileName, 'w+')
        for freq, vrms in zip(self.frequencies, self.voltRms):
            fHandle.write(str(freq) + ', ' + str(vrms) + '\n')
        fHandle.close()

    def loadFromFile(self, fileName):
        with open(fileName) as f:
            for line in f:  # read rest of lines
                freq, vrms = [float(x) for x in line.split(',')]
                self.frequencies.append(freq)
                self.voltRms.append(vrms)
                self.numSamples += 1

    def dumpData(self):
        for freq, vrms in zip(self.frequencies, self.voltRms):
            print 'Freq:', freq, '\tVrms:', vrms

    def getDeviation(self, w):
        result = Waveform()
        for freq, vrms1, vrms2 in zip(self.frequencies, self.voltRms, w.voltRms):
            result.frequencies.append(freq)
            result.voltRms.append(vrms1-vrms2)
        return result
