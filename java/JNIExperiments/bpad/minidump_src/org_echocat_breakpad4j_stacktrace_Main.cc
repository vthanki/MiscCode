#include <jni.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <istream>
#include <streambuf>
#include <string>
#include "org_echocat_breakpad4j_stacktrace_Main.h"
#include "common/rfStackTrace.h"
#include <ctype.h>
#include <sstream>

extern int RfMinidumpProcess(std::istream& dumpStream, const char *sym_paths, google_breakpad::RfStackTrace *rfStackTrace);
/*
 * Class:     com_vishal_learning_Main
 * Method:    getStackTrace
 * Signature: ([BLjava/lang/String;)Lorg/echocat/breakpad4j/stacktrace/StackTrace;
 */
JNIEXPORT jobject JNICALL Java_org_echocat_breakpad4j_stacktrace_Main_getStackTrace
  (JNIEnv *env, jclass jclsThis, jbyteArray jbyteFileStream, jstring jstrSymbolPath)
{

	/* Convert Byte Array into C++ std:istream */
	int len = env->GetArrayLength(jbyteFileStream);
	char* buffer = new char[len];
	assert(buffer != NULL);
	env->GetByteArrayRegion (jbyteFileStream, 0, len, reinterpret_cast<jbyte*>(buffer));
	std::stringstream dumpStream(ios_base::in | ios_base::out | ios_base::binary);
	dumpStream.write(buffer, len);

	/* Extract Symbol path string */
	jboolean iscopy;
	const char *symPath = env->GetStringUTFChars(jstrSymbolPath, &iscopy);

	/* Create RfStackTrace C++ class and populate the object */
	google_breakpad::RfStackTrace *rfStackTrace = new google_breakpad::RfStackTrace();
	assert(rfStackTrace != NULL);
	int ret = RfMinidumpProcess(dumpStream, symPath, rfStackTrace);
	assert(ret == 0);

	jclass jclsList = env->FindClass("java/util/List");

	jclass jclsStackTrace = env->FindClass("org/echocat/breakpad4j/stacktrace/StackTrace");
	jfieldID jfidStackTraceCrashCause = env->GetFieldID(jclsStackTrace, "crashCause", "Ljava/lang/String;");
	jfieldID jfidStackTraceOs = env->GetFieldID(jclsStackTrace, "os", "Ljava/lang/String;");
	jfieldID jfidStackTraceCpu = env->GetFieldID(jclsStackTrace, "cpu", "Ljava/lang/String;");
	jfieldID jfidThreadInfoList = env->GetFieldID(jclsStackTrace, "threads", "Ljava/util/List;");

	jclass jclsThreadInfo = env->FindClass("org/echocat/breakpad4j/stacktrace/ThreadInfo");
	jfieldID jfidThreadInfoThreadId = env->GetFieldID(jclsThreadInfo, "threadId", "J");

	jclass jclsFrameInfo = env->FindClass("org/echocat/breakpad4j/stacktrace/FrameInfo");
	jfieldID jfidFrameInfoModuleName = env->GetFieldID(jclsFrameInfo, "moduleName", "Ljava/lang/String;");
	jfieldID jfidFrameInfoFunctionName = env->GetFieldID(jclsFrameInfo, "functionName", "Ljava/lang/String;");
	jfieldID jfidFrameInfoSourceFileName = env->GetFieldID(jclsFrameInfo, "sourceFileName", "Ljava/lang/String;");
	jfieldID jfidFrameInfoSourceLine = env->GetFieldID(jclsFrameInfo, "sourceLine", "J");
	jfieldID jfidFrameInfoModuleOffset = env->GetFieldID(jclsFrameInfo, "moduleOffset", "J");
	jfieldID jfidFrameInfoFrameId = env->GetFieldID(jclsFrameInfo, "frameId", "I");


	jmethodID midStackTraceCons = env->GetMethodID(jclsStackTrace, "<init>", "()V");
	jmethodID midThreadInfoCons = env->GetMethodID(jclsThreadInfo, "<init>", "()V");
	jmethodID midFrameInfoCons = env->GetMethodID(jclsFrameInfo, "<init>", "()V");
	jmethodID midListAdd = env->GetMethodID(jclsList, "add", "(Ljava/lang/Object;)Z");

	jobject jobjStackTrace = env->NewObject(jclsStackTrace, midStackTraceCons);
	jobject jobjThreadInfoList = env->GetObjectField(jobjStackTrace, jfidThreadInfoList);

	env->SetObjectField(jobjStackTrace, jfidStackTraceCrashCause, env->NewStringUTF(rfStackTrace->getCrashCause()));
	env->SetObjectField(jobjStackTrace, jfidStackTraceOs, env->NewStringUTF(rfStackTrace->getOS()));
	env->SetObjectField(jobjStackTrace, jfidStackTraceCpu, env->NewStringUTF(rfStackTrace->getCPU()));

	for (int i = 0; i < rfStackTrace->threadCount(); i++) {
		google_breakpad::ThreadInfo thread = rfStackTrace->getThreadInfoAt(i);

		jobject jobjThreadInfo = env->NewObject(jclsThreadInfo, midThreadInfoCons);

		env->SetLongField(jobjThreadInfo, jfidThreadInfoThreadId, thread.threadId);

		env->CallBooleanMethod(jobjThreadInfoList, midListAdd, jobjThreadInfo);

		jfieldID jfidFrameInfoList = env->GetFieldID(jclsThreadInfo, "frames", "Ljava/util/List;");
		jobject jobjFrameInfoList = env->GetObjectField(jobjThreadInfo, jfidFrameInfoList);

		for (int j = 0; j < thread.frameCount(); j++) {
			google_breakpad::FrameInfo frame = thread.getFrameInfoAt(j);
			jobject jobjFrameInfo = env->NewObject(jclsFrameInfo, midFrameInfoCons);

			env->SetObjectField(jobjFrameInfo, jfidFrameInfoModuleName, env->NewStringUTF(frame.getBinFileName()));
			env->SetObjectField(jobjFrameInfo, jfidFrameInfoFunctionName, env->NewStringUTF(frame.getFuncName()));
			env->SetObjectField(jobjFrameInfo, jfidFrameInfoSourceFileName, env->NewStringUTF(frame.getSrcFileName()));
			env->SetLongField(jobjFrameInfo, jfidFrameInfoModuleOffset, frame.m_funcOffset);
			env->SetLongField(jobjFrameInfo, jfidFrameInfoSourceLine, frame.m_srcFileOffset);
			env->SetLongField(jobjFrameInfo, jfidFrameInfoFrameId, frame.m_frameId);

			env->CallBooleanMethod(jobjFrameInfoList, midListAdd, jobjFrameInfo);
		}
	}

	/* To be released before exiting */
	env->ReleaseStringUTFChars(jstrSymbolPath, symPath);
	delete[] buffer;
	delete rfStackTrace;
	return jobjStackTrace;
}
