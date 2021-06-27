cmake_minimum_required(VERSION 3.5)

project(hikvision_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# скачать с оффициального сайта netsdk
set(NET_SDK_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/EN-HCNetSDKV6.1.6.45_build20210302_linux64")
set(INC_PATH "${NET_SDK_PATH}/incEn")
set(LIB_PATH "${NET_SDK_PATH}/lib")
add_executable(hikvision_client)

set(CMAKE_INSTALL_RPATH $ORIGIN)

file(GLOB_RECURSE PRJ_SRCS src/*.c src/*.cpp src/*.h src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${PRJ_SRCS})

target_include_directories(hikvision_client PRIVATE ${INC_PATH})
target_link_libraries(hikvision_client PRIVATE ${LIB_PATH}/libhcnetsdk.so)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,\${ORIGIN}")

install (TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

set(HIKI_SDK_INSTALL_FILES
    "${LIB_PATH}/libhcnetsdk.so"
    "${LIB_PATH}/libHCCore.so"
    "${LIB_PATH}/libhpr.so"
    "${LIB_PATH}/libssl.so"
    "${LIB_PATH}/libcrypto.so"
    "${LIB_PATH}/libcrypto.so.1.0.0"
    )
install (FILES ${HIKI_SDK_INSTALL_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX})

set(HIKI_SDK_INSTALL_FILES2
    "${LIB_PATH}/HCNetSDKCom/libHCAlarm.so"
    "${LIB_PATH}/HCNetSDKCom/libHCCoreDevCfg.so"
    )
install (FILES ${HIKI_SDK_INSTALL_FILES2} DESTINATION "${CMAKE_INSTALL_PREFIX}/HCNetSDKCom/")
