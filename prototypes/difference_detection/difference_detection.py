import cv2 as cv
import numpy as np


def extract(frame):
    frame = frame[340:610, 550:1180]
    cv.cvtColor(frame, cv.COLOR_RGB2HSV, dst=frame)
    return cv.inRange(frame, (20, 0, 0), (255, 255, 255))

capture = cv.VideoCapture("../../Photos_pool_table/play.h264")

(success, frame) = capture.read()
previous = extract(frame)


while success:
    (success, frame) = capture.read()
    if frame is not None:
        frame = extract(frame)
        diff = cv.subtract(frame, previous)
        cv.erode(diff, cv.getStructuringElement(cv.MORPH_CROSS, (3,3)), dst=diff, iterations=3)
        cv.imshow("Difference", diff)
        previous = frame
        if cv.waitKey(10) == 27:
            break
