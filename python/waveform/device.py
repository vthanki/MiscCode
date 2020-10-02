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
        self.dwf.FDwfDeviceOpen(c_int(-1), byref(self.dev_handle))

    def __del__(self):
        self.dwf.FDwfDeviceCloseAll()

    def isOpen(self):
        return self.dev_handle.value != hdwfNone.value

    def enableInput(self, voltRange):
        self.dwf.FDwfAnalogInChannelEnableSet(self.dev_handle, c_int(0), c_bool(True))
        self.dwf.FDwfAnalogInChannelOffsetSet(self.dev_handle, c_int(0), c_double(0))
        self.dwf.FDwfAnalogInChannelRangeSet(self.dev_handle, c_int(0), c_double(voltRange))
        # FIXME: Do we need to set the acquisition mode?
        self.dwf.FDwfAnalogInAcquisitionModeSet(self.dev_handle, acqmodeRecord)
        time.sleep(2)

    def enableOutput(self, amplitude):
        self.dwf.FDwfAnalogOutNodeEnableSet(self.dev_handle, self.channel, AnalogOutNodeCarrier, c_bool(True))
        self.dwf.FDwfAnalogOutNodeFunctionSet(self.dev_handle, self.channel, AnalogOutNodeCarrier, funcSine)
        self.dwf.FDwfAnalogOutNodeAmplitudeSet(self.dev_handle, self.channel, AnalogOutNodeCarrier, c_double(amplitude))

    def setInputConfig(self, samplingFreq, numSamples):
        acqFreq = c_double()
        self.dwf.FDwfAnalogInBufferSizeSet(self.dev_handle, c_int(numSamples))
        self.dwf.FDwfAnalogInFrequencySet(self.dev_handle, c_double(samplingFreq))

        self.dwf.FDwfAnalogInFrequencyGet(self.dev_handle, byref(acqFreq))
        self.dwf.FDwfAnalogInRecordLengthSet(self.dev_handle, c_double(1.0 * numSamples / acqFreq.value))
        self.dwf.FDwfAnalogInConfigure(self.dev_handle, c_bool(False), c_bool(True))

    def setOutputConfig(self, signalFreq):
        self.dwf.FDwfAnalogOutNodeFrequencySet(self.dev_handle, self.channel, AnalogOutNodeCarrier, c_double(signalFreq))
        self.dwf.FDwfAnalogOutConfigure(self.dev_handle, self.channel, c_bool(True))

    def readRMS(self, freq, numSamples):
        if self.isOpen() != True:
            print "Device not accessible, cannot do measurements."
            print self.dev_handle.value, hdwfNone.value
            return float('NaN')
        else:
            self.setInputConfig(freq*32, numSamples)
            self.setOutputConfig(freq)

            # Wait for 1ms to settle output frequency, can be removed if
            # measurements are okay without this additional wait.
            time.sleep(0.001)

            # Wait until acquisition is going on
            sts = c_byte()
            avaSamples = c_int(0)
            lostSamples = c_int(0)
            corrSamples = c_int(0)
            rms_sum = 0
            ls_sum = 0
            cs_sum = 0
            sum_availSamples = 0
            capturedSampleData = (c_double*numSamples)()
            while sum_availSamples < numSamples:
                self.dwf.FDwfAnalogInStatus(self.dev_handle, c_int(1), byref(sts))
                if sum_availSamples == 0 and (sts.value == DwfStateConfig or sts.value == DwfStatePrefill or sts.value == DwfStateArmed):
                    #time.sleep(0.1)
                    continue
                self.dwf.FDwfAnalogInStatusRecord(self.dev_handle, byref(avaSamples), byref(lostSamples), byref(corrSamples))
                if avaSamples.value == 0:
                    print 'No samples available for reading', numSamples, sum_availSamples
                    #time.sleep(0.1)
                    continue
                ls_sum = ls_sum + lostSamples.value
                cs_sum = cs_sum + corrSamples.value
                #print avaSamples.value, 'samples available for reading'
                self.dwf.FDwfAnalogInStatusData(self.dev_handle, c_int(0), capturedSampleData, avaSamples)
                rms_sum = rms_sum + sum(i ** 2 for i in capturedSampleData[:avaSamples.value])

                sum_availSamples  = sum_availSamples + avaSamples.value + lostSamples.value

#            print numSamples, sum_availSamples, ls_sum, cs_sum
            rms = math.sqrt(rms_sum/numSamples)
            print freq, rms
            return rms
