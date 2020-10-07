from cue_ball_preprocessing import preprocess_image
from cue_ball_segmentation import segment_image
import cv2 as cv
import numpy as np

# background color
background_color_hsv_min = (77, 216, 180)
background_color_hsv_max = (113, 255, 255)

# white ball color
white_ball_color_hsv_min = (83, 0, 222)
white_ball_color_hsv_max = (155, 103, 255)

# picture scale
scale_x = 0.4
scale_y = 0.3

img = cv.imread("image.jpg")
img = cv.resize(img, (0, 0), fx=scale_x, fy=scale_y)

height, width, channels = img.shape 
points_old = np.float32([[160 * scale_x, 118 * scale_y],[1706 * scale_x, 118 * scale_y],[1824 * scale_x, 904 * scale_y],[70 * scale_x, 929 * scale_y]])
points_warped = np.float32([[0, 0],[width, 0],[width, height],[0, height]])

images_preprocessed = preprocess_image(img, width, height, points_old, points_warped, background_color_hsv_min, background_color_hsv_max)
image_segmented = segment_image(images_preprocessed)

warped_image = cv.cvtColor(images_preprocessed[0], cv.COLOR_HSV2BGR)
cv.imshow("warped image", warped_image)
cv.imshow("image pre_processed", images_preprocessed[1])
cv.imshow("image_segmented", image_segmented[0])
cv.waitKey(0)