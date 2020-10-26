import math

import numpy as np
import cv2 as cv

# 1. Read TEST image.
img = cv.imread("image.jpg", cv.IMREAD_COLOR)

# 2. Initialize known information.
#   - Table (corner coordinates).
#   - Cue ball (location, radius).
#   - Cue (2 points on the cue, including the coordinates of the tip).
table = [(163, 117), (1710, 120), (1836, 888), (63, 939)]
cue_ball = (590, 490)
cue_ball_radius = 30
cue_start = (203, 193)  # Random point within cue
cue_tip = (533, 400)

# 3. Get the warp matrix of the table angle and warp the image.
dst_shape = (table[1][0], table[2][1]-table[1][1])

src_pt = np.float32(table[:3])
dst_pt = np.float32([(0,0), (dst_shape[0],0), dst_shape])
warp_matrix = cv.getAffineTransform(src_pt, dst_pt)
img = cv.warpAffine(img, warp_matrix, dst_shape)

# src_pt = np.float32(table)
# dst_pt = np.float32([(0,0), (dst_shape[0],0), dst_shape, (0, dst_shape[1])])
# warp_matrix = cv.getPerspectiveTransform(src_pt, dst_pt)
# img = cv.warpPerspective(img, warp_matrix, dst_shape)

# 4. Correct known coordinates with the warp matrix.


def transform_point(pt, matrix):
    is_list = type(pt) is list
    if not is_list:
        pt = [[pt]]
    else:
        pt = [[p] for p in pt]
    array = np.array(pt)
    transformed = cv.transform(array, matrix)
    squeezed = np.squeeze(np.squeeze(transformed))
    if not is_list:
        return tuple(squeezed[:2])
    return [tuple(x[:2]) for x in squeezed]


# table = transform_point(table, warp_matrix)
table = [(0, 0), (1710, 0), (1710, 768), (0, 768)]
cue_ball = transform_point(cue_ball, warp_matrix)
cue_start = transform_point(cue_start, warp_matrix)
cue_tip = transform_point(cue_tip, warp_matrix)


