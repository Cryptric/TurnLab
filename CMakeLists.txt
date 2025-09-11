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
        ${QT_RESOURCES}
)

target_link_libraries(TurnLab
        TurnLabCore
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
)

# Include test configuration
add_subdirectory(test)

