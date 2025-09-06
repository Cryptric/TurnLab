# TurnLab - Software Architecture (arc42)

A simple CAM system for CNC lathes. Import DXF profiles, configure machining operations, and generate G-code with customizable Python post-processors.

---

## 1. Introduction and Goals

### 1.1 Requirements Overview

From the feature specification, the core requirements are:
- **DXF Import & Processing**: Parse 2D geometry for lathe profiles
- **Six Machining Operations**: Turning, facing, threading, drilling, parting off, contouring  
- **Tool Management**: Persistent tool library with ISO specifications
- **Visual Toolpath Generation**: Real-time preview and validation
- **G-code Export**: Python-based post-processor system
- **Machine Configuration**: Flexible axis setup and limits

### 1.2 Quality Goals

Based on typical CAM requirements and robotics/control systems needs:

| Priority | Quality Goal | Motivation |
|----------|--------------|------------|
| 1 | **Accuracy** | Toolpaths must be mathematically precise for safe machining |
| 2 | **Usability** | Intuitive workflow for operators without CAM expertise |
| 3 | **Extensibility** | Easy to add new operations and post-processors |
| 4 | **Performance** | Real-time visualization for complex profiles |
| 5 | **Reliability** | Robust error handling to prevent machine crashes |

### 1.3 Stakeholders

| Role | Goals/Expectations |
|------|-------------------|
| **Machinist/Operator** | Simple, reliable toolpath generation |
| **Developer** | Clean, maintainable architecture |
| **Machine Shop** | Safe, efficient G-code output |
| **Open Source Community** | Well-documented, extensible codebase |

**Target Deployment**: Personal/educational use with open-source distribution for the maker and small machine shop community.

---

## 2. Architecture Constraints

### 2.1 Technical Constraints

**Programming Language & Standards**:
- Primary implementation in **C++17** (modern features, good toolchain support)
- **Python integration** required for post-processor scripts (embedded Python or subprocess)
- Cross-platform support (Linux primary, Windows/macOS secondary)

**Core Libraries & Dependencies**:
- **DXF parsing**: libdxfrw (LGPL, lightweight, proven)
- **GUI Framework**: Qt6 (cross-platform, mature, excellent 2D graphics)
- **JSON parsing**: nlohmann/json (header-only, MIT license)
- **Python integration**: Python C API or subprocess calls
- **Mathematics**: Eigen (linear algebra, geometry operations)

**Build System**:
- **CMake** for cross-platform builds
- **C++17** standard minimum

**Performance Requirements**:
- Real-time toolpath preview for profiles up to ~1000 geometry elements
- Sub-second response for operation configuration changes
- Memory efficient for typical desktop systems (2-8GB RAM)

### 2.2 Organizational Constraints

**Documentation**:
- Doxygen for API documentation
- README with build instructions
- User manual and developer documentation

### 2.3 Conventions

**Code Style**:
- Modern C++ conventions (RAII, smart pointers, STL algorithms)
- Compact code style (per preferences)
- camelCase for variables, PascalCase for classes

**File Formats**:
- Configuration: JSON
- Projects: JSON  
- Tool library: JSON
- DXF import: Standard DXF format (R12-R2018)
- G-code export: Configurable via Python post-processors

---

## 3. System Scope and Context

### 3.1 Business Context

**System Purpose**: TurnLab converts 2D CAD profiles into G-code programs for CNC lathes through an intuitive visual interface.

**Key External Entities**:

| Entity | Input | Output | Description |
|--------|-------|---------|-------------|
| **CAD System** | DXF files | - | 2D profile geometry (lines, arcs, polylines) |
| **Machinist** | Operation parameters, tool selection | Visual feedback | Configures machining operations via GUI |
| **CNC Lathe** | - | G-code program | Receives generated toolpaths for execution |
| **Tool Manufacturer** | ISO specifications | - | Cutting tool geometry and parameters |
| **Python Scripts** | Toolpath data | G-code text | Post-processor for machine-specific output |

### 3.2 Technical Context

