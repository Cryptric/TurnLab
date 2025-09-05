# Lathe CAM Software - Features Specification

## Profile Loading Features

### DXF Import
- File chooser dialog for DXF selection
- Parse DXF file for 2D profile geometry
- Extract polylines, lines, and arcs from DXF entities

### DXF Import Configuration Dialog
- Visual preview of DXF geometry
- Interactive selection of middle line (profile centerline)
- Axial offset input (Z-direction shift)
- Radial offset input (X-direction shift) 
- Chuck side selection (left/right orientation)
- Units selection (mm, m, inch)
- Real-time preview of configured profile with offsets applied

## Tool Table Features

### Tool Table Dialog
- Table view with columns: Tool Nr, Description, ISO Code, Insert Visualization, Edit Button, Delete Button
- Small graphical representation of insert geometry based on ISO code
- Delete confirmation dialog when delete button is pressed
- Add button in last row spanning all columns
- JSON file persistence for tool table data

### Tool Edit Dialog
- Input field for tool number
- Text input for tool description
- Dropdown/input for ISO insert code specification
- Input field for nose radius
- Input field for cutting angle
- Input fields for tool offset (X and Z direction)
- Checkboxes for operation capabilities (turning, facing, grooving, threading, etc.)
- Default cutting parameters: RPM, feed rate, depth of cut
- Preview of insert shape based on selected ISO code

## Operation Features

### Facing Operation
- **Tool Selection**: Choose tool from tool table for facing operation
- **Cutting Parameters**: RPM and feedrate specification
- **Reference Geometry**: Select vertical line from loaded profile as reference
- **Cut Direction**: Specify left or right side of reference line for facing
- **Radial Distances**:
  - Clearance distance: Safe approach distance from part
  - Feed distance: Distance to engage feed rate before cutting
  - Outer distance: Radial position where facing cut begins
  - Inner distance: Radial position where facing cut ends
- **Axial Offset**: Distance offset along Z-axis from reference line (positive or negative)
- Single-pass facing operation with specified parameters

### Turning Operation
- **Tool Selection**: Choose tool from tool table for turning operation
- **Cutting Parameters**: RPM and feedrate specification
- **Reference Geometry**: Select horizontal line from loaded profile as reference
- **Cutting Direction**: Specify movement towards or away from chuck
- **Radial Distances**:
  - Clearance distance: Safe approach distance from part
  - Retract distance: Distance to retract after each pass
  - Feed distance: Distance to engage feed rate before cutting
  - Outer distance: Starting radial position for turning
  - Inner distance: Final radial position for turning
- **Axial Distances**:
  - Beginning offset: Z-axis offset at start of turning operation
  - End offset: Z-axis offset at end of turning operation
- **Stepover**: Radial depth of cut per pass for multiple-pass turning

### Contouring Operation
- **Tool Selection**: Choose tool from tool table for contouring operation
- **Cutting Parameters**: RPM and feedrate specification
- **Reference Geometry**: Select continuous section of profile geometry (multiple connected lines/arcs)
- **Cutting Direction**: Specify movement towards or away from chuck
- **Radial Distances**:
  - Clearance distance: Safe approach distance from part
  - Retract distance: Distance to retract after each pass
  - Feed distance: Distance to engage feed rate before cutting
  - Outer distance: Starting radial position for contouring
  - Inner distance: Final radial position for contouring
- **Axial Distances**:
  - Beginning offset: Z-axis offset at start of contouring operation
  - End offset: Z-axis offset at end of contouring operation
- **Stepover**: Radial depth of cut per pass for multiple-pass contouring

### Threading Operation
- **Tool Selection**: Choose tool from tool table for threading operation
- **Cutting Parameters**: RPM specification
- **Thread Pitch**: Thread pitch specification
- **Reference Geometry**: Select horizontal line from loaded profile as reference
- **Cutting Direction**: Specify movement towards or away from chuck
- **Radial Distances**:
  - Clearance distance: Safe approach distance from part
  - Retract distance: Distance to retract after each pass
  - Feed distance: Distance to engage feed rate before cutting
  - Outer distance: Starting radial position for threading
  - Inner distance: Final radial position for threading
- **Axial Distances**:
  - Beginning offset: Z-axis offset at start of threading operation
  - End offset: Z-axis offset at end of threading operation
- **Stepover**: Radial depth of cut per pass for multiple-pass threading
- **Spring Passes**: Number of finishing passes at full thread depth

