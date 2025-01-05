cmake_minimum_required(VERSION 3.20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)

project(whisper VERSION 1.0 LANGUAGES C CXX CUDA)

find_package(CUDAToolkit)

set(SRC
    #ggml-opencl.cpp
    whisper.cpp
    whisper.h
    #ggml-opencl.cpp
    #ggml-opencl.h
)

set(DEFINES)

if(BUILD_SHARED_LIBS)
    add_library(whisper SHARED ${SRC})
    set(DEFINES
        ${DEFINES}
        WHISPER_SHARED
        WHISPER_BUILD
        GGML_SHARED
        GGML_BUILD
    )
else()
    add_library(whisper ${SRC})
endif()
add_library(whisper::whisper ALIAS whisper)

target_compile_definitions(whisper PRIVATE
    ${DEFINES}
    GGML_USE_CUBLAS
    #GGML_USE_CLBLAST
)


target_include_directories(whisper PUBLIC
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(whisper PRIVATE
    CUDA::cudart CUDA::cublas CUDA::cublasLt cuda
)

#install(
#    TARGET whisper
#    EXPORT whisper
#    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(generated_headers "${CMAKE_CURRENT_BINARY_DIR}/")
set(whisper_export "${generated_headers}/whisper.h")

# https://cmake.org/cmake/help/v3.9/module/GenerateExportHeader.html
include(GenerateExportHeader)
generate_export_header(whisper EXPORT_FILE_NAME ${whisper_export})

include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
#write_basic_package_version_file(
#    "${version_config}" COMPATIBILITY SameMajorVersion
#)
