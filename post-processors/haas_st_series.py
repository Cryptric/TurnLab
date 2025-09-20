#!/usr/bin/env python3
"""
Haas ST Series Lathe Post-Processor
For Haas ST-10, ST-20, ST-30, etc. turning centers
"""

class HaasSTPostProcessor(PostProcessor):
    """Post-processor for Haas ST series lathes"""

    def __init__(self, machine_config):
        super().__init__(machine_config)
        self.current_tool = None
        self.current_x = None
        self.current_z = None

    def initialize(self):
        """Setup machine-specific headers and initialization"""
        self.add_line("(HAAS ST SERIES LATHE)")
        self.add_line("(POST-PROCESSOR BY TURNLAB)")
        self.add_line("")
        self.add_line("G18 (XZ PLANE)")
        self.add_line("G20 (INCH)")
        self.add_line("G40 (CANCEL CUTTER COMP)")
        self.add_line("G80 (CANCEL CANNED CYCLES)")
        self.add_line("G99 (FEED PER REV)")
        self.add_line("")
        self.add_line("G28 U0 W0 (HOME)")
        self.add_line("")

    def rapid_move(self, x, z):
        """Generate rapid traverse G-code commands"""
        x_str = f"X{x:.4f}" if x != self.current_x else ""
        z_str = f"Z{z:.4f}" if z != self.current_z else ""

        if x_str or z_str:
            self.add_line(f"G00 {x_str} {z_str}")
            self.current_x = x
            self.current_z = z

    def linear_move(self, x, z, feedrate):
        """Generate linear interpolation with specified feed rate"""
        x_str = f"X{x:.4f}" if x != self.current_x else ""
        z_str = f"Z{z:.4f}" if z != self.current_z else ""

        if x_str or z_str:
            self.add_line(f"G01 {x_str} {z_str} F{feedrate:.3f}")
            self.current_x = x
            self.current_z = z

    def spindle_on(self, rpm, direction=1):
        """Start spindle with speed and rotation direction"""
        if direction >= 0:
            self.add_line(f"M03 S{int(rpm)}")
        else:
            self.add_line(f"M04 S{int(rpm)}")

    def spindle_off(self):
        """Stop spindle operation"""
        self.add_line("M05")

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
            self.add_line(f"(TOOL {tool_number})")
            self.add_line("G28 U0 W0")
            self.add_line(f"T{tool_number:02d}{tool_number:02d}")
            self.add_line("G54")
            self.current_tool = tool_number
            self.coolant_on()

    def finalize(self):
        """Program end, return to home position, cleanup operations"""
        self.add_line("")
        self.add_line("G28 U0 W0")
        self.add_line("M30")
        self.add_line("%")