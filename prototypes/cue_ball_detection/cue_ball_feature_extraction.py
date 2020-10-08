import cv2 as cv
import math

# function which can be used to find the area of the contours
def find_contours_area(contours):
    contour_area = []
    for contour in contours:
        contour_area.append(cv.contourArea(contour))

    return contour_area

# function which can be used to find the center points of the contours
def find_contours_center_points(contours):
    center_points = []
    for contour in contours:
        moment = cv.moments(contour)
        if moment["m00"] != 0:
            center_x = int(moment["m10"] / moment["m00"])
            center_y = int(moment["m01"] / moment["m00"])
        else:
            center_x = 0
            center_y = 0
        center_points.append((center_x, center_y))
    return center_points

# this function can be used to extract features from images
def extract_features_image(preprocessed_images, segmentation_image, white_ball_color_min_hsv, white_ball_color_max_hsv): 
    white_parts_image = cv.inRange(preprocessed_images[0], white_ball_color_min_hsv, white_ball_color_max_hsv)
    converted_image = cv.cvtColor(white_parts_image, cv.COLOR_GRAY2BGR)
    
    height, width, channels = converted_image.shape
    for i in range(height):
        for j in range(width):
            if converted_image[i][j][0] == 255 or converted_image[i][j][1] == 255 or converted_image[i][j][2] == 255:
                converted_image[i][j][0] = 0
                converted_image[i][j][1] = 0
                converted_image[i][j][2] = 255

    # get a picture with both the segments (balls) and the while ball for visualization
    image_features_extracted = cv.bitwise_xor(segmentation_image[0], converted_image)

    
    # determine by what percentage the balls are white
    white_image_contours, white_image_hierarchy = cv.findContours(white_parts_image, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    white_image_contour_area = find_contours_area(white_image_contours)
    white_image_center_points = find_contours_center_points(white_image_contours)
    
    balls_image_contours, balls_image_hierarchy = cv.findContours(preprocessed_images[1], cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
    balls_image_contour_area = find_contours_area(balls_image_contours)
    balls_image_center_points = find_contours_center_points(balls_image_contours)

    # list with percentage of white part of ball + center point + radius
    balls = []

    for ball in segmentation_image[1][0, :]:
        for center_point_index in range(len(balls_image_center_points)):
            distance_between_center_points = math.sqrt(math.pow(ball[0] - balls_image_center_points[center_point_index][0], 2) + math.pow(ball[1] - balls_image_center_points[center_point_index][1], 2))
            
            # found the right ball
            if distance_between_center_points < ball[2]:
                for white_center_point_index in range(len(white_image_center_points)):
                    distance_between_white_center_points = math.sqrt(math.pow(ball[0] - white_image_center_points[white_center_point_index][0], 2) + math.pow(ball[1] - white_image_center_points[white_center_point_index][1], 2))

                    # found a matching white piece
                    if distance_between_white_center_points < ball[2]:
                        area = white_image_contour_area[white_center_point_index] / balls_image_contour_area[center_point_index] * 100
                        balls.append((area, (ball[0], ball[1]), ball[2]))

                
    return image_features_extracted, balls