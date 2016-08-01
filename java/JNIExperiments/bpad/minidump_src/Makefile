CXXFLAGS=-fPIC -g -O2 -std=c++11 -I. -I./src/ -Werror=missing-braces -Werror=non-virtual-dtor -Werror=overloaded-virtual -Werror=reorder -Werror=sign-compare -Werror=unused-variable -Werror=vla -fPIC -g -O2 -std=c++11 -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux
CC=g++
DEPS=./src/common/rfStackTrace.h
OBJS= basic_code_modules.o \
      basic_source_line_resolver.o \
      call_stack.o \
      cfi_frame_info.o \
      disassembler_x86.o \
      dump_context.o \
      dump_object.o \
      exploitability.o \
      exploitability_linux.o \
      logging.o \
      minidump.o \
      minidump_processor.o \
      org_echocat_breakpad4j_stacktrace_Main.o \
      pathname_stripper.o \
      process_state.o \
      proc_maps_linux.o \
      rfStackTrace.o \
      simple_symbol_supplier.o \
      source_line_resolver_base.o \
      stack_frame_cpu.o \
      stack_frame_symbolizer.o \
      stackwalk_common.o \
      stackwalker_address_list.o \
      stackwalker_amd64.o \
      stackwalker_arm64.o \
      stackwalker_arm.o \
      stackwalker.o \
      stackwalker_x86.o \
      tokenize.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CXXFLAGS)

libstacktrace.so: $(OBJS)
	$(CC) -o $@ $^ $(CXXFLAGS) -shared
clean:
	rm -f *.o *.so
