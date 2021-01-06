import cv2 as cv

cap = cv.VideoCapture('test.h264')

frame_width = 1640
frame_height = 1232
out = cv.VideoWriter('empty_table.avi', cv.VideoWriter_fourcc('M','J','P','G'), 60, (frame_width, frame_height))

while(cap.isOpened()):

    ret, img = cap.read()

    if not ret:
        break

    img_changed = img.copy()
    x = 812
    y = 406

    x_start_transfer = 602
    x_end_transfer = 771
    y_start_transfer = 384
    y_end_transfer = 504

    for x_pos in range(x_start_transfer, x_end_transfer):
        current_y = y
        for y_pos in range(y_start_transfer, y_end_transfer):
            for rgb in range(0, 3):
                img_changed[current_y][x][rgb] = img[y_pos][x_pos][rgb]
            current_y += 1
        x += 1

    out.write(img_changed)