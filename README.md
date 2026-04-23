# ds-vngine
[![Tests](https://github.com/WangET2/ds-vngine/actions/workflows/test.yml/badge.svg)](https://github.com/WangET2/ds-vngine/actions/workflows/test.yml)

A visual novel engine for Nintendo DS homebrew game development.

## Overview

## Table of Contents
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Building a ROM](#building-a-rom)
- [Project Structure](#project-structure)
- [Assets and Tools](#assets-and-tools)
- [Scripting Language](#scripting-language)
- [Design Notes](#design-notes)

## Dependencies

### For game development:
 - [BlocksDS SDK](https://blocksds.skylyrac.net/)
 - [ArchitectDS](https://codeberg.org/blocksds/architectds)
 - [ninja-build](https://ninja-build.org/)
 - [Python 3](https://www.python.org/)
 
### For asset creation:
 - [pillow](https://pypi.org/project/pillow/)
 - [numpy](https://pypi.org/project/numpy/)

### For testing:
 - [gmake](https://www.gnu.org/software/make/)
 - [gcc](https://gcc.gnu.org/)
 - [Unity Test](https://github.com/ThrowTheSwitch/Unity)

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
Note that if you change your mind later, you must manually initialize Unity as a submodule:
```
git submodule update --init --recursive
```

The [ArchitectDS build system](https://codeberg.org/blocksds/architectds) is used to generate NDS ROMs.

After installing its dependencies ([ninja](https://ninja-build.org/) and [Python](https://www.python.org/downloads/)), copy the `achitectds/` folder (from the repository linked above) into your project's root.

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
├── architectds/    #See Installation
├── include/        #Header files
├── src/            #Engine source files
├── assets/
│   ├── audio/          #Sound effects, music
│   ├── scripts/        #VN scripts (.txt)
│   └── graphics/
│       ├── bg/             # Background images
│       ├── sprites/        # Character sprites
│       │   └── offsets/    # Sprite offsets
│       └── ui/             # UI elements
├── tools/      #Asset conversion scripts
├── tests/      #Unity unit tests 
├── unity/      #Unity Test Framework                  
└── build.py    #ArchitectDS build configuration  
```

Other directories and files may appear, but these are build artifacts and can be safely ignored.

For the most part, game development can be done solely by adding assets to the correct subdirectory of `assets/` (see [Assets and Tools](#assets-and-tools)) and by modifying entry behavior and non-scripted behavior in `src/main.c`. A sample `src/main.c` is provided for convenience.

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
3. Copy the generated `.png` and `.grit` files located in the newly created `<yourimage>/` directory to the `assets/bg/` directory.

#### Character Sprites:
1. Prepare your sprite. It should be a `.png` file. If you want to avoid automatic cropping, manually crop your image to a square aspect ratio.
2. Run the command
```
python3 spritemaker.py <yourimage>.png
```
3. Copy all eight generated files (four `.png`, four `.grit`)  located in the newly created `<yourimage>/` directory.
4. Create the directory `assets/sprites/<character>/<expression>/` and paste the generated files. Replace `<character>` and `<expression>` with the name of the character and expression you would like to associate with the generated sprite.

#### UI Elements:
The engine expects to find all of the images already located in `assets/ui/` there at compile time as well.

1. Follow the steps above to generate a background image.
2. Rename the generated image to match the name of the UI element you want to replace. For example, if you want to replace `maintextbox.png`, rename the generated image to `maintextbox.png`.
3. Copy **only the `.png` file** to `assets/ui/` to replace the existing asset. **_Do not copy the generated `.grit` file or replace the existing `.grit` files in `assets/ui/`!_**

## Scripting Language

## Design Notes


