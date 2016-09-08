#include "sample.h"

class Device {
	private:
		HDWF dev_handle;
		bool open;
	public:
		Device(void);
		~Device(void);
		bool is_open(void);
		void enable_input(double volt_range);
		void enable_output(double amplitude); // in volts
		void set_input_config(double sampling_freq, int num_samples);
		void set_output_config(double signal_freq);
		bool read_rms(double frequency, int num_samples, double& rms_out);
};
