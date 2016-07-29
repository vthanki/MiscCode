#include <jni.h>
#include <stdio.h>
#include <assert.h>
#include "com_vishal_learning_Main.h"
 
JNIEXPORT void JNICALL Java_com_vishal_learning_Main_sayHello
  (JNIEnv *env, jobject thisObj)
{
	printf("hello...\n");
	return;
}
JNIEXPORT jobject JNICALL
Java_com_vishal_learning_Main_createPolygon
(JNIEnv *env, jobject thisObj) 
{
	jmethodID cnstrctr;
	jclass c = (*env)->FindClass(env, "com/vishal/learning/Polygon");
	if (c == 0)
		printf("Find Class Failed.\n");
	else
		printf("Found class.\n");

	cnstrctr = (*env)->GetMethodID(env, c, "<init>", "()V");
        if (cnstrctr == 0) {
            printf("Find method Failed.\n");
        }else {
            printf("Found method.\n");
        }

        return (*env)->NewObject(env, c, cnstrctr);
}

/*
 * Class:     com_vishal_learning_Main
 * Method:    addSideToPolygon
 * Signature: (Lcom/vishal/learning/Polygon;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_vishal_learning_Main_addSideToPolygon
  (JNIEnv *env, jobject thisObj, jobject jobjPolygon, jstring jDummy)
{
	jclass jclsPolygon;
	jmethodID midAddSide;

	jclass jclsSide;
	jobject jobjSide;
	jmethodID midSideCons;


	/* Create a new object of class Side */
	jclsSide = (*env)->FindClass(env, "com/vishal/learning/Side");
	assert(jclsSide != NULL);
	midSideCons = (*env)->GetMethodID(env, jclsSide, "<init>", "(I)V");
	assert(midSideCons != NULL);
	jobjSide = (*env)->NewObject(env, jclsSide, midSideCons, 13);

	/* Access Polygon Class */
	jclsPolygon = (*env)->GetObjectClass(env, jobjPolygon);
	assert(jclsPolygon != NULL);
	midAddSide = (*env)->GetMethodID(env, jclsPolygon, "addSide",
			"(Lcom/vishal/learning/Side;)V");
	assert(midAddSide != NULL);
	(*env)->CallVoidMethod(env, jobjPolygon, midAddSide, jobjSide);
}
