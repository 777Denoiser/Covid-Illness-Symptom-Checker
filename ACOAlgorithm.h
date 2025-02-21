#ifndef ACOAlgorithm_h
#define ACOAlgorithm_h

#include <jni.h>

extern "C" {
    JNIEXPORT jstring JNICALL Java_Main_runACOAlgorithm(JNIEnv* env, jobject obj, jstring inputData);
}

#endif
