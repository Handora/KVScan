#!/usr/bin/python3

import os
import struct

filename = "/tmp/kvscan/scan.db"


def createFile():
    if os.path.exists(filename):
        os.remove(filename)
    f = open(filename, 'wb')
    return f


def writeFile(f, offset, data):
    f.seek(offset)
    f.write(data)


def main():
    f = createFile()
    f.write(struct.pack('i', 0))
    f.write(struct.pack('i', 1))
    f.write(struct.pack('i', -1))
    f.write(struct.pack('i', 2))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'a' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512)

    f.write(struct.pack('i', 1))
    f.write(struct.pack('i', 3))
    f.write(struct.pack('i', 0))
    f.write(struct.pack('i', -1))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'c' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*2)

    f.write(struct.pack('i', 2))
    f.write(struct.pack('i', 0))
    f.write(struct.pack('i', -1))
    f.write(struct.pack('i', -1))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'b' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*3)

    f.write(struct.pack('i', 3))
    f.write(struct.pack('i', -1))
    f.write(struct.pack('i', 1))
    f.write(struct.pack('i', 4))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'd' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*4)

    f.write(struct.pack('i', 4))
    f.write(struct.pack('i', 3))
    f.write(struct.pack('i', 5))
    f.write(struct.pack('i', 6))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'f' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*5)

    f.write(struct.pack('i', 5))
    f.write(struct.pack('i', 4))
    f.write(struct.pack('i', -1))
    f.write(struct.pack('i', -1))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'e' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*6)

    f.write(struct.pack('i', 6))
    f.write(struct.pack('i', 4))
    f.write(struct.pack('i', -1))
    f.write(struct.pack('i', -1))

    for i in range(4):
        f.write(struct.pack('i', 58))
        f.write(bytearray(56 * 'a' + 'g' + chr(ord('a')+i), 'utf-8'))
        f.write(struct.pack('i', 58))
        f.write(bytearray(58 * 'a', 'utf-8'))

    f.flush()
    assert(os.path.getsize(filename) == 512*7)


main()
