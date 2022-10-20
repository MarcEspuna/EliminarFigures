from machine import Pin
import sys
import time

sys.stdout.write('Hello world!')
bt_select = Pin(7, Pin.IN, Pin.PULL_DOWN)
bt_erase = Pin(6, Pin.IN, Pin.PULL_DOWN)
bt_up = Pin(27, Pin.IN, Pin.PULL_DOWN)
bt_down = Pin(26, Pin.IN, Pin.PULL_DOWN)
pressed = False
while True:
    pressed = False
    if bt_select.value():
        pressed = True
        sys.stdout.write('T')
    if bt_erase.value():
        pressed = True
        sys.stdout.write('E')
    if bt_up.value():
        pressed = True
        sys.stdout.write('U')
    if bt_down.value():
        pressed = True
        sys.stdout.write('D')
    if not pressed:
        sys.stdout.write('S')
        
    time.sleep(0.05)
    


