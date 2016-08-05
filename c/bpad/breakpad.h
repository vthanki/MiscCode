#pragma once

#include <client/linux/handler/exception_handler.h>

class Breakpad {

public:
    Breakpad();

private:
    static bool dumpCallback(const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded);
    google_breakpad::MinidumpDescriptor *m_descriptor;
    google_breakpad::ExceptionHandler *m_exceptionHandler;

};

