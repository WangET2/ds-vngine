#!/usr/bin/env python3

# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

from architectds import *

nitrofs = NitroFS()
nitrofs.add_grit(['assets/graphics'])
nitrofs.add_files_unchanged(['assets/scripts'], out_dir='scripts')
nitrofs.add_files_unchanged(['assets/graphics/sprites/offsets'], out_dir='offsets')
nitrofs.generate_image()

arm9 = Arm9Binary(
    sourcedirs=['source'],
    includedirs=['include']
)
arm9.generate_elf()

nds = NdsRom(
    binaries=[arm9, nitrofs],
    game_title='',
    game_subtitle='',
)
nds.generate_nds()

nds.run_command_line_arguments()
