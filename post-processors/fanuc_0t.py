#!/usr/bin/env python3
"""
Fanuc 0-T Control Post-Processor
For machines with Fanuc 0-T, 0i-T, or similar controls
"""

class Fanuc0TPostProcessor(PostProcessor):
    """Post-processor for Fanuc 0-T control lathes"""

    def __init__(self, machine_config):
        super().__init__(machine_config)
        self.current_tool = None
        self.current_x = None
        self.current_z = None
        self.spindle_running = False

    def initialize(self):
        """Setup machine-specific headers and initialization"""
        self.add_line("O1001 (TURNLAB GENERATED PROGRAM)")
        self.add_line("(FANUC 0-T CONTROL)")
        self.add_line("")
        self.add_line("G18 (XZ PLANE)")
        self.add_line("G21 (METRIC)")
        self.add_line("G40 (CANCEL RADIUS COMP)")
        self.add_line("G80 (CANCEL CANNED CYCLES)")
        self.add_line("G97 (CONSTANT SPEED)")
        self.add_line("")

    def rapid_move(self, x, z):
        """Generate rapid traverse G-code commands"""
        x_str = f"X{x:.3f}" if x != self.current_x else ""
        z_str = f"Z{z:.3f}" if z != self.current_z else ""

        if x_str or z_str:
            self.add_line(f"G00{x_str}{z_str}")
            self.current_x = x
            self.current_z = z

    def linear_move(self, x, z, feedrate):
        """Generate linear interpolation with specified feed rate"""
        x_str = f"X{x:.3f}" if x != self.current_x else ""
        z_str = f"Z{z:.3f}" if z != self.current_z else ""

        if x_str or z_str:
            self.add_line(f"G01{x_str}{z_str}F{feedrate:.2f}")
            self.current_x = x
            self.current_z = z

    def spindle_on(self, rpm, direction=1):
        """Start spindle with speed and rotation direction"""
        if not self.spindle_running:
            if direction >= 0:
                self.add_line(f"M03S{int(rpm)}")
            else:
                self.add_line(f"M04S{int(rpm)}")
            self.spindle_running = True

    def spindle_off(self):
        """Stop spindle operation"""
        if self.spindle_running:
            self.add_line("M05")
            self.spindle_running = False

    def coolant_on(self):
        """Coolant system control - on"""
        self.add_line("M08")

    def coolant_off(self):
        """Coolant system control - off"""
        self.add_line("M09")

    def tool_change(self, tool_number):
        """Tool change sequence generation"""
        if self.current_tool != tool_number:
            self.add_line("")
            self.comment(f"TOOL {tool_number}")
            if self.spindle_running:
                self.spindle_off()
            self.add_line("G28U0.")
            self.add_line("G28W0.")
            self.add_line(f"T{tool_number:02d}{tool_number:02d}")
            self.current_tool = tool_number

    def finalize(self):
        """Program end, return to home position, cleanup operations"""
        self.add_line("")
        if self.spindle_running:
            self.spindle_off()
        self.add_line("G28U0.")
        self.add_line("G28W0.")
        self.add_line("M30")
        self.add_line("%")

    def thread_move(self, x, z, pitch):
        """Threading cycle generation for Fanuc controls"""
        if self.current_x is not None and self.current_z is not None:
            self.add_line(f"G32X{x:.3f}Z{z:.3f}F{pitch:.3f}")
            self.current_x = x
            self.current_z = z
            return True
        return False