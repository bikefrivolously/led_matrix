#!/usr/bin/env python3

def f1():
    print('{ ', end='')

    for j in range(64):
        for i in range(64):
            val = i//8 + j//8
            if i % 2 == 0:
                print('0x', end='')
            
            print('{:x}'.format(val), end='')
            
            if i % 2 == 1 and i < 63:
                print(', ', end='')
        if j < 63:
            print(',')
        else:
            print(' };')

def f2():
    print('{ ', end='')

    for j in range(64):
        for i in range(64):
            val = (i//4) % 16
            if i % 2 == 0:
                print('0x', end='')
            
            print('{:x}'.format(val), end='')
            
            if i % 2 == 1 and i < 63:
                print(', ', end='')
        if j < 63:
            print(',')
        else:
            print(' };')
            
def f3():
    print('{ ', end='')

    for j in range(64):
        for i in range(64):
            val = (i//2) % 32
            print('{:#x}'.format(val), end='')
            
            if i < 63:
                print(', ', end='')
        if j < 63:
            print(',')
        else:
            print(' };')


f3()
