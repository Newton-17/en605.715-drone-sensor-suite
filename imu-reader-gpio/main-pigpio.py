#!/usr/bin/env python

import json
import time
import pigpio
from socket import *

I2C_ADDR=0x9
SDA = 10
SCL = 11

pi = pigpio.pi()
DATA = {
        'X': 0,
        'Y': 0,
        'Z': 0,
        }

def i2c(id, tick):
    global pi
    global DATA

    s, b, d = pi.bsc_i2c(I2C_ADDR)
    if b:
        new_data = d[:-1].decode()
        if new_data[0] == 'X':
            DATA['X'] = new_data[1:]
        if new_data[0] == 'Y':
            DATA['Y'] = new_data[1:]
        if new_data[0] == 'Z':
            DATA['Z'] = new_data[1:]
        

if __name__ == '__main__':
    if not pi.connected:
        exit()
    # Add pull-ups in case external pull-ups haven't been added
    #pi.set_pull_up_down(SDA, pigpio.PUD_UP)
    #pi.set_pull_up_down(SCL, pigpio.PUD_UP)
    
    # Respond to BSC slave activity
    e = pi.event_callback(pigpio.EVENT_BSC, i2c)
    pi.bsc_i2c(I2C_ADDR) # Configure BSC as I2C slave

    server_socket = socket(AF_INET, SOCK_DGRAM)
    server_socket.bind(('', 10000))
    
    while True:
        # Socket Logic
        message, address = server_socket.recvfrom(1024)
        server_socket.sendto(bytes(json.dumps(DATA), encoding="utf-8"), address) 


    e.cancel()
    pi.bsc_i2c(0) # Disable BSC peripheral
    pi.stop()
