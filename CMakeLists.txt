cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME whisper_ui)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/whisperUiBin)

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX C CUDA)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(BUILD_SHARED_LIBS_DEFAULT ON)

option(BUILD_SHARED_LIBS              "whisper: build shared libs" ${BUILD_SHARED_LIBS_DEFAULT})

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /utf-8")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
    if(NOT WHISPER_NO_AVX2)
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX2")
        #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
    else()
        if(NOT WHISPER_NO_AVX)
            #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX")
            #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX")
            #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX")
        endif()
    endif()
endif()


add_subdirectory(3rd/ggml)
#add_subdirectory(3rd/whisper)

find_package(CUDAToolkit)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

find_package(SDL2 REQUIRED PATHS 3rd/SDL/cmake)

set(SRC
    main.cpp
    common.cpp
    common-ggml.cpp
    common-sdl.cpp
    common.h
    common-ggml.h
    common-sdl.h
    dr_wav.h
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    whisperstream.h
    whisperstream.cpp
    WContainer.h WContainer.cpp
    PaintSpectrum.h PaintSpectrum.cpp
    whisper/whisper.h
    whisper/whisper.cpp
    grammar-parser.cpp grammar-parser.h
)

add_executable(${PROJECT_NAME} ${SRC})

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WHISPER_BUILD GGML_SHARED)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    whisper
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${SDL2_LIBRARIES}
    #whisper::whisper
    ggml
)

#set_property(TARGET whisper PROPERTY INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
#set_property(TARGET whisper PROPERTY LIBRARIES whisper)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Widgets>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Core>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::Gui>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::QWindowsIntegrationPlugin>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/platforms/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Qt${QT_VERSION_MAJOR}::QModernWindowsStylePlugin>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
          $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS)
