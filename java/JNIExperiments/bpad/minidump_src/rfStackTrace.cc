#include <vector>
#include <iostream>
#include <string>
#include "common/scoped_ptr.h"
#include "common/using_std_string.h"
#include "google_breakpad/processor/basic_source_line_resolver.h"
#include "google_breakpad/processor/minidump.h"
#include "google_breakpad/processor/minidump_processor.h"
#include "google_breakpad/processor/process_state.h"
#include "processor/logging.h"
#include "processor/simple_symbol_supplier.h"
#include "processor/stackwalk_common.h"
#include "common/rfStackTrace.h"


using namespace std;
using namespace google_breakpad;

using google_breakpad::BasicSourceLineResolver;
using google_breakpad::Minidump;
using google_breakpad::MinidumpProcessor;
using google_breakpad::ProcessState;
using google_breakpad::SimpleSymbolSupplier;
using google_breakpad::scoped_ptr;

const char* RegInfo::getRegName() { return regName.c_str(); }
const char* FrameInfo::getFuncName() { return m_funcName.c_str(); }
const char* FrameInfo::getSrcFileName() { return m_srcFileName.c_str(); }
const char* FrameInfo::getBinFileName() { return m_binFileName.c_str(); }
void FrameInfo::addRegInfo(RegInfo reg) { regs.push_back(reg); }
int FrameInfo::regCount() { return regs.size(); }
RegInfo FrameInfo::getRegInfoAt(int i) { return regs[i]; } //FIXME: handle invalid index
const char* ModuleInfo::getModuleName() { return name.c_str(); }
const char* ModuleInfo::getModuleVersion() { return version.c_str(); }
const char* ModuleInfo::getModuleSymIssues() { return symIssues.c_str(); }
void ThreadInfo::addFrame(FrameInfo frame) { frames.push_back(frame); }
int ThreadInfo::frameCount() { return frames.size(); }
unsigned long ThreadInfo::getThreadID() { return threadId; }
FrameInfo ThreadInfo::getFrameInfoAt(int i) { return frames[i];	} //FIXME: handle invalid index
void RfStackTrace::addThread(ThreadInfo thread) { threads.push_back(thread); }
void RfStackTrace::addModule(ModuleInfo module) { modules.push_back(module); }
const char* RfStackTrace::getCrashCause() { return crashCause.c_str(); }
const char* RfStackTrace::getOS() { return os.c_str(); }
const char* RfStackTrace::getCPU() { return cpu.c_str(); }
int RfStackTrace::threadCount() { return threads.size(); }
int RfStackTrace::moduleCount() { return modules.size(); }
ThreadInfo RfStackTrace::getThreadInfoAt(int i) { return threads[i]; } //FIXME: handle invalid index
ModuleInfo RfStackTrace::getModuleInfoAt(int i) { return modules[i]; } //FIXME: handle invalid index

//int RfMinidumpProcess(const char *dump_file, const char *sym_paths, bool is_dump_file, unsigned long len)
int RfMinidumpProcess(std::istream& dumpStream, const char *sym_paths, RfStackTrace *rfStackTrace)
{

	vector<string> symbol_paths;
//	string minidump_file(dump_file);

	symbol_paths.push_back(string(sym_paths));

	scoped_ptr<SimpleSymbolSupplier> symbol_supplier;
	symbol_supplier.reset(new SimpleSymbolSupplier(symbol_paths));

	BasicSourceLineResolver resolver;
	MinidumpProcessor minidump_processor(symbol_supplier.get(), &resolver);

	//Minidump dump(minidump_file);
	Minidump dump(dumpStream);
	if (!dump.Read()) {
		BPLOG(ERROR) << "Minidump " << dump.path() << " could not be read";
		return -1;
	}
	ProcessState process_state;
	if (minidump_processor.Process(&dump, &process_state) !=
			google_breakpad::PROCESS_OK) {
		BPLOG(ERROR) << "MinidumpProcessor::Process failed";
		return -1;
	}

	RfPrintProcessState(process_state, &resolver, rfStackTrace);

	return 0;
}
