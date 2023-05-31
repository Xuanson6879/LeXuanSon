import cv2
import numpy as np
from skimage import measure
from imutils import perspective
import imutils
from skimage.filters import threshold_local

image = cv2.imread(r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1-2.jpg")
gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
            # crop number plate used by bird's eyes view transformation
# LpRegion = perspective.four_point_transform("C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1.jpg", pts)
V = cv2.split(cv2.cvtColor(image, cv2.COLOR_BGR2HSV))[2]

        # adaptive threshold
T = threshold_local(V, 15, offset=10, method="gaussian")
thresh = (V > T).astype("uint8") * 255
# cv2.imshow("thresh",thresh)
# cv2.waitKey(0)
        # convert black pixel of digits to white pixel
thresh = cv2.bitwise_not(thresh)
thresh = imutils.resize(thresh, width=400)
thresh = cv2.medianBlur(thresh, 5)
cv2.imshow("thresh",thresh)
cv2.waitKey(0)
        # connected components analysis
labels = measure.label(thresh, connectivity=2, background=0)

        # loop over the unique components
for label in np.unique(labels):
            # if this is background label, ignore it
    if label == 0:
        continue

            # init mask to store the location of the character candidates
    mask = np.zeros(thresh.shape, dtype="uint8")
    mask[labels == label] = 255

            # find contours from mask
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if len(contours) > 0:
        contour = max(contours, key=cv2.contourArea)
        (x, y, w, h) = cv2.boundingRect(contour)
        candidate = np.array(mask[y:y + h, x:x + w])
        # cv2.imshow("roi",roi)
        # cv2.waitKey(0)
                # rule to determine characters
        aspectRatio = w / float(h)
        solidity = cv2.contourArea(contour) / float(w * h)
        heightRatio = h / float(gray.shape[0])

        if 0.1 < aspectRatio < 1.0 and solidity > 0.1 and 0.35 < heightRatio < 2.0:
                    # extract characters
            candidate = np.array(mask[y:y + h, x:x + w])
            cv2.imshow("roi",candidate)
            cv2.waitKey(0)
        #     square_candidate = convert2Square(candidate)
        #     square_candidate = cv2.resize(square_candidate, (28, 28), cv2.INTER_AREA)
        #     square_candidate = square_candidate.reshape((28, 28, 1))
        #     self.candidates.append((square_candidate, (y, x)))