def update():
    global img, table, cue_tip, cue_start, cue_ball, cue_ball_radius
    shown_image = img.copy()
    # 5. Draw the trajectory
    #   5.1 Get the cue angle based on 2 known points.
    #       - To get the angle in Radian measure use the atan2(y1-y2, x1-x2) function.
    cue_angle = math.atan2(cue_tip[1] - cue_start[1], cue_tip[0] - cue_start[0])

    #   5.2 Check if the cue angle overlaps with the location of the cue ball.
    #       - Check if the distance of the cue ball to the line is not more than the radius of the cue ball.
    #           - Distance is using: abs(ax - by + c) / sqrt(aa + bb)
    #               - a = y2-y1
    #               - b = x2-x1
    #               - c = x2y1 + y2x1
    #               - x, y = point of cue ball center.
    #       - Extra check if the cue ball is not behind the cue tip.

    # http://www.pygame.org/wiki/IntersectingLineDetection
    def line_intersect(line1, line2):
        def gradient(points):
            if points[0][0] == points[1][0]:
                return None
            return (points[0][1] - points[1][1]) / (points[0][0] - points[1][0])

        def y_intersect(p, m):
            return p[1] - (m * p[0])

        m1, m2 = gradient(line1), gradient(line2)
        if m1 == m2:
            return None
        elif m1 is not None and m2 is not None:
            b1 = y_intersect(line1[0], m1)
            b2 = y_intersect(line2[0], m2)
            x = (b2 - b1) / (m1 - m2)
            pt = x, (m1 * x) + b1
        elif m1 is None:
            b2 = y_intersect(line2[0], m2)
            pt = line2[0][0], (m2 * line1[0][0]) + b2
        else:
            b1 = y_intersect(line1[0], m1)
            pt = line2[0][0], (m1 * line2[0][0]) + b1
        return tuple(int(x) for x in pt)

    def line_circle_collision(pt1, pt2, center, circle_radius):
        # Point opposite of circle
        if (pt2[0]-pt1[0] < 0) == (pt2[0]-center[0] < 0) or (pt2[1]-pt1[1]) < 0 == (pt2[1]-center[1]) < 0:
            return False

        a = (pt2[1] - pt1[1])
        b = (pt2[0] - pt1[0])
        c = (pt2[0] * pt1[1]) - (pt2[1] * pt1[0])
        x, y = center
        dist = abs(a * x - b * y + c) / math.sqrt(a * a + b * b)

        if circle_radius >= dist:
            return True
        else:
            return False

    if line_circle_collision(cue_start, cue_tip, cue_ball, cue_ball_radius):
        #   5.3 Get the angle of the cue ball trajectory.
        trj_angle = cue_angle
        start_point = cue_ball

        collisions = 0
        while collisions <= 5:
            #   5.4 Use the angle, center and radius of the cue ball to calculate at which point the line starts.
            #       - The point is: x = (x1 + r + cos(radians)), y = (y1 + r + sin(radians))
            end_point = (int(start_point[0] + 2000 * np.cos(trj_angle)), int(start_point[1] + 2000 * np.sin(trj_angle)))

            #   5.5 Draw the trajectory.
            #       - When the edge of the image is released then continue on a new angle or stop after 5 collision.
            cv.line(shown_image, start_point, end_point, (100, 100, 255), thickness=3)
            line = np.array([start_point, end_point])

            sides = [0, 0]
            if trj_angle < 0:
                sides[0] = 0
            else:
                sides[0] = 2

            if abs(trj_angle) < (math.pi / 2):
                sides[1] = 1
            else:
                sides[1] = 3

            found = False
            for i in sides:
                boundary = np.array([table[i], table[0 if i + 1 > 3 else i + 1]], dtype=float)
                point = line_intersect(line.astype(np.float), boundary)
                if 0 <= point[0] <= img.shape[1] and 0 <= point[1] <= img.shape[0]:
                    start_point = point
                    if i == 1 or i == 3:
                        if trj_angle > 0:
                            trj_angle = math.pi - trj_angle
                        else:
                            trj_angle = -(trj_angle + math.pi)
                    else:
                        trj_angle = -trj_angle

                    if trj_angle > math.pi:
                        trj_angle = math.pi - trj_angle
                    elif trj_angle < -math.pi:
                        trj_angle = math.pi + trj_angle

                    hit_x = int(start_point[0] + 2000 * np.cos(trj_angle))
                    hit_y = int(start_point[1] + 2000 * np.sin(trj_angle))
                    end_point = (hit_x, hit_y)
                    cv.circle(shown_image, start_point, 10, (0, 0, 255), thickness=3)
                    cv.line(shown_image, start_point, end_point, (0, 0, 255), thickness=3)
                    found = True
                    break

            if not found:
                break
            collisions += 1

    # DEBUG OPTIONS:
    #   - Draw circles where the points are on the cue
    cv.line(shown_image, cue_tip, cue_start, (255, 255, 0), thickness=6)
    cv.circle(shown_image, cue_tip, 6, (0, 0, 0), thickness=-1)
    cv.circle(shown_image, cue_start, 6, (0, 0, 0), thickness=-1)
    cv.circle(shown_image, cue_ball, 6, (0, 255, 0), thickness=-1)

    #   - Let the cue be determined by mouse positions.
    #       - Clicking outputs the coordinates of the mouse.

    def mouse_event(event, x, y, flags, param):
        global cue_start, cue_tip
        if event == cv.EVENT_LBUTTONDOWN:
            cue_start = (x, y)
        if event == cv.EVENT_RBUTTONDOWN:
            cue_tip = (x, y)
            update()

    cv.namedWindow("img", cv.WINDOW_NORMAL)
    cv.setMouseCallback("img", mouse_event)
    cv.imshow("img", shown_image)
    cv.waitKey(0)


update()
