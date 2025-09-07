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
        REQUIRED)

qt6_add_resources(QT_RESOURCES resources.qrc)

add_executable(TurnLab src/main.cpp src/gui/MainWindow.cpp src/gui/ToolTableDialog.cpp ${QT_RESOURCES})
target_link_libraries(TurnLab
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

