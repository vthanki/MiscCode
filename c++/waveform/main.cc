#include "Waveform.h"

int main(int argc, char *argv[])
{
//	Waveform wave(20, 200, 10);
//	wave.write_to_file(argv[1]);
	Waveform wave(argv[1]);
	wave.dump();
	return 0;
}
