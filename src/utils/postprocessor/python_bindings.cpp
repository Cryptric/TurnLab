//
// Created by gawain on 9/20/25.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <spdlog/spdlog.h>

#include "../../model/Tool.h"
#include "../../model/MachineConfig.h"
#include "../../model/toolpath/TPoint.h"
#include "../../model/toolpath/TToolpath.h"
#include "../../model/toolpath/TLine.h"
#include "../../model/toolpath/TToolpathSequence.h"

namespace py = pybind11;

// Base PostProcessor class that Python classes can inherit from
class PostProcessor {
public:
    PostProcessor(MachineConfig config, ToolTable toolTable) : config(config), toolTable(toolTable) {}
    virtual ~PostProcessor() = default;

    virtual std::string rapidMove(TPoint point) { return ""; }
    virtual std::string linearMove(TPoint point, double feedrate) { return ""; }
    virtual std::string spindleOn(double rpm) { return ""; }
    virtual std::string spindleOff() { return ""; }
    virtual std::string toolChange(int toolNumber) { return ""; }

protected:
    MachineConfig config;
    ToolTable toolTable;
};

void init_py_module(py::module& m) {
    spdlog::info("init_py_module called - starting binding registration");
    m.doc() = "TurnLab Python bindings for toolpath processing";

    spdlog::info("Registering Tool struct");
    // Bind Tool
    py::class_<Tool>(m, "Tool")
        .def(py::init<>())
        .def_readwrite("number", &Tool::number)
        .def_readwrite("description", &Tool::description)
        .def_readwrite("iso_code", &Tool::isoCode);

    spdlog::info("Registering ToolTable struct");
    // Bind ToolTable
    py::class_<ToolTable>(m, "ToolTable")
        .def(py::init<>())
        .def_readwrite("tools", &ToolTable::tools);

    spdlog::info("Registering MachineConfig struct");
    // Bind MachineConfig
    py::class_<MachineConfig>(m, "MachineConfig")
        .def(py::init<>());

    spdlog::info("Registering TToolpathType enum");
    // Bind TToolpathType enum
    py::enum_<TToolpathType>(m, "ToolpathType")
        .value("Line", TToolpathType::Line)
        .export_values();

    spdlog::info("Registering TPoint class");
    // Bind TPoint
    py::class_<TPoint>(m, "Point")
        .def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("x", &TPoint::x)
        .def_readwrite("z", &TPoint::z);

    spdlog::info("Registering TToolpath base class");
    // Bind TToolpath (abstract base class)
    py::class_<TToolpath>(m, "Toolpath")
        .def_readwrite("tool_number", &TToolpath::toolNumber)
        .def_readwrite("feed_rate", &TToolpath::feedRate)
        .def_readwrite("rpm", &TToolpath::rpm)
        .def_readonly("type", &TToolpath::type);

    spdlog::info("Registering TLine class as 'ToolpathLine'");
    // Bind TLine
    py::class_<TLine, TToolpath>(m, "ToolpathLine")
        .def(py::init<>())
        .def(py::init<const TPoint&, const TPoint&, int, double, double>(),
             py::arg("start"), py::arg("end"),
             py::arg("tool_number") = 0, py::arg("feed_rate") = 100.0, py::arg("rpm") = 1000.0)
        .def(py::init<double, double, double, double, int, double, double>(),
             py::arg("start_x"), py::arg("start_z"), py::arg("end_x"), py::arg("end_z"),
             py::arg("tool_number") = 0, py::arg("feed_rate") = 100.0, py::arg("rpm") = 1000.0)
        .def_readwrite("start", &TLine::start)
        .def_readwrite("end", &TLine::end);

    // Bind TToolpathSequence
    py::class_<TToolpathSequence>(m, "ToolpathSequence")
        .def(py::init<>())
        .def("add_line", py::overload_cast<const TPoint&, const TPoint&, int, double, double>(&TToolpathSequence::addLine),
             py::arg("start"), py::arg("end"),
             py::arg("tool_number") = 0, py::arg("feed_rate") = 100.0, py::arg("rpm") = 1000.0)
        .def("add_line", py::overload_cast<double, double, double, double, int, double, double>(&TToolpathSequence::addLine),
             py::arg("start_x"), py::arg("start_z"), py::arg("end_x"), py::arg("end_z"),
             py::arg("tool_number") = 0, py::arg("feed_rate") = 100.0, py::arg("rpm") = 1000.0)
        .def("size", &TToolpathSequence::size)
        .def("empty", &TToolpathSequence::empty)
        .def("clear", &TToolpathSequence::clear)
        .def("__len__", &TToolpathSequence::size);

    spdlog::info("Registering PostProcessor base class");
    // Base PostProcessor class for Python inheritance
    py::class_<PostProcessor>(m, "PostProcessor")
        .def(py::init<MachineConfig, ToolTable>())
        .def("rapid_move", &PostProcessor::rapidMove)
        .def("linear_move", &PostProcessor::linearMove)
        .def("spindle_on", &PostProcessor::spindleOn)
        .def("spindle_off", &PostProcessor::spindleOff)
        .def("tool_change", &PostProcessor::toolChange);
}