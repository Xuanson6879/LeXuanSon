from cv2 import threshold
import src.char_classification.data_utils as utils
import cv2
import numpy as np
from imutils import perspective
from src.char_classification.data_utils import order_points, convert2Square, draw_labels_and_boxes
import cv2
import numpy as np
from skimage import measure
from imutils import perspective
import imutils
from src.char_classification.model import CNN_Model
from skimage.filters import threshold_local
from imutils.contours import sort_contours
from tensorflow import keras

ALPHA_DICT = {0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E', 5: 'F', 6: 'G', 7: 'H', 8: 'K', 9: 'L', 10: 'M', 11: 'N', 12: 'P',
              13: 'R', 14: 'S', 15: 'T', 16: 'U', 17: 'V', 18: 'X', 19: 'Y', 20: 'Z', 21: '0', 22: '1', 23: '2', 24: '3',
              25: '4', 26: '5', 27: '6', 28: '7', 29: '8', 30: '9', 31: "Background"}
# ALPHA_DICT = {0:'a',1:'b',2:'c',3:'d',4:'e',5:'f',6:'g',7:'h',8:'i',9:'j',10:'k',11:'l',12:'m',13:'n',14:'o',15:'p',16:'q',
# 17:'r',18:'s',19:'t',20:'u',21:'v',22:'w',23:'x',24:'y',25:'z',26:'A',27:'B',28:'C',29:'D',30:'E',31:'F',32:'G',33:'H',34:'I',
# 35:'J',36:'K',37:'L',38:'M',39:'N',40:'O',41:'P',42:'Q',43:'R',44:'S',45:'T',46:'U',47:'V',48:'W',49:'X',50:'Y',51:'Z',52:'1',
# 53:'2',54:'3',55:'4',56:'5',57:'6',58:'7',59:'8',60:'9',61:'0'}
# path = "./data/categorized/plate/"
# for plate in os.listdir(path):
boxes = []
classes_id = []
confidences = []
scale = 0.00392
CHAR_CLASSIFICATION_WEIGHTS = './src/weights/weight.h5'
image = cv2.imread(r"D:\Download\License-Plate-Recognition-master-1\License-Plate-Recognition-master\data\plate\99.jpg")
blob = cv2.dnn.blobFromImage(image, scalefactor=scale, size=(416, 416), mean=(0, 0), swapRB=True, crop=False)
height, width = image.shape[:2]
threshold=0.5
recogChar = CNN_Model(trainable=False).model
recogChar.load_weights(CHAR_CLASSIFICATION_WEIGHTS)
# recogChar=model=keras.models.load_model("./src/weights/bestmodel2-9.hdf5")
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
    cv2.imshow("gray",gray)
    cv2.waitKey(0)
    # image = cv2.imread(r"C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1-2.jpg")
    # gray = cv2.cvtColor(image,cv2.COLOR_BGR2GRAY)
            # crop number plate used by bird's eyes view transformation
# LpRegion = perspective.four_point_transform("C:\Users\hoang\Downloads\License-Plate-Recognition-master\samples\1.jpg", pts)
    V = cv2.split(cv2.cvtColor(LpRegion, cv2.COLOR_BGR2HSV))[2]

        # adaptive threshold
    T = threshold_local(V, 15, offset=10, method="gaussian")
    thresh = (V > T).astype("uint8") * 255
    cv2.imshow("thresh",thresh)
    cv2.waitKey(0)
        # convert black pixel of digits to white pixel
    thresh = cv2.bitwise_not(thresh)
    thresh = imutils.resize(thresh, width=400)
    thresh = cv2.medianBlur(thresh, 5)
    cv2.imshow("thresh",thresh)
    cv2.waitKey(0)
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
                cv2.imshow("roi",candidate)
                cv2.waitKey(0)
                # square_candidate = convert2Square(candidate)
                square_candidate = cv2.resize(candidate, (28, 28), cv2.INTER_AREA)
                square_candidate = square_candidate.reshape((28, 28, 1))
                cv2.imshow("roi",square_candidate)
                cv2.waitKey(0)
                candidates.append((square_candidate, (y, x)))
    characters = []
    coordinates = []

    for char, coordinate in candidates:
        characters.append(char)
        coordinates.append(coordinate)

    characters = np.array(characters)
    result = recogChar.predict_on_batch(characters)
    result_idx = np.argmax(result, axis=1)

    candidates = []
    for i in range(len(result_idx)):
        if result_idx[i] == 31:    # if is background or noise, ignore it
            continue
        candidates.append((ALPHA_DICT[result_idx[i]], coordinates[i]))
    first_line = []
    second_line = []

    for candidate, coordinate in candidates:
        if candidates[0][1][0] + 40 > coordinate[0]:
            first_line.append((candidate, coordinate[1]))
        else:
            second_line.append((candidate, coordinate[1]))

    def take_second(s):
        return s[1]

    first_line = sorted(first_line, key=take_second)
    second_line = sorted(second_line, key=take_second)

    if len(second_line) == 0:  # if license plate has 1 line
        license_plate = "".join([str(ele[0]) for ele in first_line])
    else:   # if license plate has 2 lines
        license_plate = "".join([str(ele[0]) for ele in first_line]) + "-" + "".join([str(ele[0]) for ele in second_line])

    print(license_plate)