**Data Flow Overview**:
```
DXF File → Geometry Parser → Profile Model → Operation Planner → Toolpath Generator → Post-Processor → G-code
                                     ↑                    ↓
                            Tool Library ←→ GUI Interface ←→ Visualization Engine
```

**External Interfaces**:

| Interface | Technology | Protocol/Format | Purpose |
|-----------|------------|-----------------|---------|
| **DXF Import** | libdxfrw | DXF R12-R2018 | Load 2D geometry profiles |
| **File I/O** | C++ std::filesystem | JSON, TXT | Save/load projects, config, G-code |
| **Python Integration** | Python C API | Function calls | Execute post-processor scripts |
| **User Interface** | Qt6 Widgets/Graphics | Qt signals/slots | GUI interaction and visualization |
| **Operating System** | POSIX/Win32 API | File system calls | Cross-platform file operations |

**System Boundaries**:

**Inside TurnLab Scope**:
- DXF geometry parsing and validation
- Lathe operation planning (6 operation types)
- Toolpath calculation and collision detection
- 2D/3D toolpath visualization
- Tool library management
- Machine configuration
- Post-processor execution

**Outside TurnLab Scope**:
- DXF file creation (handled by CAD systems)
- G-code execution (handled by CNC controller)
- Tool path simulation with material removal
- CAM for other machine types (mills, routers)
- Network connectivity or cloud features

---

## 4. Solution Strategy

### 4.1 Architectural Pattern: Model-View-Presenter (MVP)

**MVP Benefits for CAM Software**:
- **Separation of Concerns**: Clear separation between UI logic, business logic, and data
- **Testability**: Business logic in Presenter can be unit tested without GUI
- **Real-time Updates**: Presenter coordinates between Model changes and View updates
- **Multiple Views**: Same Model/Presenter can support different visualization modes

**MVP Components**:
- **Model**: Geometry data, operations, tool library, machine configuration
- **View**: Qt-based GUI widgets, 2D/3D visualization, dialogs
- **Presenter**: Coordinates user actions, triggers model updates, manages view state

### 4.2 Data Architecture: Traditional Object Hierarchy

**Core Object Structure**:
```cpp
Project
├── GeometryProfile (DXF-loaded geometry)
│   ├── Line entities
│   ├── Arc entities  
│   └── Polyline entities
├── OperationList (ordered sequence)
│   ├── TurningOperation
│   ├── FacingOperation
│   ├── ThreadingOperation
│   └── ... (other operation types)
├── ToolLibrary
│   └── Tool objects with ISO specifications
└── MachineConfig (axis directions, limits)
```

**Benefits**:
- **Intuitive relationships**: Clear parent-child hierarchy matches domain concepts
- **Memory efficiency**: Direct object references, no scene graph overhead
- **Simple serialization**: Object hierarchy maps naturally to JSON structure

### 4.3 Key Strategic Decisions

**Real-time Toolpath Generation**:
- **Incremental calculation**: Only recalculate affected toolpaths on parameter changes
- **Background threading**: Heavy calculations on worker threads with progress updates
- **Caching strategy**: Cache intermediate results for complex geometry operations
- **Observer pattern**: Model notifies Views immediately when toolpaths update

**Operation History Management**:
- **Command pattern**: Each user action as reversible command object
- **Memento pattern**: Store operation state snapshots for undo/redo
- **History stack**: Configurable depth (default 50 operations)
- **Granular commands**: Parameter changes, operation additions/deletions, geometry modifications

**Technology Integration Strategy**:
- **Qt6 Integration**: Leverage Qt's model/view framework for tool tables and operation lists
- **Eigen Mathematics**: Use Eigen for all geometric transformations and calculations
- **Python Embedding**: Embedded Python interpreter for post-processor execution

---

## 5. Building Block View

### Level 1: System Overview (Black Box)

The highest level view - TurnLab as a single black box with its main interfaces:

```
[DXF Files] → [TurnLab System] → [G-code Files]
                    ↕
              [User Interface]
                    ↕
              [Configuration Files]
```

