from cue_ball_preprocessing import preprocess_image
from cue_ball_segmentation import segment_image
from cue_ball_feature_extraction import extract_features_image
from cue_ball_classification import classify_image
from config import pictures
import cv2 as cv
import numpy as np
import time

# picture scale
scale_x = 0.4
scale_y = 0.3

for key, value in pictures.items():
    img = cv.imread(value['location'])
    img = cv.resize(img, (0, 0), fx=scale_x, fy=scale_y)

    height, width, channels = img.shape 
    points_old = np.float32([[value['left_top_corner_table'][0] * scale_x, value['left_top_corner_table'][1] * scale_y], [value['right_top_corner_table'][0] * scale_x, value["right_top_corner_table"][1] * scale_y], [value["right_bottom_corner_table"][0] * scale_x, value["right_bottom_corner_table"][1] * scale_y], [value["left_bottom_corner_table"][0] * scale_x, value["left_bottom_corner_table"][1] * scale_y]])
    points_warped = np.float32([[0, 0],[width, 0],[width, height],[0, height]])

    # start the timer
    start_time = time.time()

    # execute vision steps
    images_preprocessed = preprocess_image(img, width, height, points_old, points_warped, value["background_color_hsv_min"], value["background_color_hsv_max"])
    images_preprocessed_end_time = time.time()

    image_segmented = segment_image(images_preprocessed)
    image_segmented_end_time = time.time()

    image_feature_extraction = extract_features_image(images_preprocessed, image_segmented, value["white_ball_color_hsv_min"], value["white_ball_color_hsv_max"])
    image_feature_extraction_end_time = time.time()

    image_classified = classify_image(image_feature_extraction[0], image_feature_extraction[1])

    # stop the timer
    stop_time = time.time()

    # PLEASE note to disable the visualiation option of the white spots in cue_ball_feature_extraction, which takes a lot of time
    print("needed time preprocessing: ", images_preprocessed_end_time - start_time)
    print("needed time segmentation: ", image_segmented_end_time - images_preprocessed_end_time)
    print("needed time feature extraction: ", image_feature_extraction_end_time - image_segmented_end_time)
    print("needed time classification: ", stop_time - image_feature_extraction_end_time)
    print("total time needed: ", stop_time - start_time)

    warped_image = cv.cvtColor(images_preprocessed[0], cv.COLOR_HSV2BGR)

    cv.imwrite("picture_warped.png", warped_image)
    cv.imwrite("picture_preprocessed.png", images_preprocessed[1])

    cv.imshow("image", img)
    cv.imshow("warped image", warped_image)
    cv.imshow("image pre_processed", images_preprocessed[1])
    cv.imshow("image_segmented", image_segmented[0])
    cv.imshow("image_feature_extraction", image_feature_extraction[0])
    cv.imshow("image_classified", image_classified)
    cv.waitKey(0)