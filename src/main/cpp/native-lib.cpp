#include <android/log.h>
#include <jni.h>
#include <string>
#include "native-lib.h"
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraMetadata.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraManager.h>
#include <media/NdkImageReader.h>
#include <android/native_window.h>
#include <vector>
#include <map>

JNIEXPORT jstring JNICALL Java_com_example_luciano_ndkcamera_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
///Encapsula a extração de informação se é backfacing ou não. Retorna o status da query.
camera_status_t isCameraBackFacing(const ACameraMetadata* metadataObj, bool& isBackFacing){
    ACameraMetadata_const_entry lensInfo = {0};
    camera_status_t status = ACameraMetadata_getConstEntry(metadataObj, ACAMERA_LENS_FACING, &lensInfo);
    if(status!=ACAMERA_OK){
        return status;
    }
    else{
        auto facing = static_cast<acamera_metadata_enum_android_lens_facing_t>(lensInfo.data.u8[0]);//pega o valor de facing
        if(facing == ACAMERA_LENS_FACING_BACK)
        {
            isBackFacing = true;
        }else{
            isBackFacing = false;
        }
        return status;
    }
}

camera_status_t getExposureTimeRange(const ACameraMetadata* metadataObj, int64_t &min, int64_t &max){
    ACameraMetadata_const_entry entry = { 0 };
    camera_status_t status = ACameraMetadata_getConstEntry(metadataObj, ACAMERA_SENSOR_INFO_EXPOSURE_TIME_RANGE, &entry);
    if(status != ACAMERA_OK){
        return status;
    }
    else{
        min = entry.data.i64[0];
        max = entry.data.i64[1];
        return status;
    }
}

camera_status_t getSensitivityRange(const ACameraMetadata* metadataObj, int32_t &min, int32_t &max){
    ACameraMetadata_const_entry entry = { 0 };
    camera_status_t status = ACameraMetadata_getConstEntry(metadataObj, ACAMERA_SENSOR_INFO_SENSITIVITY_RANGE, &entry);
    if(status != ACAMERA_OK){
        return status;
    }
    else{
        min = entry.data.i32[0];
        max = entry.data.i32[1];
        return status;
    }
}

camera_status_t getStreamConfiguration(const ACameraMetadata* metadataObj, std::vector<std::pair<int32_t, int32_t>>& dimensions){
    ACameraMetadata_const_entry entry = { 0 };
    camera_status_t status = ACameraMetadata_getConstEntry(metadataObj, ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &entry);
    if(status != ACAMERA_OK){
        return status;
    }
    else{
        for(auto i=0; i<entry.count; i+=4){
            int32_t input = entry.data.i32[i+3];
            if(input){
                continue;
            }

            int32_t format = entry.data.i32[i + 0];

            if (format == AIMAGE_FORMAT_JPEG)
            {
                std::pair<int32_t , int32_t> dimension = std::make_pair(entry.data.i32[i + 1], entry.data.i32[i + 2]);
                dimensions.push_back(dimension);
            }
        }
        return status;
    }
}


//https://www.sisik.eu/blog/android/ndk/camera
JNIEXPORT void JNICALL Java_com_example_luciano_ndkcamera_MainActivity_printCameras( JNIEnv *env, jobject /* this */){
    //O camera manager é o gerenciador do sistema de câmeras
    ACameraManager* cameraManager = ACameraManager_create();
    ACameraIdList *cameraIdList = nullptr;
    auto status = ACameraManager_getCameraIdList(cameraManager, &cameraIdList);//pega a lista de câmeras
    if(status!=ACAMERA_OK){//Verificação de erro
        __android_log_print(ANDROID_LOG_ERROR, "NdkCamera_native-lib", "falha ao lidar com camera = %d", status);
        return;
    }
    std::string backCameraId = "";
    //percorre a lista de câmeras do dispositivo.
    for(int i = 0; i<cameraIdList->numCameras; ++i){
        const char* id = cameraIdList->cameraIds[i];//cada camera tem um id
        ACameraMetadata* metadataObj;
        ACameraManager_getCameraCharacteristics(cameraManager, id, &metadataObj);//pega a metadata da câmera
        bool backFacing;
        auto backFaceQueryStatus = isCameraBackFacing(metadataObj, backFacing);
        if(backFacing){
            int64_t exposureMax, exposureMin;
            auto exposureQueryStatus = getExposureTimeRange(metadataObj, exposureMin, exposureMax);
            int32_t sensitivityMax, sensitivityMin;
            auto sensitivityQueryStatus = getSensitivityRange(metadataObj, sensitivityMin, sensitivityMax);
            std::vector<std::pair<int32_t, int32_t >> dimensions;
            auto dimensionsQueryStatus = getStreamConfiguration(metadataObj, dimensions);
            __android_log_print(ANDROID_LOG_ERROR, "NdkCamera_native-lib", "{exposureMax=%d, exposureMin=%d, sensitivityMax=%d"
                                                                           " sensitivityMin=%d}", exposureMax,
            exposureMin, sensitivityMax, sensitivityMin);
            std::for_each(dimensions.begin(), dimensions.end(), [](std::pair<int32_t, int32_t> p){
                __android_log_print(ANDROID_LOG_ERROR, "NdkCamera_native-lib", "{w=%d, h=%d}", p.first, p.second);
            });
        }

    }
    ACameraManager_deleteCameraIdList(cameraIdList);
    ACameraManager_delete(cameraManager);
}
