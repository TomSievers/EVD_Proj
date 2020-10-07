import cv2 as cv
import numpy as np

# function which finds the circles in the image
def segment_image(preprocessed_images): 
    circle_image = cv.cvtColor(preprocessed_images[1], cv.COLOR_GRAY2BGR)
    circles = cv.HoughCircles(preprocessed_images[1], cv.HOUGH_GRADIENT, 1, 10, param1=50, param2=16, minRadius=0, maxRadius=0)
    circles = np.uint16(np.around(circles))

    for circle in circles[0, :]:
        print(circle[0], circle[1], circle[2])
        cv.circle(circle_image, (circle[0], circle[1]), circle[2], (0, 255, 0), 2)

    return circle_image, circles