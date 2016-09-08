#include "Waveform.h"
#include "Device.h"

int main(int argc, char *argv[])
{

	// Sweep from 20hz to 20khz
	//	Waveform wave(20, 20000, 500);
	//	wave.write_to_file(argv[1]);

	// Read data from file
	//	Waveform wave(argv[1]);
	//	wave.dump();

	// Read the RMS value for a particular freq
	Device d;
	d.enable_input(5);
	d.enable_output(1.41);

	double freq = 15806;
	int samples = freq * 32;
	double rms_out;

	if (!d.read_rms(freq, samples, rms_out))
		std::cout << "Error reading RMS value for freq: " << freq << std::endl << std::flush;
	else
		std::cout << "Freq: " << freq << "\tRMS: " << rms_out << std::endl << std::flush;
	return 0;
}
