cmake_minimum_required(VERSION 3.5)

project(proj-arknights LANGUAGES CXX)

file(GLOB_RECURSE QRC_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QRC_SOURCE_FILES res.qrc)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${QRC_SOURCE_FILES})

if(ANDROID)
  add_library(proj-arknights SHARED
    gamestate.cpp
    infected.cpp
    main.cpp
    mainwindow.cpp
    map.cpp
    operator.cpp
    place.cpp
    reunion.cpp

    gamestate.h
    infected.h
    mainwindow.h 
    map.h
    operator.h 
    place.h 
    reunion.h

    mainwindow.ui 
    map.ui 
  )
else()
  add_executable(proj-arknights
    ${QRC_FILES}
    
    gamestate.cpp
    infected.cpp
    main.cpp
    mainwindow.cpp
    map.cpp
    operator.cpp
    place.cpp
    reunion.cpp

    gamestate.h
    infected.h
    mainwindow.h 
    map.h
    operator.h 
    place.h 
    reunion.h

    mainwindow.ui 
    map.ui 
  )
endif()

target_link_libraries(proj-arknights PRIVATE Qt5::Widgets)
