"""
去除 PNG 圖片多餘的透明部分（自動裁切到最小範圍）
用法：python trim_sprites.py <圖片路徑或資料夾>
範例：
    python trim_sprites.py ../resources/monster/slime/monster_green.png
    python trim_sprites.py ../resources/monster/slime/
    python trim_sprites.py ../resources/monster/
"""
import sys
import os
from PIL import Image

def trim(image_path):
    img = Image.open(image_path).convert("RGBA")
    # 取得非透明區域的邊界
    bbox = img.getbbox()
    if bbox is None:
        print(f"  跳過（全透明）: {image_path}")
        return

    old_size = img.size
    trimmed = img.crop(bbox)
    new_size = trimmed.size

    if old_size == new_size:
        print(f"  不需要裁切: {os.path.basename(image_path)} ({old_size[0]}x{old_size[1]})")
        return

    trimmed.save(image_path)
    print(f"  裁切完成: {os.path.basename(image_path)} {old_size[0]}x{old_size[1]} → {new_size[0]}x{new_size[1]}")

def process(path):
    if os.path.isfile(path) and path.endswith(".png"):
        trim(path)
    elif os.path.isdir(path):
        for root, dirs, files in os.walk(path):
            for f in files:
                if f.endswith(".png"):
                    trim(os.path.join(root, f))
    else:
        print(f"不是 PNG 檔案或資料夾: {path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("用法: python trim_sprites.py <圖片路徑或資料夾>")
        sys.exit(1)

    for arg in sys.argv[1:]:
        print(f"處理: {arg}")
        process(arg)

    print("\n完成！")
