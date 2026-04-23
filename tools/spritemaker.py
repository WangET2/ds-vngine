import os, sys
from PIL import Image, ImageOps
from pathlib import Path
import numpy as np

def create_sprite_grit(out_dir):
    for name in ["bleft","bright","tleft","tright"]:
        with open(f"{out_dir/name}.grit", "w") as f:
            f.write("-gB16 -gb -gTFF00FF")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: spritemaker.py <input1.png> <input2.png> <input3.png>...")
        sys.exit(1)
    for i in range(1, len(sys.argv)):
        try:
            im = Image.open(Path(sys.argv[i]))
        except OSError:
            print("Error loading file located at " + sys.argv[i])
            sys.exit(1)
        if im.format != ("PNG"):
            print("Warning! file " + sys.argv[i] + " is not a PNG!")
            print("Proceeding...")
        if im.size[0] != im.size[1]:
            print("Cropping image to square...")
            square = min(im.size)
            im = ImageOps.fit(im, (square, square), centering=(0.5,0.5))
        try:
            if im.size != (128, 128):
                print("Resizing image...")
                im = im.resize((128, 128), Image.Resampling.LANCZOS)
        except Exception as e:
            print("Error resizing file located at " + sys.argv[i] + f": {e}")
            sys.exit(1)
        print("Resizing complete. Tiling image...")
        try:
            topleft = im.crop((0,0,64,64))
            topright = im.crop((64,0,128,64))
            bottomleft = im.crop((0,64,64,128))
            bottomright = im.crop((64,64,128,128))
        except Exception as e:
            print("Error tiling file located at " + sys.argv[i] + f": {e}")
            sys.exit(1)
        print("Tiling complete. Setting transparency...")
        try:
            component_list = [topleft, topright, bottomleft, bottomright]
            for j in range(len(component_list)):
                img = component_list[j]
                img_arr = np.array(img)
                mask = img_arr[:, :, 3] < 50
                img_arr[mask, 0] = 255
                img_arr[mask, 1] = 0
                img_arr[mask, 2] = 255
                img_arr[mask, 3] = 255
                component_list[j] = Image.fromarray(img_arr, "RGBA")
        except Exception as e:
            print("Error setting transparency of file located at " + sys.argv[i] + f": {e}")
            sys.exit(1)
        print("Setting transparency complete. Saving file...")
        try:
            parent = Path(Path(sys.argv[i]).parent)
            stem = Path(Path(sys.argv[i]).stem)
            out_dir = parent / stem
            out_dir.mkdir(exist_ok=True)
            f, e = os.path.splitext(sys.argv[i])
            component_list[0].save(out_dir / "tleft.png")
            component_list[1].save(out_dir / "tright.png")
            component_list[2].save(out_dir / "bleft.png")
            component_list[3].save(out_dir / "bright.png")
            create_sprite_grit(out_dir)
        except Exception as e:
            print("Error saving file located at " + sys.argv[i] + f": {e}")
            sys.exit(1)
        print(f"{sys.argv[i]} Complete.")
    print("Done!")
