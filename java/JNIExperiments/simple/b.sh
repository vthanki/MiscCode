#!/bin/sh
gcc -fPIC -shared -o libhello.so -I$JAVA_HOME/include -I$JAVA_HOME/include/linux com_vishal_learning_Main.c
cp libhello.so com/vishal/learning/.
sync

