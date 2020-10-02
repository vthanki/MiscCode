#include "Device.h"
#include <iostream>
#include <math.h>
#include <unistd.h>


using namespace std;

Device::Device(void)
{
	HDWF dev_handle;

	this->open = FDwfDeviceOpen(-1, &dev_handle) != 0;

	this->dev_handle = dev_handle;
}

Device::~Device(void)
{
	if (is_open())
		FDwfDeviceClose(this->dev_handle);
}

bool Device::is_open(void)
{
	return this->open;
}

// TODO: Hard coded channel id to 0, which should be configurable, or may
// be taken as input argument to this function.
void Device::enable_input(double volt_range)
{
	FDwfAnalogInChannelEnableSet(this->dev_handle, 0, true);

	FDwfAnalogInChannelRangeSet(this->dev_handle, 0, volt_range);

	FDwfAnalogInAcquisitionModeSet(this->dev_handle, acqmodeRecord);

	Wait(2);

	FDwfAnalogInConfigure(this->dev_handle, false, true);
}

// TODO: Hard coded channel id to 0, which should be configurable, or may
// be taken as input argument to this function.
void Device::enable_output(double amplitude)
{
	FDwfAnalogOutNodeEnableSet(this->dev_handle, 0, AnalogOutNodeCarrier, true);

	FDwfAnalogOutNodeFunctionSet(this->dev_handle, 0, AnalogOutNodeCarrier, funcSine);

	FDwfAnalogOutNodeAmplitudeSet(this->dev_handle, 0, AnalogOutNodeCarrier, amplitude);
}

void Device::set_input_config(double sampling_freq, int num_samples)
{
	double acquired_freq;
	FDwfAnalogInFrequencySet(this->dev_handle, sampling_freq);

	// make sure we calculate record length with the real frequency
	FDwfAnalogInFrequencyGet(this->dev_handle, &acquired_freq);

	FDwfAnalogInRecordLengthSet(this->dev_handle, 1.0 * num_samples / acquired_freq);

	FDwfAnalogInConfigure(this->dev_handle, false, true);
}

void Device::set_output_config(double signal_freq)
{
	FDwfAnalogOutNodeFrequencySet(this->dev_handle, 0, AnalogOutNodeCarrier, signal_freq);

	FDwfAnalogOutConfigure(this->dev_handle, 0, true);
}

bool Device::read_rms(double frequency, int num_samples, double& rms_out)
{
	if (!is_open()) {
		std::cout << "Analog Discovery device not accessible" << std::endl << std::flush;
		return false;
	}

	// Configure input to take num_samples samples at 64 time higher than signal frequency
	set_input_config(frequency * 32, num_samples);
	set_output_config(frequency);

	// Wait 1ms to settle output freq
	// This is just kept for the safety purpose, and can be removed while
	// doing actual measurements.
	usleep(1000);

	STS status;
	int captured_samples = 0;
	int available_samples, lost_samples, corrupted_samples;
	bool lost, corrupted;
	double* samples_data;
	int cs_sum = 0;

	// Allocate 512 more samples to accommodate last read
	samples_data = new double[num_samples + 512];
	if (samples_data == nullptr) {
		std::cout << "Memory allocation failed for freq: " << frequency << std::endl << std::flush;
		return false;
	}

	while (captured_samples < num_samples) {

		if(!FDwfAnalogInStatus(this->dev_handle, true, &status)) {
			return false;
		}

		if(captured_samples == 0 && (status == stsCfg || status == stsPrefill || status == stsArm)){
			// Acquisition not yet started.
			continue;
		}

		FDwfAnalogInStatusRecord(this->dev_handle, &available_samples,	&lost_samples, &corrupted_samples);

		available_samples += lost_samples;

		if(lost_samples != 0) lost = true;
		if(corrupted_samples != 0) {
			corrupted = true;
			cs_sum += corrupted_samples;
		}

		if(!available_samples) continue;

		// by the time last call to read status is made, the
		// captured_samples can go beyond num_samples if
		// available_samples are added to it.
		FDwfAnalogInStatusData(this->dev_handle, 0, &samples_data[captured_samples], available_samples);
		captured_samples += available_samples;

	}

	rms_out = 0;
	for (int i = 0; i < captured_samples; i++)
		rms_out += pow(samples_data[i], 2);

	rms_out = sqrt(rms_out / captured_samples);
	if (cs_sum) {
		std::cout << "Corrupted Samples: " << cs_sum << std::endl;
		std::cout << "Freq: " << frequency << "\tRMS: " << rms_out << std::endl << std::flush;
	}

	delete[] samples_data;

	return true;
}
