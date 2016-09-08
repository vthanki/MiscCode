#include "Waveform.h"
#include "Device.h"
#include <math.h>
#include <new>

Waveform::Waveform(void)
{
	this->num_samples = 0;
}


// Generate data by sweeping from start frequency to end frequency for X number
// of samples
Waveform::Waveform(double start_freq, double end_freq, int num_samples)
{
	double startl, endl, interval, delta;
	int i;
	Device dev;

	dev.enable_input(5);
	dev.enable_output(1.41);

	this->num_samples = num_samples;

	// TODO: Currently only logarithmic scale supported,
	// probably need to add support for linear scale as well.
	// Hint: Add the scale type enum as an argument to constructor
	startl = log10(start_freq);
	endl= log10(end_freq);

	delta = (endl - startl) / (num_samples - 1);
	for (i = 0; i < (int) num_samples; i++) {
		double freq = pow(10, startl);
		double volt_rms = 0xFFFF; // Invalid

		// Take 32 samples of each period
		if (!dev.read_rms(freq, (int)freq * 32, volt_rms)) {
			std::cout << "Error reading RMS value for freq: " << freq << std::endl << std::flush;
			break;
		}

		this->frequencies.push_back(freq);
		this->volt_rms.push_back(volt_rms);

		startl += delta;
	}
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

