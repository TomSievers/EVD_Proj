import cv2 as cv
import numpy as np
import math

# function which finds the circles in the image
def segment_image(preprocessed_images): 
    circle_image = cv.cvtColor(preprocessed_images[1], cv.COLOR_GRAY2BGR)
    circles = cv.HoughCircles(preprocessed_images[1], cv.HOUGH_GRADIENT, 3.2, 1, param1=50, param2=39, minRadius=0, maxRadius=20)
    
    if circles is not None:
        circles = np.int64(np.around(circles))

        iters = []
        
        for circle in range(len(circles[0, :])):
            for circle_to_compare in range(len(circles[0, :])):
                # determine which circles should be removed
                if circle != circle_to_compare:
                    distance_between_center_points = math.sqrt(math.pow(circles[0][circle][0] - circles[0][circle_to_compare][0], 2) + math.pow(circles[0][circle][1] - circles[0][circle_to_compare][1], 2))
                    if distance_between_center_points < circles[0][circle][2] and circle not in iters and circle_to_compare not in iters: 
                        iters.append(circle)

        # remove the circles which overlap
        circles = np.delete(circles, iters, axis=1)

        for circle in circles[0, :]:
            cv.circle(circle_image, (circle[0], circle[1]), circle[2], (0, 255, 0), 2)

    return circle_image, circles