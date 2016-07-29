#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c basic_code_modules.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c basic_source_line_resolver.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c call_stack.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c cfi_frame_info.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c disassembler_x86.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c dump_context.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c dump_object.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c exploitability.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c exploitability_linux.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c logging.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c minidump.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c minidump_processor.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c pathname_stripper.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c process_state.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c proc_maps_linux.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c simple_symbol_supplier.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c source_line_resolver_base.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stack_frame_cpu.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stack_frame_symbolizer.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalk_common.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker_address_list.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker_amd64.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker_arm.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker_arm64.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c stackwalker_x86.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c tokenize.cc 
#g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c rfStackTrace.cc
g++ -fPIC -g -O2 -std=c++11 -I. -I./src/ -c org_echocat_breakpad4j_stacktrace_Main.cc -I$JAVA_HOME/include -I$JAVA_HOME/include/linux
g++ -Werror=missing-braces -Werror=non-virtual-dtor -Werror=overloaded-virtual -Werror=reorder -Werror=sign-compare -Werror=unused-variable -Werror=vla -fPIC -g -O2 -std=c++11 -shared -o libstacktrace.so *.o src/third_party/libdisasm/libdisasm.a
cp libstacktrace.so ../org/echocat/breakpad4j/stacktrace/.
