import cv2 as cv

# this function warps the image
def warp_image(image, width, height, old_corner_points, new_corner_points):
    matrix = cv.getPerspectiveTransform(old_corner_points, new_corner_points)
    img_warped = cv.warpPerspective(image, matrix, (width, height))
    return img_warped

# this function emoves the background of the table
def remove_background(image, min_background_colors_hsv, max_background_colors_hsv):
    img_background = cv.inRange(image, min_background_colors_hsv, max_background_colors_hsv)
    img_background_reversed = cv.bitwise_not(img_background)
    return img_background_reversed

# function which can be used to get a preprocessed image
# returns a warped image without noise and a image which has the background removed.
def preprocess_image(image, width, height, old_corner_points, new_corner_points, min_background_colors_hsv, max_background_colors_hsv):
    warped_image = warp_image(image, width, height, old_corner_points, new_corner_points)
    img_noise_removed = cv.fastNlMeansDenoisingColored(warped_image, None, 10, 10, 7, 21)
    img_hsv = cv.cvtColor(img_noise_removed, cv.COLOR_BGR2HSV)
    
    img_background_removed = remove_background(img_hsv, min_background_colors_hsv, max_background_colors_hsv)
    
    kernel = cv.getStructuringElement(cv.MORPH_RECT, (3, 3))
    img_balls_opened = cv.morphologyEx(img_background_removed, cv.MORPH_OPEN, kernel)
    
    return img_hsv, img_balls_opened