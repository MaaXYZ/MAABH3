import cv2
import os
from typing import List, Tuple

print(
    "Usage:\n"
    "Put the 16:9 images under ./src, and run this script, it will be auto converted to 720p.\n"
    "Drag mouse to select ROI, press 'S' to save, press 'Q' to quit.\n"
    "The cropped images will be saved in ./dst\n"
)

# 初始化参考点列表和布尔值标志：是否正在执行裁剪
refPt: List[Tuple[int, int]] = []
cropping = False


# 点击并裁剪ROI区域
# -events 鼠标事件（如按下鼠标左键，释放鼠标左键，鼠标移动等）
# -x x坐标
# -y y坐标
# -flages params 其他参数
def click_and_crop(event: int, x: int, y: int, *args) -> None:
    # 获取全局变量的引用

    global refPt, cropping

    # 如果鼠标左被单击，记录（x,y）坐标并显示裁剪正在进行
    if event == cv2.EVENT_LBUTTONDOWN:
        refPt = [(x, y)]
        cropping = True
    # 检测鼠标是否移动
    elif event == cv2.EVENT_MOUSEMOVE:
        if cropping:
            # 创建图像副本以绘制动态矩形
            draw = image.copy()
            cv2.rectangle(draw, refPt[0], (x, y), (0, 255, 0), 2)
            cv2.imshow("image", draw)
    # 检测鼠标左键是否释放
    elif event == cv2.EVENT_LBUTTONUP:
        # 记录结束（x,y）坐标，并显示裁剪结束
        refPt.append((x, y))
        cropping = False

        draw = image.copy()
        cv2.rectangle(draw, refPt[0], refPt[1], (0, 255, 0), 2)
        cv2.imshow("image", draw)


std_width: int = 1280
std_height: int = 720
std_ratio = std_width / std_height

cv2.namedWindow("image")
cv2.setMouseCallback("image", click_and_crop)

for filename in os.listdir("./src"):
    if not (filename.endswith(".png") or filename.endswith(".jpg")):
        continue

    print("src:", filename)
    image = cv2.imread(f"./src/{filename}")

    cur_ratio = image.shape[1] / image.shape[0]

    if cur_ratio >= std_ratio:
        dsize_width = int(cur_ratio * std_height)
        dsize_height = std_height
    else:
        dsize_width = std_width
        dsize_height = int(std_width / cur_ratio)

    dsize = (dsize_width, dsize_height)
    image = cv2.resize(image, dsize, interpolation=cv2.INTER_AREA)

    while True:
        cv2.imshow("image", image)
        key = cv2.waitKey(0) & 0xFF
        if key == ord("s"):
            break
        elif key == ord("q"):
            exit()

    # 如果参考点列表里有俩个点，则裁剪区域并展示
    if len(refPt) == 2:
        pt1, pt2 = sorted(refPt)

        left, top = pt1
        right, bottom = pt2

        roi = image[top:bottom, left:right]

        horizontal_expansion = 100
        vertical_expansion = 100

        # 计算扩展后的左上角坐标，并确保不超出图像边界
        filename_x = max(0, left - horizontal_expansion // 2)
        filename_y = max(0, top - vertical_expansion // 2)

        # 计算扩展后的宽度和高度，并进行边界检查
        filename_w = min(
            dsize_width - filename_x, (right - left) + horizontal_expansion
        )
        filename_h = min(dsize_height - filename_y, (bottom - top) + vertical_expansion)

        dst_filename: str = (
            f"{filename}_{filename_x},{filename_y},{filename_w},{filename_h}.png"
        )
        print("dst:", dst_filename)

        print(
            f"original roi: {left}, {top}, {right - left}, {bottom - top}, \n"
            f"amplified roi: {filename_x}, {filename_y}, {filename_w}, {filename_h}\n\n"
        )

        cv2.imwrite(f"./dst/{dst_filename}", roi)

    refPt: List[Tuple[int, int]] = []
    cropping = False

# 关闭所有打开的窗口
cv2.destroyAllWindows()
