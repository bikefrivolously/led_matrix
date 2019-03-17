#!/usr/bin/env python3

import math
import serial
import sys
import time
from collections import defaultdict
import heapq


def main():
    file_name = sys.argv[1]

    f = open(file_name, 'rb')
    tree = huffman_tree(f)
    codes = huffman_codes(tree, '')
    #s = serial.Serial('/dev/ttyACM0', 921600)

    #frame = f.read(64*64*3)
    #while frame:
        #stuffed = cobs_stuff(frame)
    #    print('Starting')
        #print(s.write(frame))
    #    print('Done')
    #    time.sleep(0.03)
    #    frame = f.read(64*64*3)

    #s.close()

    fo = open('test_videos/t1_compress.bin', 'wb')
    huffman_encode(f, fo, codes)
    fo.close()
    f.close()

def rle(data):
    output = bytearray()
    index = 1
    count = 1
    while index < len(data):
        prev = data[index-1]
        curr = data[index]
        if prev == curr:
            # repeat
            count += 1
            if count == 0xFF:
                output.append(prev)
                output.append(prev)
                output.extend(count.to_bytes(1, 'big'))
                count = 1
        elif count > 1:
            output.append(prev)
            output.append(prev)
            output.extend(count.to_bytes(1, 'big'))
            count = 1
        else:
            output.append(prev)
        index += 1

    return output

class HuffmanNode:
    def __init__(self, symbol=None, weight=0.0):
        self.weight = weight
        self.symbol = symbol
        self.parent = None
        self.left = None
        self.right = None

    def isleaf(self):
        return self.left is None and self.right is None

    def add_children(self, left, right):
        self.left = left
        self.right = right
        self.left.parent = self
        self.right.parent = self
        self.weight = self.left.weight + self.right.weight

    def __lt__(self, other):
        return self.weight < other.weight

    def __str__(self):
        return '({}, {})'.format(self.weight, self.symbol)

def huffman_tree(f):
    f.seek(0)
    frequency =  defaultdict(int)
    h = []
    pixels = 0
    pixel = f.read(3)
    while pixel:
        pixels += 1
        frequency[pixel] += 1
        if pixels % 100000 == 0:
            print(pixels, len(frequency))
        pixel = f.read(3)

    for k, v in frequency.items():
        heapq.heappush(h, HuffmanNode(k, v/pixels)) 

    while len(h) > 1:
        n1 = heapq.heappop(h)
        n2 = heapq.heappop(h)
        parent = HuffmanNode()
        parent.add_children(n1, n2)
        heapq.heappush(h, parent)

    return h[0]

def huffman_codes(root, code, codes=None):
    if codes is None:
        codes = dict()
    if root.left:
        huffman_codes(root.left, code + '0', codes)
    if root.right:
        huffman_codes(root.right, code + '1', codes)

    if root.isleaf():
        codes[root.symbol] = code

    return codes

def huffman_encode(fin, fout, codes):
    fin.seek(0)
    encoded_string = ''
    pixel = fin.read(3)
    while pixel:
        encoded_string += codes[pixel]
        while len(encoded_string) >= 8:
            fout.write(int(encoded_string[0:8], 2).to_bytes(1, 'big'))
            if len(encoded_string) == 8:
                encoded_string = ''
            else:
                encoded_string = encoded_string[8:]
        pixel = fin.read(3)

def huffman_store_tree(root):
    if root.isleaf():
        return (1 << 24) & root.symbol




def cobs_stuff(data):
    output = bytearray()
    start = 0
    end = start + 254

    data = bytearray(data)
    data += b'\x00'

    while start < len(data):
        index = data.find(0x00, start, end)
        #print('search in: {}'.format(''.join('{:02x}'.format(x) for x in data[start:end])))
        #print('start: {}, end: {}, index: {}'.format(start,end,index))
        if index == -1:
            chunk = data[start:end]
        else:
            chunk = data[start:index+1]

        #print('chunk: {}'.format(''.join('{:02x}'.format(x) for x in chunk)))

        if chunk[-1] == 0x00:
            del chunk[-1]

        chunklen = len(chunk) + 1
        packed = chunklen.to_bytes(1, 'big') + chunk
        #print('packed: {}'.format(''.join('{:02x}'.format(x) for x in packed)))
        output.extend(packed)
        start += chunklen
        end = start + 254

    return output



if __name__ == '__main__':
    main()