### Parting Off Operation
- **Tool Selection**: Choose tool from tool table for parting off operation
- **Cutting Parameters**: RPM and feedrate specification
- **Reference Geometry**: Select vertical line from loaded profile as reference
- **Cut Direction**: Specify left or right side of reference line for parting
- **Radial Distances**:
  - Clearance distance: Safe approach distance from part
  - Retract distance: Distance to retract after each pass
  - Feed distance: Distance to engage feed rate before cutting
  - Outer distance: Starting radial position for parting
  - Inner distance: Final radial position for parting
- **Axial Offset**: Distance offset along Z-axis from reference line
- **Cut Depth Per Pass**: Radial depth of material removal before retracting and advancing again

### Drilling Operation
- **Tool Selection**: Choose tool from tool table for drilling operation
- **Cutting Parameters**: RPM and feedrate specification
- **Reference Geometry**: Select geometry specifying the drilling depth
- **Axial Distance**: Depth offset from reference geometry
- **Peck Depth**: Depth of material removal per drilling cycle before retraction
- **Dwell Time**: Pause duration at full depth for chip breaking and surface finish

## Main GUI Features

### Ribbon Bar
- **File Operations**: Save project, load project buttons
- **DXF Import**: Load DXF file button with configuration dialog
- **Export**: G-code generation and export functionality
- **Tool Management**: Open tool table dialog button
- **Operation Buttons**: Individual buttons for each lathe operation (facing, turning, contouring, threading, parting off, drilling)
  - Buttons are deactivated until DXF is loaded
  - Each button opens operation configuration in sidebar
- **Settings**: General application settings button

### Sidebar - Dual Purpose Design
**Operation List Mode**:
- Displays list of configured operations in execution order
- Each operation item is expandable with "+" button
- Expanding shows operation parameters summary
- Click on operation item to edit/reconfigure

**Operation Configuration Mode**:
- Activated when adding new operation or editing existing operation
- Three-tab configuration interface:
  1. **Tool & Cutting Tab**: Tool selection, RPM, feedrate, stepover, and operation-specific parameters
  2. **Geometry & Direction Tab**: Reference geometry selection, cutting direction, axial distances and offsets
  3. **Radial Distances Tab**: Clearance, retract, feed, outer, and inner distance parameters
- OK button generates toolpath and returns to operation list mode
- Cancel button discards changes and returns to operation list mode

### Main Visualization Area
- Real-time display of loaded DXF profile geometry
- Visual representation of all configured operation toolpaths
- Color-coded toolpaths for different operations
- Zoom and pan controls for detailed inspection
- Coordinate system display with clear axis labeling
- Visual feedback during operation configuration (preview of selected geometry)

## Machine Configuration Features

### Machine Configuration Dialog
- **Axis Direction Configuration**:
  - Z-axis direction: Select whether moving from tailstock towards chuck is positive or negative
  - X-axis direction: Select whether moving away from centerline is positive or negative
- **Machine Limits**:
  - Maximum spindle speed (RPM)
  - Maximum feed rates (mm/min and mm/rev)
  - Travel limits: Maximum Z travel distance, maximum X radius
  - Minimum clearance distances for safety
- **Display Settings**:
  - Display precision: Number of decimal places for coordinates
- **Persistence**: Configuration stored in JSON file for automatic loading
- **Chuck Position**: Fixed on left side (no configuration needed)

## Post-Processing Features

### Python Script Post-Processor
- **Post-processor Implementation**: Python script with predefined function interface
- **Required Functions**:
  - `initialize()`: Setup machine-specific headers, coordinate systems, units
  - `rapid_move(x, z)`: Generate rapid traverse G-code commands
  - `linear_move(x, z, feedrate)`: Generate linear interpolation with specified feed rate
  - `spindle_on(rpm, direction)`: Start spindle with speed and rotation direction
  - `spindle_off()`: Stop spindle operation
  - `coolant_on()` / `coolant_off()`: Coolant system control
  - `tool_change(tool_number)`: Tool change sequence generation
  - `finalize()`: Program end, return to home position, cleanup operations
- **Optional Functions**:
  - `dwell(seconds)`: Generate dwell/pause commands
  - `thread_move(x, z, pitch)`: Threading cycle generation
  - `comment(text)`: Add comments to G-code output
- **Operation Processing**: Post-processor receives operation list and generates G-code by calling appropriate functions

## Project File Management

### Project Save/Load Features
- **Project File Content**:
  - Geometry profile data (loaded from DXF)
  - Complete operations list with all configured parameters
  - DXF import configuration (offsets, units, chuck side selection)
- **File Format**: JSON format for project files
- **Machine Configuration**: Stored separately as global configuration (not part of project files)
- **Project Persistence**: Save current project state and restore all operations and geometry on load
