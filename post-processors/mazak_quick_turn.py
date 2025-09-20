#!/usr/bin/env python3
"""
Mazak Quick Turn Post-Processor
For Mazak Quick Turn series lathes with Mazatrol or EIA programming
"""

class MazakQuickTurnPostProcessor(PostProcessor):
    """Post-processor for Mazak Quick Turn lathes"""

    def __init__(self, machine_config):
        super().__init__(machine_config)
        self.current_tool = None
        self.current_x = None
        self.current_z = None
        self.program_number = 1001
        self.sequence_number = 10

    def get_next_sequence(self):
        """Get next sequence number and increment"""
        seq = self.sequence_number
        self.sequence_number += 10
        return seq

    def initialize(self):
        """Setup machine-specific headers and initialization"""
        self.add_line(f"O{self.program_number}")
        self.add_line("(MAZAK QUICK TURN LATHE)")
        self.add_line("(TURNLAB POST-PROCESSOR)")
        self.add_line("")

        # Mazak-specific initialization
        self.add_line(f"N{self.get_next_sequence()} G18 G21 G40 G80 G97")
        self.add_line(f"N{self.get_next_sequence()} G28 U0")
        self.add_line(f"N{self.get_next_sequence()} G28 W0")
        self.add_line("")

    def rapid_move(self, x, z):
        """Generate rapid traverse G-code commands"""
        coords = []
        if x != self.current_x:
            coords.append(f"X{x:.3f}")
            self.current_x = x
        if z != self.current_z:
            coords.append(f"Z{z:.3f}")
            self.current_z = z

        if coords:
            self.add_line(f"N{self.get_next_sequence()} G00 {' '.join(coords)}")

    def linear_move(self, x, z, feedrate):
        """Generate linear interpolation with specified feed rate"""
        coords = []
        if x != self.current_x:
            coords.append(f"X{x:.3f}")
            self.current_x = x
        if z != self.current_z:
            coords.append(f"Z{z:.3f}")
            self.current_z = z

        if coords:
            self.add_line(f"N{self.get_next_sequence()} G01 {' '.join(coords)} F{feedrate:.2f}")

    def spindle_on(self, rpm, direction=1):
        """Start spindle with speed and rotation direction"""
        # Mazak uses M03/M04 like most controls
        if direction >= 0:
            self.add_line(f"N{self.get_next_sequence()} M03 S{int(rpm)}")
        else:
            self.add_line(f"N{self.get_next_sequence()} M04 S{int(rpm)}")

    def spindle_off(self):
        """Stop spindle operation"""
        self.add_line(f"N{self.get_next_sequence()} M05")

    def coolant_on(self):
        """Coolant system control - on"""
        self.add_line(f"N{self.get_next_sequence()} M08")

    def coolant_off(self):
        """Coolant system control - off"""
        self.add_line(f"N{self.get_next_sequence()} M09")

    def tool_change(self, tool_number):
        """Tool change sequence generation"""
        if self.current_tool != tool_number:
            self.add_line("")
            self.add_line(f"(*** TOOL {tool_number} ***)")

            # Mazak tool change sequence
            self.add_line(f"N{self.get_next_sequence()} G28 U0")
            self.add_line(f"N{self.get_next_sequence()} G28 W0")
            self.add_line(f"N{self.get_next_sequence()} T{tool_number:02d}{tool_number:02d}")
            self.add_line(f"N{self.get_next_sequence()} G54")  # Work coordinate system
            self.current_tool = tool_number

    def finalize(self):
        """Program end, return to home position, cleanup operations"""
        self.add_line("")
        self.add_line("(*** PROGRAM END ***)")
        self.add_line(f"N{self.get_next_sequence()} G28 U0")
        self.add_line(f"N{self.get_next_sequence()} G28 W0")
        self.add_line(f"N{self.get_next_sequence()} M30")
        self.add_line("%")

    def thread_move(self, x, z, pitch):
        """Threading cycle generation for Mazak controls"""
        if self.current_x is not None and self.current_z is not None:
            # Mazak threading with G76 cycle
            self.add_line(f"N{self.get_next_sequence()} G76 X{x:.3f} Z{z:.3f} K{pitch:.3f}")
            self.current_x = x
            self.current_z = z
            return True
        return False

    def dwell(self, seconds):
        """Generate dwell/pause commands"""
        self.add_line(f"N{self.get_next_sequence()} G04 P{seconds:.1f}")
        return True

    def comment(self, text):
        """Add comments to G-code output"""
        self.add_line(f"({text})")
        return True

    def rough_turning_cycle(self, start_x, start_z, end_x, end_z, depth_of_cut, feedrate):
        """Mazak-specific rough turning cycle"""
        self.add_line("")
        self.comment("ROUGH TURNING CYCLE")

        # G71 rough turning cycle (Mazak specific)
        self.add_line(f"N{self.get_next_sequence()} G71 U{depth_of_cut:.3f} R1.")
        self.add_line(f"N{self.get_next_sequence()} G71 P{self.get_next_sequence() + 10} Q{self.get_next_sequence() + 20} U0.2 W0.05 F{feedrate:.2f}")

        # Profile definition
        seq_start = self.get_next_sequence()
        self.add_line(f"N{seq_start} G00 X{start_x:.3f} Z{start_z:.3f}")
        self.add_line(f"N{self.get_next_sequence()} G01 X{end_x:.3f}")
        seq_end = self.get_next_sequence()
        self.add_line(f"N{seq_end} G01 Z{end_z:.3f}")

        self.current_x = end_x
        self.current_z = end_z