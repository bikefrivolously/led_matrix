#!/usr/bin/env python3

from PIL import Image
import sys
import math

im = Image.open(sys.argv[1])
if len(sys.argv) > 2:
    bpp = int(sys.argv[2])
else:
    bpp = 8

data = list(im.getdata())

x = 0
print('{')
for p in data:
    r = p[0]
    g = p[1]
    b = p[2]
    
    if bpp < 8:
        colours = 2**bpp
        r = r // (256 // colours)
        g = g // (256 // colours)
        b = b // (256 // colours)
    if x % 64 == 0:
        print('    ', end='')
    
    #print('{{{:#x}, {:#x}, {:#x}}}, '.format(r, g, b), end='')
    print(f'{{{r:#x}, {g:#x}, {b:#x}}}, ', end='')
    
    if x % 64 == 63:
        print()
    x += 1
print('};')

im.close()
