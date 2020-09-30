import cv2 as cv
import numpy as np

background_color_hsv_min = (77, 216, 180)
background_color_hsv_max = (113, 255, 255)

img = cv.imread("image.jpg")

scale_x = 0.3
scale_y = 0.3
img = cv.resize(img, (0, 0), fx=scale_x, fy=scale_y)

# warp the table
height, width, channels = img.shape 
points_old = np.float32([[160 * scale_x, 118 * scale_y],[1706 * scale_x, 118 * scale_y],[1824 * scale_x, 904 * scale_y],[70 * scale_x, 929 * scale_y]])
point_warped = np.float32([[0, 0],[width, 0],[width, height],[0, height]])
matrix = cv.getPerspectiveTransform(points_old, point_warped)
img_warped = cv.warpPerspective(img, matrix, (width, height))

img_noise_removed = cv.fastNlMeansDenoisingColored(img_warped,None,10,10,7,21)
img_hsv = cv.cvtColor(img_noise_removed, cv.COLOR_BGR2HSV)

# remove the table background
img_background = cv.inRange(img_hsv, background_color_hsv_min, background_color_hsv_max)
img_background_reversed = cv.bitwise_not(img_background)

cv.imshow("table warped", img_warped)
cv.imshow("table with balls", img_background_reversed)
cv.waitKey(0)