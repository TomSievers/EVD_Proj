"""
script which can be used to determine the parameters for the houghCircles function.
make sure the right image is read
"""

import cv2 as cv
import numpy as np

def nothing(x):
    pass

img = cv.imread('picture_preprocessed.png')
img_to_process = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

cv.namedWindow('trackbars')
cv.createTrackbar('dp', 'trackbars', 1, 100, nothing)
cv.createTrackbar('minDist', 'trackbars', 1, 100, nothing)
cv.createTrackbar('param1', 'trackbars', 1, 300, nothing)
cv.createTrackbar('param2', 'trackbars', 1, 300, nothing)
cv.createTrackbar('minRadius', 'trackbars', 0, 300, nothing)
cv.createTrackbar('maxRadius', 'trackbars', 0, 300, nothing)

while 1:
    dp = cv.getTrackbarPos('dp', 'trackbars') / 10 + 0.9
    minDist = cv.getTrackbarPos('minDist', 'trackbars')
    param1 = cv.getTrackbarPos('param1', 'trackbars')
    param2 = cv.getTrackbarPos('param2', 'trackbars')
    minRadius = cv.getTrackbarPos('minRadius', 'trackbars')
    maxRadius = cv.getTrackbarPos('maxRadius', 'trackbars')
    
    circle_image = img.copy()
    
    circles = cv.HoughCircles(img_to_process, cv.HOUGH_GRADIENT, dp, minDist, param1=param1, param2=param2, minRadius=minRadius, maxRadius=maxRadius)
    if circles is not None:
        circles = np.uint16(np.around(circles))
        print(len(circles[0]))
        for circle in circles[0, :]:
            cv.circle(circle_image, (circle[0], circle[1]), circle[2], (0, 255, 0), 2)
    
    cv.imshow('_image', img)
    cv.imshow('image', circle_image)
    cv.waitKey(1)

