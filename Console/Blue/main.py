from machine import Pin
import sys
import time

sys.stdout.write('Hello world!')
bt_select = Pin(7, Pin.IN, Pin.PULL_DOWN)
bt_erase = Pin(6, Pin.IN, Pin.PULL_DOWN)
bt_right = Pin(27, Pin.IN, Pin.PULL_DOWN)
bt_left = Pin(26, Pin.IN, Pin.PULL_DOWN)
pressed = False
while True:
    pressed = False
    if bt_select.value():
        pressed = True
        sys.stdout.write('T')
    if bt_erase.value():
        pressed = True
        sys.stdout.write('E')
    if bt_right.value():
        pressed = True
        sys.stdout.write('R')
    if bt_left.value():
        pressed = True
        sys.stdout.write('L')
    if not pressed:
        sys.stdout.write('S')
        
    time.sleep(0.05)
        
    

