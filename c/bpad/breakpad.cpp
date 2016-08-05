#include "breakpad.h"

bool Breakpad::dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded) {

	printf("Dump path: %s\n", descriptor.path());
	return succeeded;
}

Breakpad::Breakpad()
{
	this->m_descriptor = new google_breakpad::MinidumpDescriptor("/tmp");
//	this->m_descriptor->set_size_limit(200*1024);
	this->m_exceptionHandler = new google_breakpad::ExceptionHandler(*(this->m_descriptor), NULL, Breakpad::dumpCallback, NULL, true, -1);

	printf ("Registered the crash  handler\n");
}
