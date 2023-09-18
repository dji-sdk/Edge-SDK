cmake_minimum_required(VERSION 3.9)
project(dji_edge_sdk_demo CXX)

set(LIB_SAMPLE edge_sample)
set(CMAKE_C_FLAGS "-pthread -std=gnu99")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
add_definitions(-D_GNU_SOURCE)

set(COMMON_CXX_FLAGS "-std=c++14 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

include_directories(include)
include_directories(examples/common)

file(GLOB_RECURSE MODULE_SAMPLE_SRC
        examples/init/pre_init.cc
        examples/init/key_store_default.cc
        examples/liveview/sample_liveview.cc
        examples/liveview/stream_decoder.cc
        examples/liveview/ffmpeg_stream_decoder.cc
        examples/liveview/image_processor_thread.cc
        examples/liveview/stream_processor_thread.cc
        examples/common/image_processor_dnn.cc)

link_directories(${CMAKE_CURRENT_LIST_DIR}/lib/${CMAKE_HOST_SYSTEM_PROCESSOR})
link_directories(${CMAKE_BINARY_DIR})
link_libraries(-ledgesdk -lstdc++)
link_libraries(-lcrypto)
link_libraries(-lssh2)

find_package(OpenCV QUIET)
if (OpenCV_FOUND)
    message("\n${PROJECT_NAME}...")
    message(STATUS "Found OpenCV installed in the system, will use it to display image in AdvancedSensing APIs")
    message(STATUS " - Includes: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS " - Libraries: ${OpenCV_LIBRARIES}")
    add_definitions(-DOPEN_CV_INSTALLED)
    execute_process(COMMAND opencv_version OUTPUT_VARIABLE OPENCV_VERSION)
    if (${OPENCV_VERSION} STRLESS "4.0.0")
        add_definitions(-DOPEN_CV_VERSION_3)
    else ()
        add_definitions(-DOPEN_CV_VERSION_4)
    endif ()
else ()
    message(STATUS "Did not find OpenCV in the system, image data is inside RecvContainer as raw data")
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
find_package(FFMPEG REQUIRED)
if (FFMPEG_FOUND)
    message(STATUS "Found FFMPEG installed in the system")
    message(STATUS " - Includes: ${FFMPEG_INCLUDE_DIR}")
    message(STATUS " - Libraries: ${FFMPEG_LIBRARIES}")

    EXECUTE_PROCESS(COMMAND ffmpeg -version
            OUTPUT_VARIABLE ffmpeg_version_output
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
    string(REGEX MATCH "version.*Copyright" ffmpeg_version_line ${ffmpeg_version_output})
    string(REGEX MATCH " .* " ffmpeg_version ${ffmpeg_version_line})
    string(REGEX MATCH "^ 5.*$" ffmpeg_major_version ${ffmpeg_version})

    if (HEAD${ffmpeg_major_version} STREQUAL "HEAD")
        message(STATUS " - Version: ${ffmpeg_version}")
    else ()
        message(FATAL_ERROR " - Not support FFMPEG version: ${ffmpeg_major_version}, please install 4.x.x instead.")
    endif ()

    include_directories(${FFMPEG_INCLUDE_DIR})
    add_definitions(-DFFMPEG_INSTALLED)
else ()
    message(STATUS "Cannot Find FFMPEG")
endif (FFMPEG_FOUND)

if (OpenCV_FOUND)
    link_libraries(${OpenCV_LIBS})
endif ()
if (FFMPEG_FOUND)
    link_libraries(${FFMPEG_LIBRARIES})
endif ()

add_library(${LIB_SAMPLE} STATIC ${MODULE_SAMPLE_SRC})

add_executable(sample_liveview  examples/liveview/sample_liveview_main.cc)
target_link_libraries(sample_liveview edge_sample)

add_executable(test_liveview examples/liveview/test_liveview_main.cc)
target_link_libraries(test_liveview edge_sample)

add_executable(sample_read_media_file examples/media_manager/sample_read_media_file.cc)
target_link_libraries(sample_read_media_file edge_sample)

add_executable(sample_media_file_list examples/media_manager/sample_media_file_list.cc)
target_link_libraries(sample_media_file_list edge_sample)

add_executable(sample_set_upload_cloud_strategy examples/media_manager/sample_set_upload_cloud_strategy.cc)
target_link_libraries(sample_set_upload_cloud_strategy edge_sample)

add_executable(sample_cloud_api examples/cloud_api/sample_cloud_api.cc)
target_link_libraries(sample_cloud_api edge_sample)

add_executable(pressure_test examples/test/pressure_test.cc)
target_link_libraries(pressure_test edge_sample)

if (NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