### Level 2: Main Components (White Box)

System decomposition into main architectural layers:

```cpp
TurnLab Application
├── Presentation Layer
│   ├── MainWindow (Qt)
│   ├── ToolDialog 
│   ├── OperationConfigDialog
│   └── VisualizationWidget
├── Application Layer  
│   ├── MainPresenter
│   ├── ToolLibraryPresenter
│   ├── OperationPresenter
│   ├── VisualizationPresenter
│   └── CommandHistory
├── Business Logic Layer
│   ├── GeometryEngine
│   ├── ToolpathGenerator
│   └── OperationManager
├── Domain Model Layer
│   ├── Project
│   ├── GeometryProfile
│   ├── Operations (6 types)
│   ├── ToolLibrary
│   └── MachineConfig
└── Infrastructure Layer
    ├── DXFImporter
    ├── JSONSerializer
    ├── PythonPostProcessor
    └── FileManager
```

### Level 3: Detailed Component Breakdown

#### Presentation Layer Components

**MainWindow**
- **Purpose**: Main application window, ribbon bar, menu management
- **Responsibilities**: 
  - Coordinate overall UI layout
  - Handle file operations (save/load project)
  - Manage application state (DXF loaded, operations configured)
- **Interfaces**:
  - Signals: `dxfImportRequested()`, `projectSaveRequested()`, `projectOpenRequested()`, `operationButtonClicked(OperationType)`, `gcodeExportRequested()`, `toolDialogRequested()`, `machineConfigRequested()`, `segmentsSelected()`
  - Slots: `onProjectLoaded()`, `onOperationAdded()`, `onSegmentSelectionModeChanged(selectionMode)`, `onGcodeUpdated()`

**ToolDialog** 
- **Purpose**: Tool library management interface
- **Responsibilities**:
  - Display tool table with ISO codes and visualizations
  - Handle tool CRUD operations
  - Tool parameter editing
- **Interfaces**:
  - Signals: `toolAdded(Tool)`, `toolModified(Tool)`, `toolDeleted(int toolId)`
  - Slots: `refreshToolTable()`, `showToolEditor(int toolId)`

**OperationConfigDialog**
- **Purpose**: Three-tab operation configuration interface
- **Responsibilities**:
  - Tool & cutting parameters tab
  - Geometry & direction selection tab  
  - Radial distances configuration tab
- **Interfaces**:
  - Signals: `operationConfigured(Operation)`, `geometrySelectionRequested()`
  - Slots: `loadOperationConfig(Operation)`, `updateGeometryPreview()`

**VisualizationWidget**
- **Purpose**: 2D/3D toolpath and geometry display
- **Responsibilities**:
  - Render DXF geometry
  - Display toolpaths with color coding
  - Handle zoom/pan interactions
  - Interactive geometry selection

#### Application Layer Components

**MainPresenter**
- **Purpose**: Coordinate overall application workflow
- **Responsibilities**:
  - File operations coordination
  - DXF import workflow management
  - Project state management
- **Interfaces**:
  - Public methods: `loadDxf(path)`, `saveProject(path)`, `addOperation(OperationType)`
  - Dependencies: `Project`, `DXFImporter`

**ToolLibraryPresenter**
- **Purpose**: Manage tool library operations
- **Responsibilities**:
  - Tool CRUD coordination
  - ISO code validation
  - Tool visualization data preparation
- **Interfaces**:
  - Public methods: `addTool(Tool)`, `deleteTool(int)`, `getToolList()`
  - Dependencies: `ToolLibrary`, `ToolDialog`

**OperationPresenter**
- **Purpose**: Handle operation configuration workflow
- **Responsibilities**:
  - Operation parameter validation
  - Geometry selection coordination
  - Toolpath preview triggering
- **Interfaces**:
  - Public methods: `configureOperation(OperationType)`, `updateOperation(Operation)`
  - Dependencies: `OperationManager`, `GeometryEngine`

