package org.echocat.breakpad4j.stacktrace;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.io.IOException;

public class Main {
	static {
		System.loadLibrary("stacktrace");
	}

	private static native StackTrace getStackTrace(byte[] dumpStream, String symbolsPath);

	public static void main(String[] args) {

		Path path = Paths.get(args[0]);
		byte[] data;
		try {
			data = Files.readAllBytes(path);
			StackTrace stackTrace = getStackTrace(data, args[1]);

			for (ThreadInfo threadInfo : stackTrace.threads) {
				System.out.println("Thread : " + threadInfo.getThreadId());
				for (FrameInfo frameInfo : threadInfo.frames) {
					System.out.print(frameInfo.getModuleName());
					if (!frameInfo.getFunctionName().isEmpty()) {
						System.out.print("!" + frameInfo.getFunctionName() + "[");
						System.out.print(frameInfo.getSourceFileName() + ":");
						System.out.print(frameInfo.getSourceLine() + "]");
					}
					System.out.println("+" + String.format("0x%016x", frameInfo.getModuleOffset()));
				}
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
}
