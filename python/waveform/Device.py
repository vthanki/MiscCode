#!/usr/bin/python

from ctypes import *
from dwfconstants import *
import time
import sys
import math

class Device:
    dev_handle = c_int()
    channel = c_int(0)

    def __init__(self):
        self.dwf = cdll.LoadLibrary("libdwf.so")
        self.dwf.FDwfDeviceOpen(c_int(-1), byref(dev_handle))

    def __del__(self):
        self.dwf.FDwfDeviceCloseAll()

    def isOpen(self):
        return dev_handle.value == hdwfNone.value

    def enableInput(self, voltRange):
        dwf.FDwfAnalogInChannelEnableSet(dev_handle, c_int(0), c_bool(True))
        dwf.FDwfAnalogInChannelOffsetSet(dev_handle, c_int(0), c_double(0))
        dwf.FDwfAnalogInChannelRangeSet(dev_handle, c_int(0), c_double(voltRange))
        # FIXME: Do we need to set the acquisition mode?
        # dwf.FDwfAnalogInAcquisitionModeSet(dev_handle, acqmodeRecord)
        time.sleep(2)

    def enableOutput(self, amplitude):
        dwf.FDwfAnalogOutNodeEnableSet(dev_handle, channel, AnalogOutNodeCarrier, c_bool(True))
        dwf.FDwfAnalogOutNodeFunctionSet(dev_handle, channel, AnalogOutNodeCarrier, funcSine)
        dwf.FDwfAnalogOutNodeAmplitudeSet(dev_handle, channel, AnalogOutNodeCarrier, c_double(amplitude))

    def setInputConfig(self, samplingFreq, numSamples):
        dwf.FDwfAnalogInBufferSizeSet(dev_handle, c_int(numSamples))
        dwf.FDwfAnalogInFrequencySet(dev_handle, c_double(samplingFreq))
        dwf.FDwfAnalogInConfigure(dev_handle, c_bool(False), c_bool(True))

    def setOutputConfig(self, signalFreq):
        dwf.FDwfAnalogOutNodeFrequencySet(dev_handle, channel, AnalogOutNodeCarrier, c_double(signalFreq))
        dwf.FDwfAnalogOutConfigure(dev_handle, channel, c_bool(True))

    def readRMS(self, freq, numSamples):
        if self.isOpen != True:
            print "Device not accessible, cannot do measurements."
            return double('NaN')
        else:
            self.setInputConfig(freq * 64, numSamples)
            self.setOutputConfig(freq)

            # Wait for 1ms to settle output frequency, can be removed if
            # measurements are okay without this additional wait.
            time.sleep(0.001)

            # Wait until acquisition is going on
            while True:
                dwf.FDwfAnalogInStatus(dev_handle, c_int(1), byref(sts))
                if sts.value == DwfStateDone.value :
                    break
                time.sleep(0.1)

            capturedSampleData = (c_double*numSamples)()
            dwf.FDwfAnalogInStatusData(dev_handle, 0, capturedSampleData, numSamples)
            return math.sqrt(sum(i ** 2 for i in capturedSampleData)/numSamples)