**VisualizationPresenter**
- **Purpose**: Coordinate visualization updates
- **Responsibilities**:
  - Geometry rendering coordination
  - Toolpath display management
  - View state synchronization
- **Interfaces**:
  - Public methods: `updateDisplay()`, `setGeometrySelection(bool)`
  - Dependencies: `VisualizationWidget`, `ToolpathGenerator`

**CommandHistory**
- **Purpose**: Undo/redo functionality
- **Responsibilities**:
  - Command execution and storage
  - History stack management
  - State restoration
- **Interfaces**:
  - Public methods: `executeCommand(Command)`, `undo()`, `redo()`
  - Dependencies: All presenters (for command creation)

#### Business Logic Layer Components

**GeometryEngine**
- **Purpose**: DXF geometry processing and geometric operations
- **Responsibilities**:
  - Parse DXF entities into internal format
  - Apply coordinate transformations and offsets
  - Geometric calculations (intersections, distances)
  - Geometry selection and highlighting
- **Interfaces**:
  - Public methods: `loadDxf(DxfData)`, `applyTransform(Matrix)`, `selectGeometry(Point)`
  - Dependencies: `GeometryProfile`

**ToolpathGenerator**
- **Purpose**: Generate toolpaths from operations and geometry
- **Responsibilities**:
  - Convert operations into G-code coordinate sequences
  - Apply tool offsets and compensation
  - Calculate multiple passes for roughing operations
  - Collision detection between tool and geometry
- **Interfaces**:
  - Public methods: `generateToolpath(Operation)`, `updateToolpath(Operation)`
  - Dependencies: `GeometryProfile`, `ToolLibrary`, `MachineConfig`

**OperationManager**
- **Purpose**: Manage the list of machining operations
- **Responsibilities**:
  - Operation sequence management
  - Operation parameter validation
  - Dependencies between operations
- **Interfaces**:
  - Public methods: `addOperation(Operation)`, `removeOperation(int)`, `getOperationList()`
  - Dependencies: `Project`

---

## 6. Runtime View

### Scenario 1: DXF Import Workflow

**Sequence of interactions from file selection to geometry display:**

```
User → MainWindow → MainPresenter → DXFImporter → GeometryEngine → VisualizationPresenter → VisualizationWidget
```

**Detailed Flow**:
1. User clicks "Import DXF" button in ribbon bar
2. MainWindow emits `dxfImportRequested()` signal
3. MainPresenter handles signal, opens file dialog
4. User selects DXF file, DXF configuration dialog appears
5. User configures offsets, chuck side, units
6. MainPresenter calls `DXFImporter::loadFile(path, config)`
7. DXFImporter uses libdxfrw to parse DXF entities
8. GeometryEngine processes entities with `loadDxf(DxfData)`
9. GeometryEngine applies coordinate transformations and offsets
10. GeometryEngine emits `geometryUpdated()` signal
11. VisualizationPresenter receives signal, calls `updateDisplay()`
12. VisualizationWidget renders geometry, fits view to content
13. MainWindow enables operation buttons
14. CommandHistory stores `ImportDxfCommand` for undo capability

### Scenario 2: Operation Configuration (Turning Operation)

**Sequence for configuring a turning operation with real-time preview:**

```
User → MainWindow → OperationPresenter → OperationConfigDialog → GeometryEngine → ToolpathGenerator → VisualizationPresenter
```

**Detailed Flow**:
1. User clicks "Turning" operation button
2. MainWindow emits `operationButtonClicked(OperationType::Turning)`
3. OperationPresenter handles signal, opens OperationConfigDialog
4. Dialog displays three tabs: Tool & Cutting, Geometry & Direction, Radial Distances
5. User selects tool from dropdown (populated from ToolLibrary)
6. User sets cutting parameters (RPM, feedrate, stepover)
7. User clicks "Select Reference Geometry" button
8. Dialog emits `geometrySelectionRequested()` signal
9. VisualizationPresenter enables geometry selection mode
10. User clicks on horizontal line in visualization
11. VisualizationWidget emits `geometrySelected(GeometryEntity)`
12. OperationPresenter receives selection, updates dialog preview
13. User adjusts radial distances (clearance, feed, outer, inner)
14. On each parameter change, dialog emits parameter change signal
15. OperationPresenter triggers `ToolpathGenerator::generatePreview(operation)`
16. ToolpathGenerator calculates toolpath coordinates
17. VisualizationPresenter updates display with preview toolpath
18. User clicks OK, operation is added to project
19. CommandHistory stores `AddOperationCommand`

