import threading
import cv2
import numpy as np
import asyncio
import websocket
import json
import rickroll
import time

# Configuration WebSocket
# Configuration de la matrice LED
LED_MATRIX_WIDTH = 16
LED_MATRIX_HEIGHT = 8

uri = "ws://192.168.137.45/ws"
rickroll.ws = websocket.WebSocket()
rickroll.ws.connect(uri)

def image_to_led_matrices(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    resized = cv2.resize(gray, (LED_MATRIX_WIDTH, LED_MATRIX_HEIGHT))
    _, binary = cv2.threshold(resized, 127, 255, cv2.THRESH_BINARY)

    matrix_left = []
    matrix_right = []

    for row in binary:
        left_byte = 0
        right_byte = 0
        for i in range(LED_MATRIX_WIDTH // 2):
            if row[i] > 0:
                left_byte |= 1 << (7 - i)
            if row[LED_MATRIX_WIDTH // 2 + i] > 0:
                right_byte |= 1 << (7 - i)
        matrix_left.append(left_byte)
        matrix_right.append(right_byte)

    return matrix_left, matrix_right

# Envoi des données via WebSocket
def send_frame(websocket, matrix_left, matrix_right):
    data = {
        "cmd": 5,
        "data": {
            "matrixLeft": matrix_left,
            "matrixRight": matrix_right
        }
    }
    rickroll.ws.send(json.dumps(data))

# Lecture de la vidéo et envoi des images
def stream_video(video_path):
    cap = cv2.VideoCapture(video_path)
    frame_count = 0

    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # Prendre une frame toutes les 6 frames:
        matrix_left, matrix_right = image_to_led_matrices(frame)
        send_frame(websocket, matrix_left, matrix_right)
        time.sleep(0.03)  # 200 ms par image

        frame_count += 1

    cap.release()


# Exécution du script
video_path = "bad.mp4"  # Remplacez par le chemin de votre vidéo

video_thread = threading.Thread(target=lambda: stream_video(video_path))
midi_thread = threading.Thread(target=lambda: rickroll.main("bad_apple.mid", 1.3))



# Démarrage des threads
video_thread.start()
time.sleep(9)
midi_thread.start()

# Attendre que les threads se terminent
video_thread.join()
midi_thread.join()