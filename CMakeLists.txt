cmake_minimum_required(VERSION 3.31)

project(TurnLab LANGUAGES CXX C)

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
find_package(nlohmann_json REQUIRED)

find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Find QWT using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(QWT REQUIRED Qt6Qwt6)

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
        src/model/MachineConfig.h
        src/model/geometry/Transform.cpp
        src/model/geometry/Transform.h
        src/model/StockMaterial.h
        src/model/Project.h
        src/model/Tool.h
)

target_include_directories(TurnLabCore PUBLIC
        src/model/geometry
        src/utils
)

target_link_libraries(TurnLabCore PUBLIC
        spdlog::spdlog
        dime
        nlohmann_json::nlohmann_json
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
        src/view/MachineConfigDialog.cpp
        src/view/MachineConfigDialog.h
        ${QT_RESOURCES}
        src/presenter/MainPresenter.cpp
        src/presenter/MainPresenter.h
        src/presenter/DXFImportPresenter.cpp
        src/presenter/DXFImportPresenter.h
        src/presenter/MachineConfigPresenter.cpp
        src/presenter/MachineConfigPresenter.h
        src/view/OperationConfigurationView.cpp
        src/view/OperationConfigurationView.h
        src/utils/ConfigurationManager.cpp
        src/presenter/OperationConfigurationPresenter.h
        src/presenter/OperationConfigurationPresenter.cpp
        src/utils/ProjectUtils.h
        src/utils/ProjectUtils.cpp
        src/view/OperationConfigurationPlotHelper.cpp
        src/view/OperationConfigurationPlotHelper.h
        src/presenter/operation/FacingOperationPresenter.cpp
        src/presenter/operation/FacingOperationPresenter.h
        src/utils/GeometryUtils.cpp
        src/utils/GeometryUtils.h
        src/utils/toolpath/ToolpathGenerator.cpp
        src/utils/toolpath/ToolpathGenerator.h
        src/view/ToolpathPlotter.cpp
        src/utils/postprocessor/PythonPostProcessor.cpp
        src/utils/postprocessor/PythonPostProcessor.h
        src/utils/postprocessor/python_bindings.cpp
        src/utils/postprocessor/python_bindings.h
        src/presenter/operation/TurningOperationPresenter.cpp
        src/presenter/operation/TurningOperationPresenter.h
        src/presenter/operation/PartingOperationPresenter.cpp
        src/presenter/operation/PartingOperationPresenter.h
)

target_link_libraries(TurnLab
        TurnLabCore
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
        ${QWT_LIBRARIES}
        pybind11::embed
)
target_include_directories(TurnLab PRIVATE ${QWT_INCLUDE_DIRS})
target_compile_options(TurnLab PRIVATE ${QWT_CFLAGS_OTHER})

# Include test configuration
add_subdirectory(test)