### Scenario 3: Real-time Toolpath Update

**Sequence for parameter changes triggering immediate toolpath recalculation:**

```
OperationConfigDialog → OperationPresenter → ToolpathGenerator → VisualizationPresenter → VisualizationWidget
```

**Detailed Flow**:
1. User changes parameter in dialog (e.g., outer diameter)
2. Dialog input widget emits value change signal
3. OperationPresenter receives signal via slot connection
4. OperationPresenter validates parameter range and consistency
5. If valid, OperationPresenter calls `ToolpathGenerator::updateToolpath(operation)`
6. ToolpathGenerator recalculates affected toolpath segments
7. ToolpathGenerator emits `toolpathUpdated(operationId)` signal
8. VisualizationPresenter receives signal
9. VisualizationPresenter calls `VisualizationWidget::updateToolpaths()`
10. VisualizationWidget redraws with updated toolpath
11. Update completes within target 100ms for responsive feedback

### Scenario 4: G-code Export Workflow

**Sequence from operation list to final G-code file generation:**

```
MainPresenter → OperationManager → ToolpathGenerator → PythonPostProcessor → FileManager
```

**Detailed Flow**:
1. User clicks "Export G-code" button
2. MainPresenter opens file save dialog
3. User selects output file path and post-processor script
4. MainPresenter calls `OperationManager::getOperationList()`
5. OperationManager returns ordered list of configured operations
6. For each operation in sequence:
   a. MainPresenter calls `ToolpathGenerator::generateToolpath(operation)`
   b. ToolpathGenerator calculates final toolpath coordinates
   c. ToolpathGenerator applies tool offsets and machine limits
   d. Toolpath data stored in export queue
7. MainPresenter calls `PythonPostProcessor::generateGcode(toolpaths, postProcessorScript)`
8. PythonPostProcessor executes Python script with toolpath data
9. Python script calls required functions (initialize, rapid_move, linear_move, etc.)
10. PythonPostProcessor collects G-code output from script execution
11. MainPresenter calls `FileManager::writeFile(path, gcodeContent)`
12. FileManager writes G-code to specified file
13. Success/error message displayed to user
14. CommandHistory stores `ExportGcodeCommand` for operation tracking

### Cross-cutting Runtime Concerns

**Error Handling Strategy**:
- Exceptions for critical errors (file I/O, memory allocation)
- Return codes for recoverable errors (invalid parameters)
- Qt signals for user-facing error messages
- Graceful degradation for non-critical failures

**Threading Model**:
- Main UI thread for all user interactions
- Background thread for DXF parsing (large files)
- Background thread for complex toolpath generation
- Background thread for G-code export
- Qt queued connections for thread-safe communication

**Performance Optimization**:
- Debouncing for real-time parameter updates (50ms delay)
- Incremental toolpath calculation (only affected segments)
- Level-of-detail rendering for complex geometry
- Lazy loading of tool visualizations

---

## 7. Deployment View

### 7.1 Development Environment

**Build System Configuration**:
```
TurnLab/
├── CMakeLists.txt (root configuration)
├── src/
│   ├── CMakeLists.txt
│   ├── presentation/
│   ├── application/
│   ├── business/
│   ├── domain/
│   └── infrastructure/
├── libs/ (third-party dependencies)
├── tests/
├── docs/
├── scripts/ (build and packaging scripts)
└── post-processors/ (example Python scripts)
```

**Dependency Management**:
- **System packages**: Qt6, Python development headers

