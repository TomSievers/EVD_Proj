from cue_ball_preprocessing import preprocess_image
from cue_ball_segmentation import segment_image
from cue_ball_feature_extraction import extract_features_image
from cue_ball_classification import classify_image
import cv2 as cv
import numpy as np

# background color
background_color_hsv_min = (101, 160, 104)
background_color_hsv_max = (104, 210, 161)

# white ball color
white_ball_color_hsv_min = (37, 40, 175)
white_ball_color_hsv_max = (53, 81, 243)

# picture scale
scale_x = 0.4
scale_y = 0.3

# table positions
left_top_corner = (345, 67)
right_top_corner = (1688, 123)
right_bottom_corner = (1662, 783)
left_bottom_corner = (360, 757)

img = cv.imread("picture.png")
img = cv.resize(img, (0, 0), fx=scale_x, fy=scale_y)

height, width, channels = img.shape 
points_old = np.float32([[left_top_corner[0] * scale_x, left_top_corner[1] * scale_y], [right_top_corner[0] * scale_x, right_top_corner[1] * scale_y], [right_bottom_corner[0] * scale_x, right_bottom_corner[1] * scale_y], [left_bottom_corner[0] * scale_x, left_bottom_corner[1] * scale_y]])
points_warped = np.float32([[0, 0],[width, 0],[width, height],[0, height]])

# execute vision steps
images_preprocessed = preprocess_image(img, width, height, points_old, points_warped, background_color_hsv_min, background_color_hsv_max)
image_segmented = segment_image(images_preprocessed)
image_feature_extraction = extract_features_image(images_preprocessed, image_segmented, white_ball_color_hsv_min, white_ball_color_hsv_max)
image_classified = classify_image(image_feature_extraction[0], image_feature_extraction[1])

warped_image = cv.cvtColor(images_preprocessed[0], cv.COLOR_HSV2BGR)

cv.imwrite("picture_warped.png", warped_image)
cv.imwrite("picture_preprocessed.png", images_preprocessed[1])

cv.imshow("warped image", warped_image)
cv.imshow("image pre_processed", images_preprocessed[1])
cv.imshow("image_segmented", image_segmented[0])
cv.imshow("image_feature_extraction", image_feature_extraction[0])
cv.imshow("image_classified", image_classified)
cv.waitKey(0)