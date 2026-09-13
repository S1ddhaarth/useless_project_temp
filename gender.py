import os
import sys
import serial

# 1. Force OpenCV / Qt to use XWayland instead of missing wayland plugin
os.environ["QT_QPA_PLATFORM"] = "xcb"

# 2. Configure pip-installed NVIDIA CUDA libraries in LD_LIBRARY_PATH
if "_CUDA_ENV_LOADED" not in os.environ:
    import site

    submodules = [
        "cudnn", "cublas", "cuda_runtime", "cuda_nvrtc",
        "cufft", "curand", "cusolver", "cusparse", "nccl", "nvjitlink"
    ]
    
    cuda_dirs = [
        os.path.join(p, "nvidia", sub, "lib")
        for p in site.getsitepackages()
        for sub in submodules
    ]
    valid_cuda_dirs = [p for p in cuda_dirs if os.path.isdir(p)]

    if valid_cuda_dirs:
        current_ld = os.environ.get("LD_LIBRARY_PATH", "")
        os.environ["LD_LIBRARY_PATH"] = ":".join(valid_cuda_dirs) + (":" + current_ld if current_ld else "")
        os.environ["_CUDA_ENV_LOADED"] = "1"
        os.execv(sys.executable, [sys.executable] + sys.argv)

import cv2
from deepface import DeepFace

# Serial setup
SERIAL_PORT = "/dev/ttyUSB0"  # Change to /dev/ttyUSB0 or /dev/ttyACM0 if using standard USB-to-UART
BAUD_RATE = 115200

try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
except Exception as e:
    print(f"Warning: Failed to open serial port {SERIAL_PORT}: {e}")
    ser = None

# Set capture device to /dev/video0
cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)

if not cap.isOpened():
    print("Error: Could not open camera.")
    if ser:
        ser.close()
    exit(1)

cv2.namedWindow("Gender Identification - Live Overlay", cv2.WINDOW_NORMAL)
cv2.resizeWindow("Gender Identification - Live Overlay", 960, 540)

frame_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture frame")
        break

    frame_count += 1

    try:
        results = DeepFace.analyze(
            img_path=frame,
            actions=['gender'],
            detector_backend='opencv',
            enforce_detection=True
        )

        if isinstance(results, dict):
            results = [results]

        for face in results:
            region = face.get('region', {})
            x, y, w, h = region.get('x', 0), region.get('y', 0), region.get('w', 0), region.get('h', 0)

            if w > 0 and h > 0:
                gender_data = face.get('gender', {})
                man_conf = gender_data.get('Man', 0.0)
                woman_conf = gender_data.get('Woman', 0.0)

                # Send serial signal based on confidence thresholds
                if ser and ser.is_open:
                    if man_conf > 50.0:
                        ser.write(b'1')
                    elif woman_conf > 50.0:
                        ser.write(b'0')

                dominant = face.get('dominant_gender', 'Unknown')
                confidence = gender_data.get(dominant, 0)
                label = f"{dominant}: {confidence:.1f}%"

                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

                (text_w, text_h), _ = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.6, 2)
                cv2.rectangle(frame, (x, max(0, y - text_h - 10)), (x + text_w, y), (0, 255, 0), -1)
                cv2.putText(frame, label, (x, max(15, y - 5)),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 0, 0), 2)

    except Exception as e:
        print(f"[ERROR] DeepFace analysis failed: {type(e).__name__}: {e}")

    cv2.imshow("Gender Identification - Live Overlay", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

if ser and ser.is_open:
    ser.close()

cap.release()
cv2.destroyAllWindows()
