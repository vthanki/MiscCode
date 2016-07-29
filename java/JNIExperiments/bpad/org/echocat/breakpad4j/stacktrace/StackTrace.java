package org.echocat.breakpad4j.stacktrace;

import java.util.List;
import java.util.ArrayList;

public class StackTrace {
	String crashCause;
	String os;
	String cpu;
	List<ThreadInfo> threads = new ArrayList<ThreadInfo>();
}
