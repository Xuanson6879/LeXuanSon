from cv2 import threshold
import src.data_utils as utils
import cv2
import numpy as np
from imutils import perspective
from src.data_utils import order_points, convert2Square, draw_labels_and_boxes
import cv2
import numpy as np
from skimage import measure
from imutils import perspective
import imutils
from src.char_classification.model import CNN_Model
from skimage.filters import threshold_local
from imutils.contours import sort_contours
from tensorflow import keras
import os
import random
ALPHA_DICT = {0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E', 5: 'F', 6: 'G', 7: 'H', 8: 'K', 9: 'L', 10: 'M', 11: 'N', 12: 'P',
              13: 'R', 14: 'S', 15: 'T', 16: 'U', 17: 'V', 18: 'X', 19: 'Y', 20: 'Z', 21: '0', 22: '1', 23: '2', 24: '3',
              25: '4', 26: '5', 27: '6', 28: '7', 29: '8', 30: '9', 31: "Background"}
charss=["A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"]
numss=["0","1","2","3","4","5","6","7","8","9","."]
path = r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\data\plate"
for plate in os.listdir(path):
    print(path+"\\"+plate)
    boxes = []
    classes_id = []
    confidences = []
    scale = 0.00392
    image = cv2.imread(path+"\\"+plate)
    blob = cv2.dnn.blobFromImage(image, scalefactor=scale, size=(416, 416), mean=(0, 0), swapRB=True, crop=False)
    height, width = image.shape[:2]
    threshold=0.5

    recogChar=model=keras.models.load_model("./src/weights/bestmodel2-9.hdf5")
    LP_DETECTION_CFG = {
        "weight_path": "./src/weights/yolov3-tiny_15000.weights",
        "classes_path": "./src/lp_detection/cfg/yolo.names",
        "config_path": "./src/lp_detection/cfg/yolov3-tiny.cfg"
    }
    # take image to model
    LP_DETECTION_CFG['classes_path']
    model = cv2.dnn.readNet(model=LP_DETECTION_CFG['weight_path'], config=LP_DETECTION_CFG['config_path'])
    model.setInput(blob)
    # run forward
    outputs = model.forward(utils.get_output_layers(model))

    for output in outputs:
        for i in range(len(output)):
            scores = output[i][5:]
            class_id = np.argmax(scores)
            confidence = float(scores[class_id])

            if confidence > threshold:
                # coordinate of bounding boxes
                center_x = int(output[i][0] * width)
                center_y = int(output[i][1] * height)

                detected_width = int(output[i][2] * width)
                detected_height = int(output[i][3] * height)

                x_min = center_x - detected_width / 2
                y_min = center_y - detected_height / 2

                boxes.append([x_min, y_min, detected_width, detected_height])
                classes_id.append(class_id)
                confidences.append(confidence)

    indices = cv2.dnn.NMSBoxes(boxes, confidences, score_threshold=threshold, nms_threshold=0.4)

    coordinates = []
    for i in indices:
        index = i
        x_min, y_min, width, height = boxes[index]
        x_min = round(x_min)
        y_min = round(y_min)

        coordinates.append((x_min, y_min, width, height))
    for coordinate in coordinates:
        candidates=[]
        pts = order_points(coordinate)

                # crop number plate used by bird's eyes view transformation
        LpRegion = perspective.four_point_transform(image, pts)
        gray = cv2.cvtColor(LpRegion,cv2.COLOR_BGR2GRAY)
        # cv2.imshow("gray",gray)
        # cv2.waitKey(0)
        # image = cv2.imread(r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1-2.jpg")
        # gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
                # crop number plate used by bird's eyes view transformation
    # LpRegion = perspective.four_point_transform("C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1.jpg", pts)
        V = cv2.split(cv2.cvtColor(LpRegion, cv2.COLOR_BGR2HSV))[2]

            # adaptive threshold
        T = threshold_local(V, 15, offset=10, method="gaussian")
        thresh = (V > T).astype("uint8") * 255
    # cv2.imshow("thresh",thresh)
    # cv2.waitKey(0)
            # convert black pixel of digits to white pixel
        thresh = cv2.bitwise_not(thresh)
        thresh = imutils.resize(thresh, width=400)
        thresh = cv2.medianBlur(thresh, 5)
        # cv2.imshow("thresh",thresh)
        # cv2.waitKey(0)
            # connected components analysis
        labels = measure.label(thresh, connectivity=2, background=0)
        print(labels.shape)
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
            # sorted_ctrs = sorted(contours, key=lambda contours: cv2.boundingRect(contours)[0] + cv2.boundingRect(contours)[1] * thresh.shape[1] )
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
                    # cv2.imshow("roi",candidate)
                    # cv2.waitKey(0)
                    # square_candidate = convert2Square(candidate)
                    square_candidate = cv2.resize(candidate, (28, 28), cv2.INTER_AREA)
                    square_candidate = square_candidate.reshape((28, 28, 1))
                    cv2.imshow("roi",square_candidate)
                    if(cv2.waitKey(33) == 2490368):
                        cv2.destroyAllWindows()
                    fi=input(" ")
                    if fi in charss:
                        fi=fi.upper()
                        if fi == "A":
                            label = 0
                        elif fi == "B":
                            label = 1
                        elif fi == "C":
                            label = 2
                        elif fi == "D":
                            label = 3
                        elif fi == "E":
                            label = 4
                        elif fi == "F":
                            label = 5
                        elif fi == "G":
                            label = 6
                        elif fi == "H":
                            label = 7
                        elif fi == "K":
                            label = 8
                        elif fi == "L":
                            label = 9
                        elif fi == "M":
                            label = 10
                        elif fi == "N":
                            label = 11
                        elif fi == "P":
                            label = 12
                        elif fi == "R":
                            label = 13
                        elif fi == "S":
                            label = 14
                        elif fi == "T":
                            label = 15
                        elif fi == "U":
                            label = 16
                        elif fi == "V":
                            label = 17
                        elif fi == "X":
                            label = 18
                        elif fi == "Y":
                            label = 19
                        elif fi == "Z":
                            label = 20
                        else:
                            label = -1
                            ValueError("Don't match file")
                        cv2.imwrite(r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\data\categorized\\alphas\\"+str(label)+"\\"+plate+str(random.randint(0, 100))+".jpg",square_candidate)
                    elif fi in numss:
                        if fi == "0":
                            label = 21
                        elif fi == "1":
                            label = 22
                        elif fi == "2":
                            label = 23
                        elif fi == "3":
                            label = 24
                        elif fi == "4":
                            label = 25
                        elif fi == "5":
                            label = 26
                        elif fi == "6":
                            label = 27
                        elif fi == "7":
                            label = 28
                        elif fi == "8":
                            label = 29
                        elif fi == "9":
                            label = 30
                        elif fi == ".":
                            label = 31
                        else:
                            label = -1
                            ValueError("Don't match file")
                        cv2.imwrite(r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\data\categorized\\digits\\"+str(label)+"\\"+plate+str(random.randint(0, 100))+".jpg",square_candidate)
                    
