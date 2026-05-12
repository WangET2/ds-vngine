import sys
from PIL import Image, ImageOps
from pathlib import Path
import numpy as np

def create_sprite_grit(out_dir):
    for name in ["bleft","bright","tleft","tright"]:
        with open(f"{out_dir/name}.grit", "w") as f:
            f.write("-gB16 -gb -gTFF00FF")

def convert_sprites(in_dir: Path=Path("assets/graphics/sprites"), out_dir: Path=Path("build/assets/graphics/sprites")):
    print("Converting sprites...")
    in_dir = Path(in_dir)
    out_dir = Path(out_dir)
    for char_dir in in_dir.iterdir():
        if char_dir.is_dir():
            for img in char_dir.iterdir():
                if img.suffix == ".png" or img.suffix == ".jpg":
                    out_path = out_dir / char_dir.name / img.stem
                    if not out_path.is_dir() or out_path.stat().st_mtime < img.stat().st_mtime:
                        convert_and_save_sprite(img, out_path)
    print("Sprite conversion complete.")
    return

def convert_and_save_sprite(img: Path, out_dir: Path):
    try:
        im = Image.open(Path(img))
    except OSError:
        print(f"Error loading file located at {img}")
        return
    if im.size[0] != im.size[1]:
        square = min(im.size)
        im = ImageOps.fit(im, (square, square), centering=(0.5,0.5))
    try:
        if im.size != (128, 128):
            im = im.resize((128, 128), Image.Resampling.LANCZOS)
    except Exception as e:
        print(f"Error resizing file located at {img}: {e}")
        return
    try:
        topleft = im.crop((0,0,64,64))
        topright = im.crop((64,0,128,64))
        bottomleft = im.crop((0,64,64,128))
        bottomright = im.crop((64,64,128,128))
    except Exception as e:
        print(f"Error tiling file located at {img}: {e}")
        return
    try:
        component_list = [topleft, topright, bottomleft, bottomright]
        for j in range(len(component_list)):
            im = component_list[j]
            img_arr = np.array(im)
            mask = img_arr[:, :, 3] < 50
            img_arr[mask, 0] = 255
            img_arr[mask, 1] = 0
            img_arr[mask, 2] = 255
            img_arr[mask, 3] = 255
            component_list[j] = Image.fromarray(img_arr, "RGBA")
    except Exception as e:
        print(f"Error setting transparency of file located at {img}: {e}")
        return
    try:
        out_dir.mkdir(parents=True, exist_ok=True)
        component_list[0].save(out_dir / "tleft.png")
        component_list[1].save(out_dir / "tright.png")
        component_list[2].save(out_dir / "bleft.png")
        component_list[3].save(out_dir / "bright.png")
        create_sprite_grit(out_dir)
    except Exception as e:
        print(f"Error saving file located at {img}: {e}")
        return
    return

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: spritemaker.py <input1.png> <input2.png> <input3.png>...")
        sys.exit(1)
    for i in range(1, len(sys.argv)):
        parent = Path(Path(sys.argv[i]).parent)
        stem = Path(Path(sys.argv[i]).stem)
        out_dir = parent / stem
        convert_and_save_sprite(Path(sys.argv[i]), out_dir)
    print("Done!")