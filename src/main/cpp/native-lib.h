#ifndef NDKCAMERA_NATIVE_LIB_H
#define NDKCAMERA_NATIVE_LIB_H

#include <jni.h>

extern "C"
{
JNIEXPORT jstring JNICALL Java_com_example_luciano_ndkcamera_MainActivity_stringFromJNI( JNIEnv *env, jobject /* this */);
JNIEXPORT void JNICALL Java_com_example_luciano_ndkcamera_MainActivity_printCameras( JNIEnv *env, jobject /* this */);
};
#endif //NDKCAMERA_NATIVE_LIB_H
