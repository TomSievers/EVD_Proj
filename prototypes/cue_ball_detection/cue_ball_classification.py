import cv2 as cv

# this function can be used to classify images
def classify_image(image, balls):
    image_features_extracted = image.copy()
    if balls:
        max_value = 0
        index_white_ball = 0
        for index, value in enumerate(balls):
            if value[0] > max_value:
                index_white_ball = index
                max_value = value[2]
        
        print("drawing", index)
        image = cv.circle(image_features_extracted, balls[index_white_ball][1], balls[index_white_ball][2], (0, 0, 255), thickness=-1)
    return image_features_extracted
    


