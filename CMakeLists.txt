cmake_minimum_required(VERSION 3.31)

project(TurnLab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

find_package(spdlog REQUIRED)
find_package(dime REQUIRED)
find_path(QWT_INCLUDE_DIR qwt_plot.h
        PATHS /usr/include/qwt-qt6
        /usr/include/qwt
        /usr/local/include/qwt
        /usr/include/qt6/qwt
        /usr/local/include/qt6/qwt
        $ENV{QWT_ROOT}/include
)

find_library(QWT_LIBRARY NAMES qwt-qt6 qwt
        PATHS /usr/lib
        /usr/local/lib
        /usr/lib/x86_64-linux-gnu
        $ENV{QWT_ROOT}/lib
)

if(NOT QWT_INCLUDE_DIR OR NOT QWT_LIBRARY)
    message(FATAL_ERROR "QWT library not found")
endif()

# Create core library with all business logic
add_library(TurnLabCore STATIC
        src/model/geometry/Point.cpp
        src/model/geometry/Line.cpp
        src/model/geometry/Geometry.cpp
        src/utils/DXFUtils.cpp
        src/model/geometry/Segment.h
        src/model/geometry/Geometry.h
        src/model/geometry/Point.h
        src/model/geometry/Line.h
        src/utils/DXFUtils.h
        src/model/geometry/Vector.cpp
        src/model/geometry/Vector.h
)

target_include_directories(TurnLabCore PUBLIC
        src/model/geometry
        src/utils
)

target_link_libraries(TurnLabCore PUBLIC
        spdlog::spdlog
        dime::dime
)

qt6_add_resources(QT_RESOURCES resources.qrc)

add_executable(TurnLab 
        src/main.cpp 
        src/view/MainWindow.cpp 
        src/view/ToolTableDialog.cpp 
        src/view/LeftPanel.cpp 
        src/view/GeometryView.cpp
        src/view/GeometryView.h
        src/view/DXFImportDialog.cpp
        src/view/DXFImportDialog.h
        ${QT_RESOURCES}
        src/presenter/MainPresenter.cpp
        src/presenter/MainPresenter.h
        src/presenter/DXFImportPresenter.cpp
        src/presenter/DXFImportPresenter.h
)

target_link_libraries(TurnLab
        TurnLabCore
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
        ${QWT_LIBRARY}
)
target_include_directories(TurnLab PRIVATE ${QWT_INCLUDE_DIR})

# Include test configuration
add_subdirectory(test)

