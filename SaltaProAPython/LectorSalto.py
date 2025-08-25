import serial
import pyautogui

arduino = serial.Serial('/dev/ttyACM0', 9600)  # Asegurate que COM3 sea el puerto correcto

while True:
    data = arduino.readline().decode().strip()
    if data == "JUMP":
        pyautogui.click()
        print("SALTO")