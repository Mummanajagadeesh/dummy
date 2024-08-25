import cv2
import face_recognition
import numpy as np
import time
import os
from datetime import datetime
import RPi.GPIO as GPIO

known_face_encodings = []
known_face_names = []

known_faces_dir = "known_faces/"


for filename in os.listdir(known_faces_dir):
    image = face_recognition.load_image_file(os.path.join(known_faces_dir, filename))
    encoding = face_recognition.face_encodings(image)[0]
    known_face_encodings.append(encoding)
    known_face_names.append(os.path.splitext(filename)[0])


GPIO.setmode(GPIO.BCM)
GPIO.setup(18, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # GPIO pin for mode switch


MODES = {
    "day_relaxed": "Owner is in house (Daytime)—relaxed monitoring",
    "day_active": "Owner is not in house (Daytime)—active surveillance",
    "night_relaxed": "Owner is in house (Nighttime)—nighttime relaxed",
    "night_full": "Owner is not in house (Nighttime)—full security",
    "vacation": "Vacation mode—for extended absence",
    "visitor": "Visitor mode—manual verification",
    "sleep": "Sleep mode",
    "custom": "Custom mode"
}

current_mode = "day_relaxed"


def change_mode():
    global current_mode
    input_state = GPIO.input(18)
    hour = datetime.now().hour
    
    if input_state == False:  # Button pressed
        current_mode = "custom"  # Assuming button press is for custom mode
        print(f"Mode switched to: {MODES[current_mode]}")
        time.sleep(0.5)
    elif 8 <= hour < 18:
        current_mode = "day_active" if current_mode == "vacation" or current_mode == "visitor" else "day_relaxed"
    else:
        current_mode = "night_full" if current_mode == "vacation" or current_mode == "visitor" else "night_relaxed"
    print(f"Current mode: {MODES[current_mode]}")


def process_frame(frame):
    rgb_frame = frame[:, :, ::-1]  # Convert BGR to RGB
    face_locations = face_recognition.face_locations(rgb_frame)
    face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)
    
    for face_encoding, face_location in zip(face_encodings, face_locations):
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
        face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        best_match_index = np.argmin(face_distances)
        
        if matches[best_match_index]:
            name = known_face_names[best_match_index]
            print(f"Recognized: {name}")
        else:
            print("Unknown face detected! Sending alert...")
            # Trigger alert mechanism, e.g., send email or SMS
    
    return len(face_locations) > 0


video_capture = cv2.VideoCapture(0)
last_detected_time = time.time()

try:
    while True:
        change_mode()
        
        ret, frame = video_capture.read()
        if not ret:
            print("Failed to grab frame")
            break
        
        face_detected = process_frame(frame)
        
        if face_detected:
            last_detected_time = time.time()
        else:
            if time.time() - last_detected_time > 300: 
                print("No face detected for a while, entering relaxed mode...")
                time.sleep(1)
        
        cv2.imshow('Video', frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    video_capture.release()
    cv2.destroyAllWindows()
    GPIO.cleanup()
