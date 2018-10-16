#ifndef NDKCAMERA_NATIVE_LIB_H
#define NDKCAMERA_NATIVE_LIB_H

#include <jni.h>
#define MainActivity_ Java_com_example_luciano_ndkcamera_MainActivity_
extern "C"
{
JNIEXPORT jstring JNICALL MainActivity_stringFromJNI( JNIEnv *env, jobject /* this */);
JNIEXPORT void JNICALL MainActivity_printCameras( JNIEnv *env, jobject /* this */);
};
#endif //NDKCAMERA_NATIVE_LIB_H
