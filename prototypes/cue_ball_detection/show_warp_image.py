"""
Script used to load images to check warp positions
"""

import cv2 as cv
import numpy as np

scale_x = 0.4
scale_y = 0.3

left_top_corner = (712, 240)
right_top_corner = (1975, 267)
right_bottom_corner = (1958, 907)
left_bottom_corner = (705, 873)

img = cv.imread("used_pictures/setup5_3.jpg")

cv.imshow("img_not_resized", img)

img = cv.resize(img, (0, 0), fx=0.4, fy=0.3)

height, width, channels = img.shape 
points_old = np.float32([[left_top_corner[0] * scale_x, left_top_corner[1] * scale_y], [right_top_corner[0] * scale_x, right_top_corner[1] * scale_y], [right_bottom_corner[0] * scale_x, right_bottom_corner[1] * scale_y], [left_bottom_corner[0] * scale_x, left_bottom_corner[1] * scale_y]])
points_warped = np.float32([[0, 0],[width, 0],[width, height],[0, height]])

matrix = cv.getPerspectiveTransform(points_old, points_warped)
img_warped = cv.warpPerspective(img, matrix, (width, height))

cv.imshow("img", img)
cv.imshow("img_warped", img_warped)
cv.waitKey(0)