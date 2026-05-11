#!/usr/bin/env python3

# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

from architectds import *
from tools.soundbank_mapgen import create_soundbank_gperf, build_soundbank_source

nitrofs = NitroFS()
nitrofs.add_grit(['assets/graphics'])
nitrofs.add_files_unchanged(['assets/scripts'], out_dir='scripts')
nitrofs.add_files_unchanged(['assets/graphics/sprites/offsets'], out_dir='offsets')
nitrofs_soundbank_header = nitrofs.add_mmutil(['assets/audio/sfx', 'assets/audio/bgm'])
nitrofs.generate_image()

arm9 = Arm9Binary(
    sourcedirs=['src'],
    libs=['nds9', 'mm9'],
    libdirs=['${BLOCKSDS}/libs/libnds', '${BLOCKSDS}/libs/maxmod'],
    includedirs=['include']
)
arm9.add_header_dependencies([nitrofs_soundbank_header])
create_soundbank_gperf()
build_soundbank_source()
arm9.generate_elf()

nds = NdsRom(
    binaries=[arm9, nitrofs],
    game_title='',
    game_subtitle='',
)
nds.generate_nds()

nds.run_command_line_arguments()
