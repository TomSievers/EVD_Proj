import cv2 as cv
import numpy as np

cap = cv.VideoCapture('empty_table.h264')

while(cap.isOpened()):

    ret, img = cap.read()
    img = cv.resize(img, None, fx=0.5, fy=0.5)
    blur = cv.GaussianBlur(img,(5,5),0)

    img_hsv = cv.cvtColor(blur, cv.COLOR_BGR2HSV)
    h,s,v = cv.split(img_hsv)
    
    h_shifted = np.zeros(h.shape, h.dtype)
    shift = 25

    LUT = []

    for i in range(180):
        LUT.append((i + shift) % 180)

    LUT = np.array(LUT, dtype=np.uint8)
    h_shifted = LUT[h]

    ret, threshold = cv.threshold(h_shifted, 0, 255, cv.THRESH_BINARY+cv.THRESH_OTSU)
    
    closed = cv.morphologyEx(threshold, cv.MORPH_CLOSE, (3, 3), iterations=3)

    contours, hierarchy = cv.findContours(closed, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    #debug
    #for contour in range(len(contours)):
       #cv.drawContours(img, contours, contour, (0, 255, 0))

    max_contour = 0
    max_position = 0

    contour_area = []

    for contour in contours:
        contour_area.append(cv.contourArea(contour))
    
    for contour in range(len(contour_area)):
        if contour_area[contour] > max_contour:
            max_position = contour
            max_contour = contour_area[contour]

    #debug
    cv.drawContours(img, contours, max_position, (0, 255, 0))

    M = cv.moments(contours[max_position])
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])

    cv.circle(img, (cX, cY), 4, (0, 255, 0), -1)

    height, width, channels = img.shape
    minX = width
    maxX = 0
    minY = height
    maxY = 0

    for contour in contours[max_position]:
        if contour[0][0] > maxX:
            maxX = contour[0][0]
        if contour[0][0] < minX:
            minX = contour[0][0]
        if contour[0][1] > maxY:
            maxY = contour[0][1]
        if contour[0][1] < minY:
            minY = contour[0][1]

    cv.circle(img, (minX, minY), 4, (0, 255, 0), -1)
    cv.circle(img, (maxX, maxY), 4, (0, 255, 0), -1)

    # determine min and max values around center point
    valueXmin = (minX + cX) / 2.0
    valueXmax = (maxX + cX) / 2.0
    valueYmin = (minY + cY) / 2.0
    valueYmax = (maxY + cY) / 2.0

    print(valueXmin, valueXmax, valueYmin, valueYmax)

    hsvMin = np.array([255, 255, 255])
    hsvMax = np.array([0, 0, 0])

    for i in range(int(valueYmin), int(valueYmax)):
        for k in range(int(valueXmin), int(valueXmax)):
            for x in range(0, 3):
                if img_hsv[i][k][x] < hsvMin[x]:
                    hsvMin[x] = img_hsv[i][k][x]
                if img_hsv[i][k][x] > hsvMax[x]:
                    hsvMax[x] = img_hsv[i][k][x]

    for i in range(len(hsvMin)):
        hsvMin[i] -= 8
        hsvMax[i] += 8

    print(hsvMin, hsvMax)

    img_ranged = cv.inRange(img_hsv, hsvMin, hsvMax)
    
    
    cv.imshow("img", img)
    cv.imshow("image_h", h)
    cv.imshow("image_h_shifted", h_shifted)
    cv.imshow("image_s", s)
    cv.imshow("image_v", v)
    cv.imshow("window", threshold)
    cv.imshow("img_ranged", img_ranged)
    cv.waitKey(0)

    
    
    
    
    
    
    
    
    
    
    
    
    '''
    #slow loop
    h_shifted = h.copy()
    shift = 25
    for i in range(len(h_shifted)):
        for y in range(len(h_shifted[i])):
            h_shifted[i][y] = (h_shifted[i][y] + shift) % 180
    '''
    '''
    dilated = cv.dilate(img_canny, (3,3), iterations=1)

    contours, hierarchy = cv.findContours(dilated, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

    for contour in contours:
        cv.fillPoly(dilated, pts=contour, color=255)

    hull = []

    for contour in contours:
        hull.append(cv.convexHull(contour, False))

    #for contour in range(len(contours)):
       #cv.drawContours(img, contours, contour, (0, 255, 0))

    contour_area = []

    for contour in hull:
        contour_area.append(cv.contourArea(contour))

    max_contour = 0
    max_position = 0

    for contour in range(len(contour_area)):
        if contour_area[contour] > max_contour:
            max_position = contour
            max_contour = contour_area[contour]

    cv.drawContours(img, contours, max_position, (0, 255, 0))

    M = cv.moments(hull[max_position])
    cX = int(M["m10"] / M["m00"])
    cY = int(M["m01"] / M["m00"])

    #cv.circle(img, (cX, cY), 4, (0, 255, 0), -1)

    height, width, channels = img.shape
    minX = width
    maxX = 0
    minY = height
    maxY = 0

    for contour in hull[max_position]:
        if contour[0][0] > maxX:
            maxX = contour[0][0]
        if contour[0][0] < minX:
            minX = contour[0][0]
        if contour[0][1] > maxY:
            maxY = contour[0][1]
        if contour[0][1] < minY:
            minY = contour[0][1]

    #cv.circle(img, (minX, minY), 4, (0, 255, 0), -1)
    #cv.circle(img, (maxX, maxY), 4, (0, 255, 0), -1)

    # determine min and max values around center point
    valueXmin = (minX + cX) / 2.0
    valueXmax = (maxX + cX) / 2.0
    valueYmin = (minY + cY) / 2.0
    valueYmax = (maxY + cY) / 2.0

    print(valueXmin, valueXmax, valueYmin, valueYmax)

    hsvMin = np.array([255, 255, 255])
    hsvMax = np.array([0, 0, 0])

    blur = cv.GaussianBlur(img, (3,3), 0)
    img_hsv = cv.cvtColor(blur, cv.COLOR_BGR2HSV)

    for i in range(int(valueYmin), int(valueYmax)):
        for k in range(int(valueXmin), int(valueXmax)):
            for x in range(0, 3):
                if img_hsv[i][k][x] < hsvMin[x]:
                    hsvMin[x] = img_hsv[i][k][x]
                if img_hsv[i][k][x] > hsvMax[x]:
                    hsvMax[x] = img_hsv[i][k][x]

    for i in range(len(hsvMin)):
        hsvMin[i] -= 8
        hsvMax[i] += 8

    print(hsvMin, hsvMax)

    img_ranged = cv.inRange(img_hsv, hsvMin, hsvMax)

    h, s, v = cv.split(img_hsv)
    cv.imshow("image_canny", img_canny)
    cv.imshow("image_ranged", img_ranged)
    cv.imshow("eroded", dilated)
    cv.imshow("image_h", h)
    cv.imshow("image_s", s)
    cv.imshow("image_v", v)
    cv.imshow("image", img_hsv)
    cv.imshow("image_contours", img)
    cv.waitKey(0)

    '''