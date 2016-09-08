#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Waveform {
	private:
		vector<double> frequencies;
		vector<double> volt_rms;
		double volt_ref;
		int num_samples;

		double measure_rms(int dev_handle, double frequency);
	public:
		Waveform(void);
		Waveform(double start_freq, double end_freq, int num_samples);
		Waveform(const char *filename);
		~Waveform(void);

		void set_ref_volt(double volt);
		int get_num_samples(void);
		int write_to_file(const char *filename);
		void dump();
};
