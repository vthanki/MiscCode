package org.echocat.breakpad4j.stacktrace;

import java.util.List;
import java.util.ArrayList;

public class ThreadInfo {
	long threadId;
	List<FrameInfo> frames = new ArrayList<FrameInfo>();

	public void setThreadId(int threadId) {
		this.threadId = threadId;
	}

	public long getThreadId() {
		return this.threadId;
	}
}
