from socket import timeout
import time
import torch
import cv2
import numpy as np
from mss import mss
import keyboard
import serial



model = torch.hub.load(r'C:\Users\yohan\OneDrive\Desktop\ai_in_game\scripts\yolov5-master','custom', path=r'C:\Users\yohan\OneDrive\Desktop\ai_in_game\scripts\best.pt', source='local' )
arduino = serial.Serial("com7", "115200", timeout = .1)

def SendCordinates(code):
    code = str(code)
    arduino.write(str.encode(code))

def CalculateDistance(x,y):
    if x < 0:
        x *=-1
        x_d = "n"
    else:
        x_d = "p"
    if y < 0:
        y*=-1
        y_d = "n"
    else:
        y_d = "p"

    x_v = int(x/5)
    y_v = int(y/5)
    code = x_d + "," + str(x_v) + "," + y_d + "," + str(y_v) + "*"

    return code

with mss() as sct:
    monitor = {"top": 220, "left": 640, "width": 640, "height":640}

    while(True):
        screenshot = np.array(sct.grab(monitor))
        results = model(screenshot, size=640)
        df= results.pandas().xyxy[0]
        try:
            xmin = int(df.iloc[0,0])
            ymin = int(df.iloc[0,1])
            xmax = int(df.iloc[0,2])
            ymax = int(df.iloc[0,3])

            head_level = (int(xmin + (xmax - xmin) / 2), int(ymin + (ymax - ymin) / 8))
            cv2.circle(screenshot, head_level, 4, (0,255,0), thickness = -1)
            cv2.rectangle(screenshot, (xmin, ymin), (xmax, ymax), (255,0,0), 2)

            distance = (head_level[0] - 320, head_level[1] -320)
            
            if keyboard.is_pressed('v'):
                code = CalculateDistance(int(distance[0]), int(distance[1]))
                SendCordinates(code)
                time.sleep(0.175)
        except:
            print("",end="")

        cv2.imshow("frame", screenshot)
        if(cv2.waitKey(1) == ord('q')):
            cv2.destroyAllWindows()
            break