#!/usr/bin/env python3
import lief
from ctypes import *

from binascii import hexlify, unhexlify
import struct
import hashlib
from pwn import *

HOST = "34.81.146.180"
PORT = 4444

rng_so = CDLL("../server/rng.so")
rng_so.genrand64_int64.restype = c_ulonglong

seed = 100
score = 31338
maxX = 100
maxY = 100

md5 = hashlib.md5()

# process seed
rng_so.init_genrand64(seed)
md5.update(struct.pack("<I", seed))

# generate foods based on score
for i in range(score):
    food_x = rng_so.genrand64_int64() % (maxX // 2)
    food_y = rng_so.genrand64_int64() % maxY
    md5.update(struct.pack("<II", food_x, food_y))

text = open("text.dump", "rb").read()
md5.update(text)

r = remote(HOST, PORT)
r.send(p32(seed) + p32(maxX) + p32(maxY) + p32(score) + md5.digest())

r.interactive()
