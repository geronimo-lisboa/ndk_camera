
cmake_minimum_required(VERSION 3.4.1)

add_library(native-lib SHARED src/main/cpp/native-lib.cpp)

find_library( camera-lib camera2ndk )
find_library( media-lib mediandk )
find_library( android-lib android )
find_library( gl-lib GLESv2 )
find_library(log-lib log)

target_link_libraries(native-lib ${log-lib})
target_link_libraries( native-lib ${camera-lib} ${media-lib} ${android-lib} ${gl-lib} )