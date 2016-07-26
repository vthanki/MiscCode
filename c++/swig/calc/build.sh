#!/bin/sh

set -x
JAVA_HOME=/opt/jdk

mkdir -p java \
	&& swig -c++ -java -outdir java swig_example.i \
	&& g++ -fPIC -shared Calc.cc swig_example_wrap.cxx -I$JAVA_HOME/include -I$JAVA_HOME/include/linux -o libcpp_calc.so \
	&& cp libcpp_calc.so java/.

set +x
