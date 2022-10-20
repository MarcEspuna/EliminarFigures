import serial
import socket
import sys

usb_port = raw_input("Introduce the USB port (default = '/dev/ttyACM0'):\n")
if usb_port == "":
    usb_port = '/dev/ttyACM0'

ip = raw_input("Introduce the game IP (default = 'localhost'):\n")
port = raw_input("\nIntroduce the game Port (default = 10000):\n")
if ip == "":
    ip = 'localhost'
if port == "":
    port = 10000

sckt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
datos_servidor = (ip, int(port))
sckt.connect(datos_servidor)

ser = serial.Serial(usb_port, baudrate=9600)
while True:
    try:
        output = ''
        while output == '':
            output = ser.read(1)
        print(output)
        sckt.send(output)
    except KeyboardInterrupt:
        sckt.close()
        sys.exit()
