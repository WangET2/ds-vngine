import sys
from PIL import Image, ImageOps
from pathlib import Path

def create_bg_grit(name):
    with open(f"{name}.grit", "w") as f:
            f.write("-gB8 -gb -gTFF00FF")

def convert_backgrounds(in_dir:Path=Path("assets/graphics/bg"), out_dir:Path=Path("build/assets/graphics/bg")):
    print("Converting backgrounds...")
    in_dir = Path(in_dir)
    out_dir = Path(out_dir)
    for img in in_dir.iterdir():
        if img.suffix == ".png" or img.suffix == ".jpg":
            out_file = out_dir / img.name
            if not out_file.exists() or out_file.stat().st_mtime < img.stat().st_mtime:
                convert_and_save_background(img, out_dir)
    print("Background conversion done.")
    return

def convert_and_save_background(img: Path, out_dir: Path):
    if img.suffix == ".png" or img.suffix == ".jpg":
        try:
            im = Image.open(Path(img))
        except OSError:
            print(f"Error loading file located at {img}")
            return
        im = ImageOps.fit(im, (256, 192), centering=(0.5,0.5))
        try:
            out_dir.mkdir(parents=True, exist_ok=True)
            im.save(out_dir / f"{img.stem}.png")
            create_bg_grit(out_dir / img.stem)
        except Exception as e:
            print(f"Error saving file located at {img}: {e}")
            return
    return


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: backgroundmaker.py <input1.png> <input2.png> <input3.png>...")
        sys.exit(1)
    for i in range(1, len(sys.argv)):
        parent = Path(Path(sys.argv[i]).parent)
        stem = Path(Path(sys.argv[i]).stem)
        out_dir = parent / stem
        convert_and_save_background(Path(sys.argv[i]), out_dir)
    print("Done!")