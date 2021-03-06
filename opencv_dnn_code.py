import numpy as np
import cv2
import time
PROTOTXT = "MobileNetSSD_deploy.prototxt"
MODEL = "MobileNetSSD_deploy.caffemodel"
GPU_SUPPORT = 0
CLASSES = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus",  "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike", "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]
COLORS = np.random.uniform(0, 255, size=(len(CLASSES), 3))

net = cv2.dnn.readNetFromCaffe(PROTOTXT, MODEL)

cap = cv2.VideoCapture(0)
while True:
    ret, frame = cap.read()
    # tick = time.time()
    if not ret:
       break
    h, w = frame.shape[:2]
    blob = cv2.dnn.blobFromImage(frame, 0.007843, (300, 300), 127.5)
    net.setInput(blob)
    detections = net.forward()
    for i in np.arange(0, detections.shape[2]):
       confidence = detections[0, 0, i, 2]
       if confidence > 0.5:
           idx = int(detections[0, 0, i, 1])
           box = detections[0, 0, i, 3:7] * np.array([w, h, w, h])
           (startX, startY, endX, endY) = box.astype("int")
           label = "{}: {:.2f}%".format(CLASSES[idx],confidence*100)
           cv2.rectangle(frame, (startX, startY), (endX, endY),    COLORS[idx], 2)
           y = startY - 15 if startY - 15 > 15 else startY + 15
        #    tock = time.time()
           cv2.putText(frame, label, (startX, y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, COLORS[idx], 2)
    # cv2.putText(frame, str(1/(tock-tick)), (10, 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0,250,0), 5)
    cv2.imshow("ww",frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break