#include <android/log.h>
#include <jni.h>
#include <string>
#include "native-lib.h"
#include <camera/NdkCameraManager.h>
JNIEXPORT jstring JNICALL Java_com_example_luciano_ndkcamera_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
//https://www.sisik.eu/blog/android/ndk/camera
JNIEXPORT void JNICALL Java_com_example_luciano_ndkcamera_MainActivity_printCameras( JNIEnv *env, jobject /* this */){
    ACameraManager* cameraManager = ACameraManager_create();//isso aqui é um handle pra camer
    ACameraIdList *cameraIdList = nullptr;
    auto status = ACameraManager_getCameraIdList(cameraManager, &cameraIdList);
    if(status!=ACAMERA_OK){
        __android_log_print(ANDROID_LOG_ERROR, "NdkCamera_native-lib", "falha ao lidar com camera = %d", status);
        return;
    }
    for(int i = 0; i<cameraIdList->numCameras; ++i){
        const char* id = cameraIdList->cameraIds[i];
        ACameraMetadata* metadataObj;
        ACameraManager_getCameraCharacteristics(cameraManager, id, &metadataObj);
        __android_log_print(ANDROID_LOG_DEBUG, "NdkCamera_native-lib", "Camera id = %s", id);
    }
    ACameraManager_deleteCameraIdList(cameraIdList);
    ACameraManager_delete(cameraManager);
}
