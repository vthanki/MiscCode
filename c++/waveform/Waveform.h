#include "sample.h"
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
		int input_samples;

		int device_config(void);
		double measure_rms(int dev_handle, double frequency);
		void device_close(int dev_handle);
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
