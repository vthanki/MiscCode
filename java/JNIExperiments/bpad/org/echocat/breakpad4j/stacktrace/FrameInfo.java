package org.echocat.breakpad4j.stacktrace;

/**
 * Created by vishal.thanki@teufel.local on 7/28/16.
 */
public class FrameInfo {
    String moduleName;
    String functionName;
    String sourceFileName;
    long sourceLine;
    long moduleOffset;

    public String getModuleName() {
	    return this.moduleName;
    }

    public String getFunctionName() {
	    return this.functionName;
    }

    public String getSourceFileName() {
	    return this.sourceFileName;
    }

    public long getSourceLine() {
	    return this.sourceLine;
    }

    public long getModuleOffset() {
	    return this.moduleOffset;
    }
}
