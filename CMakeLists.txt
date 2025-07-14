cmake_minimum_required(VERSION 3.5)

project(MineCleaner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql)

set(TS_FILES MineCleaner_zh_CN.ts)

file(GLOB_RECURSE PROJECT_SOURCES
    include/*.h
    src/*.cpp
    src/*.h
    src/menuscene.ui
    ${TS_FILES}
)

# Add MOC files generation for files containing Q_OBJECT macro
set(MOC_HEADERS
    src/gamewindow.h
    src/unititem.h
    # Add other headers containing Q_OBJECT macro here
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MineCleaner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${MOC_HEADERS}  # Include MOC headers here
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(MineCleaner SHARED
            ${PROJECT_SOURCES}
            ${MOC_HEADERS}  # Include MOC headers here
        )
        set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MineCleaner
            ${PROJECT_SOURCES}
            resource.qrc
            ${MOC_HEADERS}  # Include MOC headers here
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(MineCleaner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MineCleaner)
endif()

set_target_properties(MineCleaner PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MineCleaner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MineCleaner)
endif()
