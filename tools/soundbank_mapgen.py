"""
Maxmod generates soundbank_info.h, which defines macros used to load and play audio files.
At runtime, the engine cannot trivially associate strings with these macro values.
This script generates a .gperf file, which will build a table associating filename strings
to their corresponding value based on soundbank_info.h.
An alternative approach is to parse soundbank_info.h at runtime every time audio is to
be played. A table avoids the runtime overhead of file parsing.
The size of this table in memory should be negligible; even a hundred 256-character long
filenames shouldn't result in a table consuming more than ~30KB in memory.
"""

from pathlib import Path
import subprocess

SOUNDBANK_INFO_PATH = Path("build/assets/arm9/nitrofs/soundbank_info.h")
GPERF_OUTPUT_PATH = Path("src/soundbank.gperf")
CFILE_OUTPUT_PATH = Path("src/soundbank.c")

BGM_DELIM = "MOD_"
SFX_DELIM = "SFX_"

GPERF_HEADER = """%{
#include <string.h>
%}

%struct-type

struct MMAudioFile {
    const char name[256];
    int index;
};

%%
"""

GPERF_FOOTER = "%%"

GPERF_BUILD_CMD = ["gperf", "-C", "-E", "-m", "-f", f"--lookup-function-name=get_mmindex", f"--output-file={CFILE_OUTPUT_PATH}", f"{GPERF_OUTPUT_PATH}"]

def create_soundbank_gperf():
    try:
        with open(SOUNDBANK_INFO_PATH, "r") as src:
            with open(GPERF_OUTPUT_PATH, "w") as dst:
                dst.write(GPERF_HEADER)
                for rawline in src:
                    line = rawline.strip()
                    if BGM_DELIM in line or SFX_DELIM in line:
                        delimited = line.split()
                        # ["#define", "MOD/SFX_FILE_NAME" "int"]
                        dst.write(f"{delimited[1].lower()}, {delimited[2]}\n")
                dst.write(GPERF_FOOTER)
    except FileNotFoundError:
        print(f"Soundbank file at f{SOUNDBANK_INFO_PATH} not found.")

def build_soundbank_source():
    res = subprocess.run(GPERF_BUILD_CMD, capture_output=True, text=True)
    print(res.stdout)

