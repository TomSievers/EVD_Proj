import cv2 as cv


# this function can be used to extract features from images
# TODO AANPASSEN NAAR FEATURES IPV SEGMENTATION
def extract_features_image(preprocessed_images, white_ball_color_min_hsv, white_ball_color_max_hsv): 
    white_parts_image = cv.inRange(preprocessed_images[0], white_ball_color_min_hsv, white_ball_color_max_hsv)
    converted_image = cv.cvtColor(white_parts_image, cv.COLOR_GRAY2BGR)
    
    height, width, channels = converted_image.shape
    for i in range(height):
        for j in range(width):
            if converted_image[i][j][0] == 255 or converted_image[i][j][1] == 255 or converted_image[i][j][2] == 255:
                converted_image[i][j][0] = 0
                converted_image[i][j][1] = 0
                converted_image[i][j][2] = 255
    
    preprocessed_images_rgb = cv.cvtColor(preprocessed_images[1], cv.COLOR_GRAY2BGR)
    segmented_image = cv.bitwise_xor(preprocessed_images_rgb, converted_image)
            
    return segmented_image