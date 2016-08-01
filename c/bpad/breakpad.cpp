#include "breakpad.h"

bool Breakpad::dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded) {

	printf("Dump path: %s\n", descriptor.path());
	return succeeded;
}

Breakpad::Breakpad() :
    m_descriptor("/tmp"),
    m_exceptionHandler(m_descriptor, NULL, Breakpad::dumpCallback, NULL, true, -1)

{
	printf ("Registered the crash  handler\n");
}