**Required Dependencies**:
```cmake
# Core dependencies
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL)
find_package(PkgConfig REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(libdxfrw CONFIG REQUIRED)
```

### 7.2 Target Platform Deployment

#### Linux (Primary Platform - Arch Linux)

**Runtime Environment**:
- Qt6 libraries (system packages)
- Python 3.9+ interpreter
- libdxfrw shared library
- Standard C++ runtime (gcc/clang)


### 7.4 Configuration Management

**Configuration File Locations**:

| Platform | Configuration Directory | Purpose |
|----------|------------------------|---------|
| Linux | `~/.config/turnlab/` | User settings, machine config |

**Configuration Files**:
```
~/.config/turnlab/
├── machine_config.json (axis directions, limits)
├── tool_library.json (persistent tool database)
├── post_processors/ (user Python scripts)
└── projects/ (recent project files)
```

**Project Files** (user-specified locations):
- `.turnlab` extension (JSON format)
- Contains geometry, operations, import settings
- Portable across different machines


---

## 8. Cross-cutting Concepts

### 8.1 Coordinate Systems & Mathematical Conventions

**Lathe Geometry Constraints**:
- **Top-down view**: Chuck positioned on left side
- **Profile usage**: Only bottom half of loaded DXF profile used for computations
- **Rationale**: Lathe operations work on revolution of the bottom profile around Z-axis
- **Y-coordinate handling**: Positive Y-coordinates ignored or filtered out during import

**Supported Coordinate System Conventions**:
- **Convention 1 (Standard)**: Z+ away from chuck (toward tailstock), X+ away from centerline
- **Convention 2 (Alternative)**: Z+ toward chuck, X+ away from centerline  
- **Convention 3 (Mixed)**: Z+ toward chuck, X+ toward centerline
- **Convention 4 (Mixed)**: Z+ away from chuck, X+ toward centerline

**Implementation**:
- User selects convention in machine configuration dialog
- Internal coordinate transformations handle conversion
- G-code output uses selected convention
- Profile geometry filtered to X-Z plane, Y ≤ 0 coordinates only

**Units & Origin**:
- **Internal units**: Millimeters throughout application
- **Display units**: Millimeters only (no inch support)
- **Origin**: Configurable in machine setup (typically chuck face, centerline)

### 8.2 Error Handling Strategy

**Exception-based Error Handling**:
- **File I/O errors**: `std::runtime_error` for missing/corrupt files
- **Parsing errors**: `std::invalid_argument` for malformed DXF/JSON
- **Geometric errors**: Custom `GeometryException` for invalid operations
- **Parameter errors**: `std::out_of_range` for invalid cutting parameters

**Qt Integration**:
- Exceptions caught at presenter level
- Converted to user-friendly error messages via Qt message boxes
- Non-critical errors displayed in status bar

### 8.3 Threading Model

**Thread Organization**:
- **Main UI Thread**: All Qt widget interactions and user input
- **Worker Threads**: Background operations using `QThread`
  - DXF file parsing for large files
  - Complex toolpath generation
  - G-code export operations

**Thread Communication**:
- Qt signals/slots for thread-safe communication
- `QueuedConnection` for cross-thread signal delivery
- Progress updates via signals for long-running operations

### 8.4 File Format Standards

**Project Files (.turnlab)**:
- JSON format with nested structure
- Human-readable for debugging and version control
- Contains: geometry data, operation list, import configuration

**Configuration Files**:
- JSON format for machine config and tool library only
- Stored in platform-appropriate configuration directories
- Immediate persistence on configuration changes

**External Format Support**:
- **DXF Import**: R12-R2018 via libdxfrw library, Y ≤ 0 coordinates only
- **G-code Export**: Python post-processor scripts with standard function interface

### 8.5 Configuration Management

**Configuration Scope**:
- **Machine Configuration**: Coordinate conventions, axis directions, machine limits
- **Tool Library**: Persistent tool database with ISO specifications
- **Project Settings**: DXF import configuration, operation parameters

