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

qt6_add_resources(QT_RESOURCES resources.qrc)

add_executable(TurnLab src/main.cpp src/view/MainWindow.cpp src/view/ToolTableDialog.cpp src/view/LeftPanel.cpp ${QT_RESOURCES}
        src/view/GeometryView.cpp
        src/view/GeometryView.h
        src/model/geometry/Segment.h
        src/model/geometry/Geometry.h
        src/model/geometry/Point.h
        src/model/geometry/Point.cpp
        src/model/geometry/Line.h
        src/model/geometry/Line.cpp
        src/utils/DXFUtils.cpp
        src/utils/DXFUtils.h
        src/model/geometry/Geometry.cpp)

target_link_libraries(TurnLab
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
        spdlog::spdlog
        dime::dime
)

# Include test configuration
add_subdirectory(test)

