import cv2 as cv
import numpy as np

img = cv.imread("setup_picture_warped.png")
#img = cv.resize(img, (0, 0), fx=0.3, fy=0.3)

def nothing(x):
    pass

cv.namedWindow('image')
cv.createTrackbar('Hmin', 'image', 0, 180, nothing)
cv.createTrackbar('Smin', 'image', 0, 255, nothing)
cv.createTrackbar('Vmin', 'image', 0, 255, nothing)
cv.createTrackbar('Hmax', 'image', 0, 180, nothing)
cv.createTrackbar('Smax', 'image', 0, 255, nothing)
cv.createTrackbar('Vmax', 'image', 0, 255, nothing)

#img_noise_removed = cv.fastNlMeansDenoisingColored(img,None,10,10,7,21)
img_HSV = cv.cvtColor(img, cv.COLOR_BGR2HSV)

while(1):
    
    # get slider positions
    h_min = cv.getTrackbarPos('Hmin', 'image')
    s_min = cv.getTrackbarPos('Smin', 'image')
    v_min = cv.getTrackbarPos('Vmin', 'image')
    h_max = cv.getTrackbarPos('Hmax', 'image')
    s_max = cv.getTrackbarPos('Smax', 'image')
    v_max = cv.getTrackbarPos('Vmax', 'image')

    img_cue_ball = cv.inRange(img_HSV, (h_min, s_min, v_min), (h_max, s_max, v_max))

    # show img
    cv.imshow('original_image', img)

    #show in range
    cv.imshow('image in range', img_cue_ball)

    # show seperate channels hsv
    h, s, v = cv.split(img_HSV)
    cv.imshow('img_cue_ball_hue', h)
    cv.imshow('img_cue_ball_saturation', s)
    cv.imshow('img_cue_ball_value', v)
    cv.waitKey(1)

cv.destroyAllWindows()