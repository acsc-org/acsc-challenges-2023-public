#!/usr/bin/env python3
import lief
from ctypes import *

from binascii import hexlify, unhexlify
import sys
import struct
import hashlib
import os

rng_so = CDLL("./rng.so")
rng_so.genrand64_int64.restype = c_ulonglong

# binary = lief.parse("./snake")
# text = binary.get_section(".text")

SUBMISSION_SIZE = 32

data = sys.stdin.buffer.read(SUBMISSION_SIZE)

seed, maxX, maxY, score, digest = struct.unpack("<IIII16s", data)
# print("seed: ", seed)
# print("maxX: ", maxX)
# print("maxY: ", maxY)
# print("score: ", score)
# print("digest: ", hexlify(digest))

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

if md5.digest() != digest:
    print("Cheater Detected!")

else:
    response = "=== LEADERBOARD ===\n"
    response += "1. johnwick   31337\n"
    response += "2. ishowsewey 12\n"
    response += "3. vinh       4\n"
    response += "4. cakash     3\n"
    response += "5. weekek     2\n"
    response += "\n"

    if score > 31337 and md5.digest() == digest:
        response += "Congrats! You beat the highscore!\n"
        response += open("./flag.txt").read()
    else:
        response += "Your score is so low...\n"
        response += "I don't bother updating the leaderboard"

    print(response)