**Immediate Effect Policy**:
- All configuration changes take effect immediately
- No restart required for any setting changes
- Real-time preview updates when parameters change

**Single Machine Focus**:
- One machine configuration per installation
- Configuration stored locally in user config directory

---

## 9. Architecture Decisions

### 9.1 MVP Pattern Choice
- **Decision**: Model-View-Presenter over MVC/MVVM
- **Reason**: Better separation for testing, clear coordination of real-time updates
- **Trade-off**: Slightly more complex than MVC, but better testability

### 9.2 Qt6 GUI Framework
- **Decision**: Qt6 over alternatives (GTK, Dear ImGui, etc.)
- **Reason**: Excellent 2D graphics, mature, cross-platform, signal/slot system
- **Trade-off**: Larger dependency, but provides complete solution

### 9.3 Real-time Toolpath Generation
- **Decision**: Real-time calculation over batch processing
- **Reason**: Immediate user feedback improves workflow efficiency
- **Trade-off**: Higher CPU usage, but better user experience

### 9.4 Python Post-processors
- **Decision**: Python scripts over compiled plugins
- **Reason**: Easy customization by users, no compilation required
- **Trade-off**: Slightly slower execution, but much more flexible

### 9.5 Traditional Object Hierarchy
- **Decision**: Simple hierarchy over scene graph
- **Reason**: Matches domain concepts naturally, easier to understand
- **Trade-off**: Less flexible for complex scenes, but sufficient for lathe CAM

---

## 10. Quality Requirements

### 10.1 Correctness
- Toolpaths must be mathematically accurate for safe machining
- G-code output must match configured operations exactly
- Coordinate transformations must preserve geometric relationships

### 10.2 Responsiveness 
- Parameter changes provide immediate visual feedback
- UI remains responsive during background operations
- No blocking operations on the main thread

### 10.3 Robustness
- Recovers from invalid user input without crashing
- Provides clear error messages for failed operations

### 10.4 Simplicity
- Minimal learning curve for basic operations
- Clear visual representation of toolpaths and geometry
- Consistent interface patterns throughout application

### 10.5 Extensibility
- New machining operations can be added without major refactoring
- Post-processor scripts are user-customizable
- Configuration system supports future feature additions

---

## 11. Risks and Technical Debts

### 11.1 Technical Risks
- **DXF parsing complexity**: Different CAD systems produce varying DXF formats
- **Python integration**: Embedding Python interpreter adds complexity
- **Geometric calculations**: Floating-point precision issues in edge cases

### 11.2 Development Risks  
- **Library dependencies**: Updates to libdxfrw, Qt6, or Eigen could break builds
- **Cross-platform testing**: Limited ability to test on all target platforms

### 11.3 Technical Debts (Planned Shortcuts)
- **No coordinate validation**: Accepting risk of invalid moves for simplicity
- **No safety checks**: User responsible for safe machining parameters
- **No backup/recovery**: Simpler implementation, user manages file safety
- **Single machine config**: Easier development, limits multi-machine shops

---

## 12. Glossary

### Domain-Specific Terms
- **CAM**: Computer-Aided Manufacturing
- **DXF**: Drawing Exchange Format (AutoCAD file format)
- **G-code**: Numerical control programming language
- **Toolpath**: Sequence of coordinates defining tool movement
- **Post-processor**: Software that converts toolpath to machine-specific G-code
- **ISO insert code**: International standard for cutting tool identification

### Lathe-Specific Terms
- **Chuck**: Workpiece holding device on spindle
- **Tailstock**: Support for long workpieces, opposite chuck
- **Facing**: Cutting across the end of workpiece
- **Turning**: Cutting along the outside diameter
- **Parting off**: Cutting through workpiece to separate parts
- **Threading**: Cutting helical grooves for screws/bolts

### Technical Terms
- **MVP**: Model-View-Presenter architectural pattern
- **Qt signals/slots**: Event handling mechanism in Qt framework
- **libdxfrw**: Open-source DXF reading/writing library