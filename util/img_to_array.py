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
    if bpp < 8:
        colours = 2**bpp
        p = p // (256 // colours)
    if x % 64 == 0:
        print('    ', end='')
    
    print('{:#x},'.format(p), end='')
    
    if x % 64 == 63:
        print()
    x += 1
print('};')

im.close()
