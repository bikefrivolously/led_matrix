#!/usr/bin/env python3

import serial
import sys
import time

file_name = sys.argv[1]

f = open(file_name, 'rb')
s = serial.Serial('/dev/ttyACM0', 921600)

#frame = f.read(64*64*3)

frame = f.read(64*64*3)
while frame:
#    print('Starting')
    print(s.write(frame))
#    print('Done')
#    time.sleep(0.03)
    frame = f.read(64*64*3)
    
s.close()
f.close()

