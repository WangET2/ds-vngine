import os, sys
from PIL import Image, ImageOps
from pathlib import Path

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: backgroundmaker.py <input1.png> <input2.png> <input3.png>...")
        sys.exit(1)
    for i in range(1, len(sys.argv)):
        try:
            im = Image.open(Path(sys.argv[i]))
        except OSError:
            print("Error loading file located at " + sys.argv[i])
            sys.exit(1)
        print("Cropping image to 4:3...")
        im = ImageOps.fit(im, (256, 192), centering=(0.5,0.5))
        print("Resizing complete. Saving image...")
        try:
            f, e = os.path.splitext(sys.argv[i])
            im.save(f + "bg.png")
        except Exception as e:
            print("Error saving file located at " + sys.argv[i] + f": {e}")
            sys.exit(1)
        print(f"{sys.argv[i]} Complete.")
    print("Done!")