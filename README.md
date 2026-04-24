# ds-vngine [![Tests](https://github.com/WangET2/ds-vngine/actions/workflows/test.yml/badge.svg)](https://github.com/WangET2/ds-vngine/actions/workflows/test.yml)

A visual novel engine for Nintendo DS homebrew game development.

## Overview

ds-vngine is a visual novel engine inspired by the likes of [Ren'Py](https://www.renpy.org/) and [VNDS](https://github.com/BASLQC/vnds). It's meant to facilitate the low-code development of homebrew visual novels for the Nintendo DS.

The engine is built around a custom scripting language, and is designed to take advantage of the DS's titular dual screen layout. Character sprites, scene backgrounds, and dialogue are displayed on the upper scene, while the lower screen features narration and player choices.

Currently, the engine supports dialogue, narration, sprite and background rendering, setting and unsetting flags, and the conditional execution of instructions based on flags.

Features to be implemented soon include choices, sound effects, background music, save/load states, and text rollback.  

## Table of Contents
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Building a ROM](#building-a-rom)
- [Project Structure](#project-structure)
- [Assets and Tools](#assets-and-tools)
- [Scripting Language](#scripting-language)
- [Unit Tests](#unit-tests)
- [Design Notes](#design-notes)

## Dependencies

### For game development:
 - [BlocksDS SDK](https://blocksds.skylyrac.net/)
 - [ArchitectDS](https://codeberg.org/blocksds/architectds)
 - [ninja-build](https://ninja-build.org/)
 - [Python 3](https://www.python.org/)
 
### For asset creation:
 - [Pillow](https://pypi.org/project/pillow/)
 - [NumPy](https://pypi.org/project/numpy/)

### For testing:
 - [gmake](https://www.gnu.org/software/make/)
 - [gcc](https://gcc.gnu.org/)
 - [Unity Test](https://github.com/ThrowTheSwitch/Unity)
 - [CMock](https://github.com/throwtheswitch/cmock)
 - [Ruby](https://www.ruby-lang.org/en/documentation/installation/)

### For extending:
 - [gperf](https://www.gnu.org/software/gperf/)


## Installation
Begin by acquiring an installation of [BlocksDS](https://blocksds.skylyrac.net/docs/setup/).  

Next, clone the repo from GitHub:
```
git clone --recurse-submodules https://github.com/WangET2/ds-vngine.git
```
Alternatively, if you have no interest in running unit tests,
```
git clone https://github.com/WangET2/ds-vngine.git
```
Note that if you change your mind later, you must manually initialize Unity and CMock as submodules:
```
git submodule update --init --recursive
```

The [ArchitectDS build system](https://codeberg.org/blocksds/architectds) is used to generate NDS ROMs.

After installing its dependencies ([ninja](https://ninja-build.org/) and [Python](https://www.python.org/downloads/)), copy the `architectds/` folder (from the repository linked above) into your project's root.

## Building a ROM
The instructions ArchitectDS uses to build a ROM are located in `build.py`.

```
nds = NdsRom(
    binaries=[arm9, nitrofs],
    game_title='',
    game_subtitle='',
)
```

You can choose to update `game_title` and `game_subtitle` if desired.

To generate your ROM, run the following command:
```
python3 build.py
```
Ensure that you follow the instructions provided in the [BlocksDS getting started guide](https://blocksds.skylyrac.net/docs/setup/) so that your environment variables are set properly:  
- On macOS, you should be building ROMs from within a [Docker](https://docs.docker.com/get-docker/) container running an instance of the provided image.  
- On Windows, you should be building ROMs from the [Wonderful Toolchain Shell](https://wonderful.asie.pl/wiki/doku.php?id=getting_started:windows).

>**Note:** After updating project assets, clean all build files before generating a new ROM:

```
python3 build.py -c
python3 build.py
```
The generated ROM will be located in the root of your project with the `.nds` file extension.

This ROM can be run on a DS emulator, or, with the [NDS Homebrew Menu](https://github.com/devkitPro/nds-hb-menu), on an actual Nintendo DS. See [these sections](https://blocksds.skylyrac.net/docs/guides/faq/#what-emulators-are-recommended-to-test-my-code) of the BlocksDS documentation for more information. 

## Project Structure

```
ds-vngine/
├── architectds/    # See Installation
├── include/        # Header files
├── src/            # Engine source files
├── assets/
│   ├── audio/          # Sound effects, music
│   ├── scripts/        # VN scripts (.txt)
│   └── graphics/
│       ├── bg/             # Background images
│       ├── sprites/        # Character sprites
│       │   └── offsets/    # Sprite offsets
│       └── ui/             # UI elements
├── tools/      # Asset conversion scripts
├── tests/      # Unity unit tests 
├── unity/      # Unity Test Framework                  
└── build.py    # ArchitectDS build configuration  
```

Other directories and files may appear, but these are build artifacts and can be safely ignored.

For the most part, game development can be done solely by adding assets to the correct subdirectory of `assets/` (see [Assets and Tools](#assets-and-tools)) and by modifying entry behavior and non-scripted behavior in `src/main.c`. A sample [`src/main.c`](src/main.c) is provided for convenience.

## Assets and Tools

The Nintendo DS is somewhat restrictive in terms of the formats it expects for video and audio assets. This leads to a few quirks when developing for the system with modern frameworks.

### Image assets

All images in `assets/` are expected to be `.png` files.  

The [grit](https://github.com/blocksds/grit) tool is used by BlocksDS to convert `.png` images into a format compatible with the DS, `.grf`.  

However, grit requires instructions for how to convert images from `.png` to `.grf`. This means that every `.png` image is expected to have an associated `.grit` file containing conversion instructions.  

For example, `samplebg.png` must be paired with `samplebg.grit` in order to be displayed correctly.

Furthermore, sprites on the DS can only be 64x64 pixels large. This poses a problem for displaying character sprites, which are typically larger than 64x64 pixels.

Located in the `tools/` directory are Python scripts intended to automate away the tedium of asset conversion.

> Note that these scripts have dependencies:
```
pip3 install numpy
pip3 install pillow
```

#### Backgrounds:

1. Prepare your background image. It should be a `.png` file. If you want to avoid automatic cropping, manually crop your image to a 4:3 aspect ratio.
2. Run the command 
```
python3 backgroundmaker.py <yourimage>.png
```
3. Copy the generated `.png` and `.grit` files located in the newly created `<yourimage>/` directory to the `assets/graphics/bg/` directory.

#### Character Sprites:
1. Prepare your sprite. It should be a `.png` file. If you want to avoid automatic cropping, manually crop your image to a square aspect ratio.
2. Run the command
```
python3 spritemaker.py <yourimage>.png
```
3. Copy all eight generated files (four `.png`, four `.grit`)  located in the newly created `<yourimage>/` directory.
4. Create the directory `assets/graphics/sprites/<character>/<expression>/` and paste the generated files. Replace `<character>` and `<expression>` with the name of the character and expression you would like to associate with the generated sprite.

Character sprites can have (but do not require) an associated `offset.txt` file located at `assets/graphics/sprites/offsets/<character>/<expression>/`. This file should contain a single integer only, positive or negative, specifying a constant horizontal pixel shift to be applied to the corresponding sprite whenever it is displayed on screen.

Character sprites use magenta `(255, 0, 255)` as a transparency indicator, which is why the background of your sprites may appear bright purple in file previews. This is because grit struggles with converting transparency based on the alpha layer alone, leading to artifacts; thus, `spritemaker.py` recolors all transparent pixels to be magenta. If you notice misplaced transparent pixels in your converted sprites due to the original image containing magenta, consider using an image editor like [GIMP](https://www.gimp.org/) to alter their color slightly.

#### UI Elements:
The engine expects to find all of the images already located in `assets/ui/` there at compile time as well.

1. Follow the steps above to generate a background image.
2. Rename the generated image to match the name of the UI element you want to replace. For example, if you want to replace `maintextbox.png`, rename the generated image to `maintextbox.png`.
3. Copy **only the `.png` file** to `assets/ui/` to replace the existing asset. **_Do not copy the generated `.grit` file or replace the existing `.grit` files in `assets/ui/`!_**

## Scripting Language

The bulk of your visual novel's content will be outlined in scripts: `.txt` files located in the `assets/scripts/` directory.

These script files are written in a simple scripting language and must follow certain syntax rules:  
- Leading whitespaces and tabs are ignored.
- Trailing whitespaces and tabs are prohibited and will result in an error.
- Comments take up an entire line, and are denoted with the `#` symbol.
- Blank/empty lines are permitted.
- Exactly one space is expected between the arguments to an instruction.
- Each argument should be at most 256 characters in length.
- Each line should be at most 512 characters in length.

The scripting language provides a minimal instruction set for describing scene flow in script files.

The engine parses, interprets, and executes instructions within script files continuously without user input until it encounters a blocking instruction, such as `WAIT` or `SAY`.

Commands listed below are non-blocking unless otherwise specified.

### Display Instructions:

* `BG <bgname>` 
    * Loads the image located at `assets/graphics/bg/<bgname>.png` into VRAM and displays it as the scene background on the main (upper) screen.
* `HIDE_BG` 
    * Hides the scene background currently displayed on the main screen, if there is one.
    * Does not unload the scene background from VRAM.
* `BGSUB <bgname>`
    * Loads the image located at `assets/graphics/bg/<bgname>.png` into VRAM and displays it as the scene background on the sub (lower) screen.
* `HIDE_BGSUB` 
    * Hides the scene background currently displayed on the sub screen, if there is one.
    * Does not unload the scene background from VRAM.
* `SHOW_LEFT <character> <expression>`
    * Loads the sprite located at `assets/graphics/sprites/<character>/<expression>/` into VRAM and displays it on the left side of the main screen.
    * Note that only one sprite can be displayed in the left "slot" of the screen at a time. If a sprite is already being displayed in the left slot, calling this instruction with valid arguments will replace it.
* `HIDE_LEFT`
    * Hides the sprite currently being displayed in the left slot of the main screen, if there is one.
    * Does not unload the sprite from VRAM.
* `SHOW_RIGHT <character> <expression>`
    * Loads the sprite located at `assets/graphics/sprites/<character>/<expression>/` into VRAM and displays it on the right side of the main screen.
    * Note that only one sprite can be displayed in the right "slot" of the screen at a time. If a sprite is already being displayed in the right slot, calling this instruction with valid arguments will replace it.
* `HIDE_RIGHT`
    * Hides the sprite currently being displayed in the right slot of the main screen, if there is one.
    * Does not unload the sprite from VRAM.
* `SHOW_CENTER <character> <expression>`
    * Loads the sprite located at `assets/graphics/sprites/<character>/<expression>/` into VRAM and displays it in the center of the main screen.
    * Note that only one sprite can be displayed in the center "slot" of the screen at a time. If a sprite is already being displayed in the center slot, calling this instruction with valid arguments will replace it.
    * Note that displaying a sprite in the center slot of the screen is mutually exclusive with displaying a sprite in the left and/or right slots of the screen. Calling this instruction with valid arguments will hide any sprites currently being displayed in those slots.
* `HIDE_CENTER`
    * Hides the sprite currently being displayed in the center slot of the main screen, if there is one.
    * Does not unload the sprite from VRAM.

### Dialogue Instructions:

* `SAY <character> <dialogue>`
    * **Blocking instruction.**
    * Loads the textbox located at `assets/graphics/ui/maintextbox.png` into VRAM and displays it on the main screen, if not already displayed.
    * Writes `<character>` onto the main screen above dialogue text.
    * Begins writing `<dialogue>` onto the main screen with a typewriter effect.
        * The user can skip this animation by pressing the `A` button.
    * Blocks until text reveal is complete and the user presses the `A` button to proceed.
    * Note that the `<character>` argument is expected to not contain any whitespace. If it contains whitespace, the subsequent contents of the argument will be treated as part of the `<dialogue>` argument.
    * Note that the `<dialogue>` argument does not necessarily need to be surrounded by quotation marks.
    * Note that the `<character>` argument does not necessarily need to correspond to a "character" with sprites located in `assets/graphics/sprites/<character>/`.
* `HIDE_SAY`
    * Hides the textbox, character name, and dialogue currently being displayed on the main screen, if any.
    * Does not unload the textbox from VRAM.
* `NARRATE <narration>`
    * **Blocking instruction.**
    * Loads the textbox located at `assets/graphics/ui/subtextbox.png` into VRAM and displays it on the sub screen, if not already displayed.
    * Begins writing `<narration>` onto the sub screen with a typewriter effect.
        * The user can skip this animation by pressing the `A` button.
    * Blocks until text reveal is complete and the user presses the `A` button to proceed.
    * Note that the `<narration>` argument does not necessarily need to be surrounded by quotation marks.
* `HIDE_NARRATE`
    * Hides the textbox and narration text currently being displayed on the sub screen, if any.
    * Does not unload the textbox from VRAM.

### Control Flow Instructions:

* `FLAG <flag>`
    * Creates `<flag>`, marking it as active. Active flags can be checked by the `IF` and `IFN` instructions.
    * Calling this instruction multiple times on the same `<flag>` has no effect.
* `UNSET <flag>`
    * Removes `<flag>` if it exists, marking it as no longer active. 
    * `UNSET` flags can be recreated with the `FLAG` instruction.
* `IF <flag> <instruction>`
    * Checks if `<flag>` is active. If it is, executes `<instruction>`.
    * Note that the `IF` instruction can be blocking if the `<instruction>` argument is a blocking instruction and `<flag>` is active.
    * Note that the `<instruction>` argument can also be an `IF` or `IFN` instruction, enabling nested conditional execution as long as the line does not exceed the maximum line length and `<instruction>` does not exceed the maximum argument length.
 * `IFN <flag> <instruction>`
    * Checks if `<flag>` is active. If it is not active, executes `<instruction>`.
    * Note that the `IFN` instruction can be blocking if the `<instruction>` argument is a blocking instruction and `<flag>` is not active.
    * Note that the `<instruction>` argument can also be an `IF` or `IFN` instruction, enabling nested conditional execution as long as the line does not exceed the maximum line length and `<instruction>` does not exceed the maximum argument length.
* `LOAD <script>`
    * Loads the script located at `assets/scripts/<script>.txt` into memory and begins execution.
    * Note that any remaining instructions in the current (caller) script will not be executed, as it is unloaded from memory.
* `WAIT`
    * **Blocking instruction.**
    * Blocks until the user presses the `A` button.
* `PASS`
    * No-op (does nothing).
    * Intended to be used alongside the `CHOICE` instruction.
* `CHOICE <text_1> {<instruction_1>} ... <text_4> {<instruction_4>}`
    * **Blocking instruction.**
    * This instruction accepts anywhere between 2 and 4 `<text_i> {<instruction_i>}` pairs, inclusive.
    * Loads the textbox located at `assets/graphics/ui/choices<i>.png` into VRAM and displays it on the sub screen, if not already displayed.
    * Writes `<text_i>` onto the sub screen, vertically stacked.
    * This instruction blocks until the user selects a choice with the touchscreen or with the D-pad and `A` key.
    * If the user selects the choice associated with `<text_i>`, `<instruction_i>` will be executed.
    * Note that the `<instruction_i>` arguments must be wrapped with curly brackets `{}`.
* `END`
    * Marks the end of engine execution.
    * Can be used to return control to `src/main.c`.
    * Equivalent to reaching the end of a script file.

Your starting script can be executed by calling `engine_init();` and `engine_load_scene(char * scene_name)` in `src/main.c`. A [template `src/main.c`](src/main.c) is provided for reference.

## Unit Tests

If you choose to extend or update engine behavior and would like to ensure that your changes have not affected existing functionality, you can run the provided unit tests (See [Dependencies](#dependencies)).

To run unit tests:
```
make test
```

The provided tests cover the expected behavior of the flag and parser modules.

To write your own unit tests, see the [Unity testing framework documentation](https://github.com/ThrowTheSwitch/unity). Be sure to add any new test files to the `Makefile`.

You may find [CMock](https://www.throwtheswitch.org/cmock) useful for testing modules that directly or indirectly interact with DS hardware. 

## Design Notes

- The "flag" system is implemented as a statically allocated HashSet. While it would have been possible to implement it as a dynamically allocated HashSet instead, it felt inappropriate considering the DS's limited resources: the original DS only has 4 MB of RAM.

- The "flag" system is implemented as a HashSet rather than a HashMap. Theoretically, a HashMap would allow for simple counters, as a flag could be incremented each time a `FLAG` instruction is called on it. However, this would introduce more conditional instructions or require the introduction of conditional expressions in order to be useful. This remains an area for potential extension.

- While the instruction set is minimal and RISC-inspired, there are no instructions for loading data directly into memory or directly into VRAM. Due to the way the DS's VRAM is banked in particular, the engine "reserves" regions of memory for different types of data, such as scene backgrounds, UI layers, and sprites. On engine shutdown (typically on an `END` instruction or script end of file), the engine relinquishes control of the VRAM banks.

- Currently, text rendering is handled by the [`PrintConsole`](https://blocksds.skylyrac.net/libnds/structPrintConsole.html) struct provided by [libnds](https://blocksds.skylyrac.net/libnds/). Unfortunately, this solution does not allow for fine-grained control of text location or text palette memory management; thus, it is temporary.

- The parser is designed to be as simple and as safe as possible. String delimiting, in particular, was made to be as simple as possible, which is why arguments are primarily separated by spaces (with the exception of the `CHOICE` instruction). This is why the `<character>` argument of the `SAY` instruction cannot contain whitespaces; when a custom text rendering solution is implemented, a character (likely `_`) will be reserved to render nothing for characters with multi-word names.

- Intentionally absent from the syntax and instruction set are labels and a `JUMP` instruction. This was done to avoid the complexity of a two-pass parser or the overhead of loading an entire script file into memory. Instead, the `LOAD` instruction can be used to handle branching paths and even looping behavior in combination with `IF`/`IFN` instructions. Furthermore, `.txt` files contain relatively little metadata; a set of reasonably sized `.txt` files should not take up significantly more storage than one `.txt` file with all of their contents combined.