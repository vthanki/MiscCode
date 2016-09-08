#include "Waveform.h"
#include <math.h>
#include <new>

int Waveform::device_config(void)
{
	HDWF dev_handle;
	double hzAcq;

	if(!FDwfDeviceOpen(-1, &dev_handle))
		return -1;

	std::cout << "Device configuration started... " << std::flush;
	// Configure Waveform Generator

	// enable first channel
	FDwfAnalogOutNodeEnableSet(dev_handle, 0, AnalogOutNodeCarrier, true);
	// set sine function
	FDwfAnalogOutNodeFunctionSet(dev_handle, 0, AnalogOutNodeCarrier, funcSine);
	// 1.41V amplitude (1Vrms), 2.82V pk2pk
	FDwfAnalogOutNodeAmplitudeSet(dev_handle, 0, AnalogOutNodeCarrier, 1.41);

	// Prepare Input

	FDwfAnalogInChannelEnableSet(dev_handle, 0, true);

	// Set voltage range to 5V
	FDwfAnalogInChannelRangeSet(dev_handle, 0, 5);

	// recording rate for more samples than the device buffer is
	// limited by device communication
	FDwfAnalogInAcquisitionModeSet(dev_handle, acqmodeRecord);

	// Set 1KHz frequency as default
	FDwfAnalogInFrequencySet(dev_handle, 1000);

	// make sure we calculate record length with the real frequency
	FDwfAnalogInFrequencyGet(dev_handle, &hzAcq);

	FDwfAnalogInRecordLengthSet(dev_handle, 1.0*(this->input_samples/hzAcq));

	// wait at least 2 seconds with Analog Discovery for the offset to
	// stabilize, before the first reading after device open or
	// offset/range change
	Wait(2);

	FDwfAnalogInConfigure(dev_handle, false, true);

	std::cout << "Done\n" << std::flush;

	return dev_handle;
}

void Waveform::device_close(int dev_handle)
{
	FDwfDeviceClose(dev_handle);
}

double Waveform::measure_rms(int dev_handle, double frequency)
{
	STS sts;
	double hzAcq;
	double* rgdSamples;
	int cSamples = 0;
	int cAvailable, cLost, cCorrupted;
	bool fLost = false, fCorrupted = false;
	double rms = 0.0;

	// Allocate 512 extra samples
	// FIXME: Handle allocation failure
	rgdSamples = new double[this->input_samples + 512];

	FDwfAnalogInFrequencySet(dev_handle, frequency * 32);

	// make sure we calculate record length with the real frequency
	FDwfAnalogInFrequencyGet(dev_handle, &hzAcq);
	FDwfAnalogInRecordLengthSet(dev_handle, 1.0*this->input_samples/hzAcq);

	FDwfAnalogInConfigure(dev_handle, false, true);

	FDwfAnalogOutNodeFrequencySet(dev_handle, 0, AnalogOutNodeCarrier, frequency);
	FDwfAnalogOutConfigure(dev_handle, 0, true);

	while(cSamples < this->input_samples){
		if(!FDwfAnalogInStatus(dev_handle, true, &sts)) {
			printf("error");
			return -1;
		}
		if(cSamples == 0 && (sts == stsCfg || sts == stsPrefill || sts == stsArm)){
			// Acquisition not yet started.
			continue;
		}

		FDwfAnalogInStatusRecord(dev_handle, &cAvailable, &cLost, &cCorrupted);

		cSamples += cLost;

		if(cLost) fLost = true;
		if(cCorrupted) fCorrupted = false;

		if(!cAvailable) continue;

		FDwfAnalogInStatusData(dev_handle, 0, &rgdSamples[cSamples], cAvailable);
		cSamples += cAvailable;
	}


	if (fLost) {
		printf("Samples were lost! Reduce frequency");
	} else if(cCorrupted) {
		printf("Samples could be corrupted! Reduce frequency");
	}

	for (int i = 0; i < cSamples; i++)
		rms += (rgdSamples[i]*rgdSamples[i]);

	rms = sqrt(rms/this->input_samples);

	delete[] rgdSamples;

	return rms;
}

Waveform::Waveform(void)
{
	this->num_samples = 0;
}

Waveform::Waveform(double start_freq, double end_freq, int num_samples)
{
		double startl, endl, interval, delta;
		int i;

		// All the frequencies will be sampled at hardcoded
		// 1600 times, which can be kept configurable in future.
		this->input_samples = 1600;

		this->num_samples = num_samples;

		int dev_handle = device_config();

		// TODO: Currently only logarithmic scale supported,
		// probably need to add support for linear scale as well.
		// Hint: Add the scale type enum as an argument to constructor
		startl = log10(start_freq);
		endl= log10(end_freq);

		delta = (endl - startl) / (num_samples - 1);
		for (i = 0; i < (int) num_samples; i++) {
			this->frequencies.push_back(pow(10, startl));
			this->volt_rms.push_back(measure_rms(dev_handle, this->frequencies[i]));
			startl += delta;
		}

		device_close(dev_handle);
}

Waveform::Waveform(const char *waveform_data_file)
{
	std::string line;
	ifstream file(waveform_data_file);

	double frequency, volt_rms;
	std::string comma;

	this->num_samples = 0;
	while (file >> frequency >> comma >> volt_rms) {
		this->num_samples++;
		this->frequencies.push_back(frequency);
		this->volt_rms.push_back(volt_rms);
	}

	file.close();
}

Waveform::~Waveform(void)
{
	/* Place holder */
}

void Waveform::set_ref_volt(double volt)
{
	this->volt_ref = volt;
}

int Waveform::get_num_samples(void)
{
	return this->num_samples;
}

int Waveform::write_to_file(const char *filename)
{
	ofstream target_file;

	target_file.open(filename);
	if (!target_file.is_open())
		return -1;

	std::cout << "Writing " << this->num_samples << " sample information to file... " << std::flush;
	for (int i = 0; i < this->num_samples; i++)
		target_file << this->frequencies[i] << ", " << this->volt_rms[i] << std::endl;

	target_file.close();

	std::cout << "Done\n" << std::flush;
	return 0;
}

void Waveform::dump(void)
{
	std::cout << "Dumping " << this->num_samples << " samples information\n" << std::flush;
	for (int i = 0; i < this->num_samples; i++)
		std::cout << "Freq: " << this->frequencies[i]
			  << "Hz\tV(rms): " << this->volt_rms[i]
			  << std::endl << std::flush;

}

