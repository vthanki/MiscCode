#include <malloc.h>
#include <unistd.h>
#include <pthread.h>
#include "client/linux/handler/exception_handler.h"
#include "breakpad.h"
#ifdef __cplusplus
extern "C" {
#endif
void crash(void);
#ifdef __cplusplus
}
#endif
#if 1
static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor,
		void* context, bool succeeded) {
	printf("# Dump path: %s\n", descriptor.path());
	return succeeded;
}
#endif
int main(int argc, char* argv[]) {
	Breakpad bp;
	crash();
	return 0;
}

